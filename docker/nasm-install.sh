#!/bin/bash
here_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

#
# ovmf needs newer version of nasm than included in ubuntu 20.04
#
wget http://www.nasm.us/pub/nasm/releasebuilds/2.16.01/nasm-2.16.01.tar.bz2
tar xfj nasm-2.16.01.tar.bz2
cd nasm-2.16.01/
./autogen.sh
./configure --prefix=/usr/local/ 
make 
make install