#!/bin/bash
set -euo pipefail

#
# builds sshd service
#

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
working_dir=$script_dir/build

COMMIT=b6b49130a0089b297245ee39e769231d7c763014
REPO=https://github.com/openssh/openssh-portable.git
DIRNAME=openssh-portable

cd $working_dir

mkdir -p $working_dir

if [ -d $working_dir/$DIRNAME ]; then
    echo "Directory already exists"
else
    git clone $REPO
    cd $working_dir/$DIRNAME
    git checkout $COMMIT
    git apply $script_dir/sshd.diff
fi

cd $working_dir/$DIRNAME

set -x

install_dir=$script_dir/build/sshd
mkdir -p $install_dir

autoreconf
./configure --prefix $install_dir
make -j $(nproc)
make install
cp -rf $script_dir/sshd_config $install_dir/etc

ls -al $install_dir