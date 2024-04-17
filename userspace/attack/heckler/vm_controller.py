import os
import re
import subprocess
import threading
from time import sleep

from heckler.boot_filter import BootFilter, LoginBootFilter

script_dir = os.path.dirname(os.path.realpath(__file__))

class VmController:
    def __init__(self, boot_filter: BootFilter, vm_run_script: str):
        self.pgid = None
        self.task = None
        self.boot_filter = boot_filter
        self.vm_run_script = vm_run_script

    @property
    def stdout(self):
        for stdout_line in iter(self.task.stdout.readline, ""):
            yield stdout_line
        return self.task.stdout.read()

    @property
    def stderr(self):
        for stdout_line in iter(self.task.stderr.readline, ""):
            yield stdout_line

    def _capture_pages(self, verbose=False):
        BLUE = '\033[34m'
        RESET = '\033[0m'

        for msg in self.stdout:
            self.boot_filter.parse_bootlog(msg)
            if verbose:
                cleaned = re.sub('\[[0-9;]+[a-zA-Z]', ' ', msg)
                print(BLUE + "[vm] " + cleaned + RESET, end="")

    def _kill_instances(self):
        try:
            cmd = "pgrep qemu | xargs -I{} sudo kill -9 {}".split()
            subprocess.call(cmd, shell=True)
        except PermissionError as e:
            print("Permission Error!")
            print(e)

    def launch(self, verbose=False, daemon=True):
        print('using run_script: ', self.vm_run_script)
        self.task = subprocess.Popen(["bash", self.vm_run_script],
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     stdin=subprocess.DEVNULL,
                                     preexec_fn=os.setsid, bufsize=10, universal_newlines=True)

        self.pgid = os.getpgid(self.task.pid)
        th = threading.Thread(target=self._capture_pages, args=[verbose], daemon=daemon)
        th.start()

    def stop(self):
        print("Pgid: %s, pid: %s" % (self.pgid, self.task.pid))
        try:
            cmd = "pgrep qemu | xargs -I{} sudo kill -9 {}".split()
            subprocess.call(cmd, shell=True)
            self._kill_instances()
        except PermissionError:
            print("Permission Error!")
            print("Calling sudo kill %d" % self.pgid)
            subprocess.check_call(["sudo", "kill", str(self.pgid)])
            print("B")


if __name__ == '__main__':
    filter = LoginBootFilter()

    run_script = script_dir + "/../../../run.sh"
    VmController(boot_filter=filter, vm_run_script=run_script).launch(verbose=True)

    while not filter.has_booted():
        print('waiting for boot finish')
        sleep(2)
    print('has booted!')
