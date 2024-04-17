class BootFilter:
    def __init__(self, has_booted_cb=None):
        self.has_booted_cb = has_booted_cb

    def mark_as_booted(self):
        if self.has_booted_cb is not None:
            self.has_booted_cb()

    def has_booted(self):
        return False

    def parse_bootlog(self, msg):
        pass


class BeforeInitBootFilter(BootFilter):
    END_OF_BOOT = "Welcome to"

    def __init__(self, has_booted_cb = None):
        super().__init__(has_booted_cb)
        self.booted = False

    def has_booted(self):
        return self.booted

    def parse_bootlog(self, msg):
        if self.END_OF_BOOT in msg:
            self.booted = True
            self.mark_as_booted()


class LoginBootFilter(BootFilter):
    END_OF_BOOT = "Record Runlevel Change in UTMP"

    def __init__(self, has_booted_cb = None):
        super().__init__(has_booted_cb)
        self.booted = False

    def has_booted(self):
        return self.booted

    def parse_bootlog(self, msg):
        if self.END_OF_BOOT in msg:
            self.booted = True
            self.mark_as_booted()


class SshBootFilter(BootFilter):
    # boot logs:
    # [  176.038064] sshd[1319]: /usr/local/etc/sshd_config line 85: Unsupported option UsePAM
    # [  176.038677] sshd[1319]: auth_password phys_page: 0x0000000033820fa0| virt_addr: 0x000055d95252efa0
    # [  176.038812] sshd[1319]: mm_answer_authpassword phys_page: 0x00000000320eb260 | virt_addr: 0x000055d952552260
    # [  176.038933] sshd[1319]: freezero phys_page: 0x0000000033fa2b00 | virt_addr: 0x000055d9525dcb00
    # [  176.039053] sshd[1319]: sshbuf_reset phys_page: 0x0000000033a79880 | virt_addr: 0x000055d952569880
    # [  176.039193] sshd[1319]: xcrypt phys_page: 0x0000000033fa1cc0 | virt_addr: 0x000055d9525d8cc0
    sshd_filter_context_keys = ['auth_password phys_page:',
                                'mm_answer_authpassword phys_page:',
                                'xcrypt phys_page:']

    sshd_filter_auth_password = sshd_filter_context_keys[0]
    sshd_filter_mm_answer_authpassword = sshd_filter_context_keys[1]

    def __init__(self, has_booted_cb = None, trace=False):
        super().__init__(has_booted_cb)
        self.page_sshd_auth_password = 0
        self.page_sshd_mm_answer_authpassword = 0
        self.page_sshd_context = {}
        self.booted = False
        self.trace = trace

        self.max_find = len(self.sshd_filter_context_keys)
        self.found = 0

    def _parse(self, line, split, name):
        val = int(line.split(split)[1].split("|")[0].strip(), 16)
        if self.trace:
            print('found (unaligned) ' + name + ": ", val)
        val_aligned = val - val % 4096
        if self.trace:
            print('found ' + name + ": ", val_aligned)
        return val_aligned, val

    def _page_context_name(self, key):
        return key.split(" ")[0]

    def has_booted(self):
        return self.booted

    def parse_bootlog(self, msg):
        for filter_key in self.sshd_filter_context_keys:
            name = self._page_context_name(filter_key)
            if filter_key in msg:
                page, page_unaligned = self._parse(msg, filter_key, name)
                self.page_sshd_context[name] = page
                self.page_sshd_context[name + "_unaligned"] = page_unaligned
                self.found = self.found + 1

            if self.found >= self.max_find and not self.booted:
                self.page_sshd_auth_password = \
                    self.page_sshd_context[self._page_context_name(self.sshd_filter_auth_password)]

                self.page_sshd_mm_answer_authpassword = \
                    self.page_sshd_context[self._page_context_name(self.sshd_filter_mm_answer_authpassword)]

                if self.trace:
                    print('found all addresses', self.page_sshd_context)
                self.booted = True
                self.mark_as_booted()


if __name__ == '__main__':
    pass
