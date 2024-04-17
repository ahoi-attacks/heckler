# heckler/sudo

Sudo utilities for heckler.

Place ./pam_unix_ubuntu_23_10.so into /lib/x86_64-linux-gnu/security/pam_unix.so
in CVM. pam_unix_ubuntu_23_10 is unmodified pam shared library from ubuntu 23.10.

```
make
```

Find target binaries in:

- ./profile_pam: Ping pong profiling of pam gadgets (to run in CVM).



