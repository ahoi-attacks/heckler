#!/bin/bash
set -euo pipefail

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
root_dir=$script_dir/../

#
# simple ci for some basic tests
#

function check_pgm() {
    local pgm=$1

    if ! command -v $pgm &> /dev/null
    then
        echo "$pgm could not be found"
        exit 1
    fi
}


function ssh() {
    cd $root_dir/userspace/ssh

    #
    # github runner fails
    # /usr/bin/mkdir: cannot create directory ‘/var/empty’: Permission denied
    # ignore
    make || true
}

function sudo() {
    cd $root_dir/userspace/sudo
    make

}

function kernel() {
    set -x
    source $root_dir/stable-commits

    mkdir -p $root_dir/linux/host
    git clone --depth 1 -b $KERNEL_HOST_BRANCH $KERNEL_GIT_URL $root_dir/linux/host || true
    cd $root_dir/linux/host
    make headers
}

function attack() {
    set -x
    cd $root_dir/userspace/sev-step
    make

    cd $root_dir/userspace/heckler_bindings
    make

    cd $root_dir/userspace/attack
    make

    make test
}

check_pgm python
check_pgm make
check_pgm gcc


ssh
sudo
kernel
attack


