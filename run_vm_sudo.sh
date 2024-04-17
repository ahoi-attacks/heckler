#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR

function run {
	local disk=$1
    local mount=$HOME
    local kernel=$SCRIPT_DIR/linux/guest/arch/x86/boot/bzImage
    
    cd $SCRIPT_DIR
    QEMU_CMD="sudo ./launch-qemu-shell.sh -hda $disk -smp 1 -kernel $kernel -sev-snp"

    
    echo "Remapping CTRL-C to CTRL-]"
    stty intr ^]
    echo "Launch VM:"
    eval ${QEMU_CMD}
    # restore CTRL-C mapping
    stty intr ^c
}

#
# requires vm image in qcow2 format in current directory
#
run $SCRIPT_DIR/VM_DISK_sudo.qcow2


