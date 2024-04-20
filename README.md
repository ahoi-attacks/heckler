[![build-heckler-userspace](https://github.com/ahoi-attacks/heckler/actions/workflows/ci.yaml/badge.svg)](https://github.com/ahoi-attacks/heckler/actions/workflows/ci.yaml)
[![License: MIT](https://img.shields.io/badge/USENIX%20Security%20'24-Accepted%20Paper-blue)](https://www.usenix.org/conference/usenixsecurity24/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

# Heckler
>
> Breaking Confidential VMs with Malicious Interrupts  
> (USENIX Security 2024)  
> https://ahoi-attacks.github.io/heckler/
>

### Getting Started

Build host and guest kernel as well as VMM in a docker container.

```bash
# On your Host
cd ./docker
./docker-build.sh
./docker-run.sh

# In the container
$ ./build.sh --package
```

This builds host and guest kernel, qemu and ovmf for your host machine.
On succesful build, the binaries will be available in `heckler-snp-release-<DATE>`.

```bash
# On your Host
cd heckler-snp-release-<DATE>
sudo ./install.sh
````

### Prepare Host
Follow up on [Prepare Host](./README-amdsev.md#prepare-host) to set up your host for SEV-SNP.
We recommend the following additional kernel arguments. 

```sh
GRUB_CMDLINE_LINUX_DEFAULT="transparent_hugepage=never rcupdate.rcu_cpu_stall_suppress=1 nmi_watchdog=0"
```

We ran our experiments on an AMD 4th gen EPYC 9124 16-Core Processor. However, any other AMD SEV SNP capable processor should work as well.

### VM Image
Create an Ubuntu 22.04/23.10 image in qcow2 format. See [./run.sh](./run_vm_sudo.sh) script.

### Attack
- [Sudo Attack](./userspace/attack/heckler/sudo/sudo_attack.py)
- [SSH Attack](./userspace/attack/heckler/ssh/ssh_attack.py)

### Companion Repositories:
- [heckler-kernel](https://github.com/ahoi-attacks/heckler-linux) (`./linux/{guest|host}`): Kernel code for gadget profiling
- [heckler-qemu](https://github.com/ahoi-attacks/heckler-qemu) (`./qemu`): Compatible qemu version
- [heckler-ovmf](https://github.com/ahoi-attacks/heckler-ovmf) (`./edk2`): Compatible ovmf version

These repositories are downloaded during [project build](./build.sh). 

### CVEs
- [2024-25744](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-25744)
- [2024-25743](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-25743)
