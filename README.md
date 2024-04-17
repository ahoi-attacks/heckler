# Heckler
>
> Breaking Confidential VMs with Malicious Interrupts  
> (USENIX Security 2024)  
> https://ahoi-attacks.github.io/heckler/
>

## Getting Started

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


### VM Image
Create an Ubuntu 22.04/23.10 image in qcow2 format. See [./run.sh](./run_vm_sudo.sh) script.

### Companion Repositories:
- [heckler-kernel](https://github.com/ahoi-attacks/heckler-kernel) (`./linux/{guest|host}`): Kernel code for gadget profiling
- [heckler-qemu](https://github.com/ahoi-attacks/heckler-qemu) (`./qemu`): Compatible qemu version
- [heckler-ovmf](https://github.com/ahoi-attacks/heckler-ovmf) (`./edk2`): Compatible ovmf version

### Artifact Evaluation
This code is still a work in progress.
We plan to apply for artifact evaluation when time permits.
