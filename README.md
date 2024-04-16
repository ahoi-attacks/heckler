# Heckler

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

This builds host and guest kernel, qemu and ovmf for your host machine. Upon completion, install the host kernel on your machine.
