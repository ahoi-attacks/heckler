FROM ubuntu:22.04
ENV DEBIAN_FRONTEND noninteractive

#   apt-get -y upgrade && \
RUN \
  sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
  apt-get update 

RUN \  
  apt-get install -y build-essential && \
  apt-get install -y software-properties-common && \
  apt-get install -y curl git htop man unzip vim wget ninja-build

RUN \
  apt-get -y install emacs vim python3 python3-pip fish

# qemu build deps
RUN apt-get -y install pkg-config libglib2.0-dev
RUN \
  apt-get -y install git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev ninja-build  && \
  apt-get -y install git-email && \
  apt-get -y install libaio-dev libbluetooth-dev libcapstone-dev libbrlapi-dev libbz2-dev && \
  apt-get -y install libcap-ng-dev libcurl4-gnutls-dev libgtk-3-dev && \
  apt-get -y install libibverbs-dev libjpeg8-dev libncurses5-dev libnuma-dev && \
  apt-get -y install librbd-dev librdmacm-dev && \
  apt-get -y install libsasl2-dev libsdl2-dev libseccomp-dev libsnappy-dev libssh-dev && \
  apt-get -y install libvde-dev libvdeplug-dev libvte-2.91-dev libxen-dev liblzo2-dev && \
  apt-get -y install valgrind xfslibs-dev && \
  apt-get -y install libnfs-dev libiscsi-dev && \
  apt-get -y install p11-kit-modules p11-kit libp11-kit0 libp11-kit-dev

RUN apt-get -y install libunistring-dev libmount-dev libnfs-dev libiscsi-dev libpixman-1-dev libcairo2-dev libpango1.0-dev libjpeg8-dev libgif-dev slirp libslirp-dev 

# ovmf
RUN apt-get -y install iasl python-is-python3

# ovmf needs newer version of nasm than included in ubuntu 20.04
ADD nasm-install.sh /root
RUN bash -c "/root/nasm-install.sh"


# kernel
RUN apt install -y libncurses-dev gawk flex bison openssl libssl-dev dkms libelf-dev libudev-dev libpci-dev libiberty-dev autoconf llvm cpio zstd  build-essential libncurses-dev bison flex libssl-dev libelf-dev fakeroot rsync debhelper bc

RUN git config --global --add safe.directory '*'


# go
RUN apt install -y golang-go


# Set environment variables.
ENV HOME /root

# Define working directory.
WORKDIR /root

# Define default command.
CMD ["bash"]