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
- **heckler-kernel**: Kernel code for gadget profiling
- **heckler-qemu**: Compatible qemu version
- **heckler-ovmf**: Compatible ovmf version⏎
