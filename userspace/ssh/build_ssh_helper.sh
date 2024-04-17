#!/bin/bash
set -euo pipefail
#
# builds heckler_ssh_helper
# utility program to establish ssh connection with CVM
#

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
working_dir=$script_dir/build

COMMIT=833695f0a57b3037385dc9c0073bc88773cae6f3
REPO=https://github.com/golang/crypto.git

mkdir -p $working_dir

if [ -d $working_dir/crypto ]; then
    echo "Directory already exists"
else
    cd $working_dir
    git clone $REPO
    cd $working_dir/crypto
    git checkout $COMMIT
    git apply $script_dir/heckler_ssh_helper.diff
fi

cd $working_dir/crypto

set -x
go build
ls -al | grep heckler
