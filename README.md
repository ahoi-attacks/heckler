# Heckler
>
> Breaking Confidential VMs with Malicious Interrupts  
> (USENIX Security 2024)  
> https://ahoi-attacks.github.io/heckler/
>

## Getting Started

Build host and guest kernel as well as VMM in a docker container.

```sh
# On your Host
cd ./docker
./docker-build.sh
./docker-run.sh

# In the container
$ ./build.sh --package
```

This builds host and guest kernel, qemu and ovmf for your host machine.
Upon completion, install the host kernel on your machine.

### VM Image
Create an ubuntu 22.04 image in qcow2 format. See ./run.sh script.  

### Companion Repositories:
- [heckler-kernel](https://github.com/ahoi-attacks/heckler-kernel) (`./linux/{guest|host}`): Kernel code for gadget profiling
- [heckler-qemu](https://github.com/ahoi-attacks/heckler-qemu) (`./qemu`): Compatible qemu version
- [heckler-ovmf](https://github.com/ahoi-attacks/heckler-ovmf) (`./edk2`): Compatible ovmf version⏎

### Artifact Evaluation
This code is still a work in progress.
We plan to apply for artifact evaluation when time permits.