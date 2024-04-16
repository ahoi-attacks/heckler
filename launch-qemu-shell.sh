#!/bin/bash

#
# user changeable parameters
#
MEM="2500"
SMP="1"
VNC=":9"
CONSOLE="serial"
USE_VIRTIO="1"

SEV="0"
SEV_ES="0"
SEV_SNP="0"
ALLOW_DEBUG="0"
USE_GDB="0"
CDROM=""

EXEC_PATH="./usr/local"
UEFI_PATH="$EXEC_PATH/share/qemu"

usage() {
	echo "$0 [options]"
	echo "Available <commands>:"
	echo " -sev               launch SEV guest"
	echo " -sev-es            launch SEV guest"
	echo " -sev-snp           launch SNP guest"
	echo " -bios              the bios to use (default $UEFI_PATH)"
	echo " -hda PATH          hard disk file (default $HDA)"
	echo " -mem MEM           guest memory size in MB (default $MEM)"
	echo " -smp NCPUS         number of virtual cpus (default $SMP)"
	echo " -allow-debug       Enable SEV Debug API"
	echo " -cdrom             insert .iso image into vm."
	echo " -vnc               start given <host:d> start vnc server on host:5900 + d (thank qemu for that). Host may be omitted"
	exit 1
}

add_opts() {
	echo -n "$* " >> ${QEMU_CMDLINE}
}

exit_from_int() {
	rm -rf ${QEMU_CMDLINE}
	# restore the mapping
	stty intr ^c
	exit 1
}

run_cmd () {
	$*
	if [ $? -ne 0 ]; then
		echo "command $* failed"
		exit 1
	fi
}

get_cbitpos() {
	modprobe cpuid
	#
	# Get C-bit position directly from the hardware
	#   Reads of /dev/cpu/x/cpuid have to be 16 bytes in size
	#     and the seek position represents the CPUID function
	#     to read.
	#   The skip parameter of DD skips ibs-sized blocks, so
	#     can't directly go to 0x8000001f function (since it
	#     is not a multiple of 16). So just start at 0x80000000
	#     function and read 32 functions to get to 0x8000001f
	#   To get to EBX, which contains the C-bit position, skip
	#     the first 4 bytes (EAX) and then convert 4 bytes.
	#

	EBX=$(dd if=/dev/cpu/0/cpuid ibs=16 count=32 skip=134217728 | tail -c 16 | od -An -t u4 -j 4 -N 4 | sed -re 's|^ *||')
	CBITPOS=$((EBX & 0x3f))
}

trap exit_from_int SIGINT

if [ `id -u` -ne 0 ]; then
	echo "Must be run as root!"
	exit 1
fi

while [ -n "$1" ]; do
	case "$1" in
		-sev-snp)	SEV_SNP="1"
				SEV_ES="1"
				SEV="1"
				;;
		-sev-es)	SEV_ES="1"
				SEV="1"
				;;
		-sev)		SEV="1"
				;;
		-hda) 		HDA="$2"
				shift
				;;
		-mem)  		MEM="$2"
				shift
				;;
		-smp)		SMP="$2"
				shift
				;;
		-bios)          UEFI_PATH="$2"
				shift
				;;
		-allow-debug)   ALLOW_DEBUG="1"
				;;
		-kernel)	KERNEL_FILE=$2
				shift
				;;
		-initrd)	INITRD_FILE=$2
				shift
				;;
		-cdrom) CDROM="$2"
				shift
				;;
		-vnc)   VNC="$2"
				shift
				;;
		*) 		usage
				;;
	esac

	shift
done

TMP="$EXEC_PATH/bin/qemu-system-x86_64"
QEMU_EXE="$(readlink -e $TMP)"
[ -z "$QEMU_EXE" ] && {
	echo "Can't locate qemu executable [$TMP]"
	usage
}

[ -n "$HDA" ] && {
	TMP="$HDA"
	HDA="$(readlink -e $TMP)"
	[ -z "$HDA" ] && {
		echo "Can't locate guest image file [$TMP]"
		usage
	}

	GUEST_NAME="$(basename $TMP | sed -re 's|\.[^\.]+$||')"
}

TMP="$UEFI_PATH/OVMF_CODE.fd"
UEFI_CODE="$(readlink -e $TMP)"
[ -z "$UEFI_CODE" ] && {
	echo "Can't locate UEFI code file [$TMP]"
	usage
}

[ -e "./$GUEST_NAME.fd" ] || {
	TMP="$UEFI_PATH/OVMF_VARS.fd"
	UEFI_VARS="$(readlink -e $TMP)"
	[ -z "$UEFI_VARS" ] && {
		echo "Can't locate UEFI variable file [$TMP]"
		usage
	}

	run_cmd "cp $UEFI_VARS ./$GUEST_NAME.fd"
}
UEFI_VARS="$(readlink -e ./$GUEST_NAME.fd)"

if [ "$ALLOW_DEBUG" = "1" ]; then
	# This will dump all the VMCB on VM exit
	echo 1 > /sys/module/kvm_amd/parameters/dump_all_vmcbs

	# Enable some KVM tracing to the debug
	#echo kvm: >/sys/kernel/debug/tracing/set_event
	#echo kvm:* >/sys/kernel/debug/tracing/set_event
	#echo kvm:kvm_page_fault >/sys/kernel/debug/tracing/set_event
	#echo >/sys/kernel/debug/tracing/set_event
	#echo > /sys/kernel/debug/tracing/trace
	#echo 1 > /sys/kernel/debug/tracing/tracing_on
fi

# we add all the qemu command line options into a file
QEMU_CMDLINE=/tmp/cmdline.$$
rm -rf $QEMU_CMDLINE

add_opts "$QEMU_EXE"

# Basic virtual machine property
add_opts "-enable-kvm -cpu EPYC-v4 -machine q35"

# add number of VCPUs
[ -n "${SMP}" ] && add_opts "-smp ${SMP},maxcpus=64"

# define guest memory
add_opts "-m ${MEM}M"

# don't reboot for SEV-ES guest
add_opts "-no-reboot"

# The OVMF binary, including the non-volatile variable store, appears as a
# "normal" qemu drive on the host side, and it is exposed to the guest as a
# persistent flash device.
add_opts "-drive if=pflash,format=raw,unit=0,file=${UEFI_CODE},readonly=on"

# add network support and fwd port 22 to 8000
# echo "guest port 22 is fwd to host 8000..."
add_opts "-netdev user,id=vmnic,hostfwd=tcp:127.0.0.1:2222-:22,hostfwd=tcp:127.0.0.1:8080-:8080"
add_opts " -device virtio-net-pci,disable-legacy=on,iommu_platform=true,netdev=vmnic,romfile="

# If harddisk file is specified then add the HDD drive
if [ -n "${HDA}" ]; then
	if [ "$USE_VIRTIO" = "1" ]; then
		if [[ ${HDA} = *"qcow2" ]]; then
			add_opts "-drive file=${HDA},if=none,id=disk0,format=qcow2"
		else
			add_opts "-drive file=${HDA},if=none,id=disk0,format=raw"
		fi
		add_opts "-device virtio-scsi-pci,id=scsi0,disable-legacy=on,iommu_platform=true"
		add_opts "-device scsi-hd,drive=disk0"
	else
		if [[ ${HDA} = *"qcow2" ]]; then
			add_opts "-drive file=${HDA},format=qcow2"
		else
			add_opts "-drive file=${HDA},format=raw"
		fi
	fi
fi


# If this is SEV guest then add the encryption device objects to enable support
if [ ${SEV} = "1" ]; then
	add_opts "-machine memory-encryption=sev0,vmport=off" 
	get_cbitpos

	if [ "${SEV_SNP}" = 1 ]; then
		POLICY=$((0x30000)) # "Base Policy" that allows SMT. See 4.3 in sev snp abi for other options
		if [ "${ALLOW_DEBUG}" = "1" ]; then
			POLICY=$(($POLICY | (0x1 << 19) )) #set bit 19, to enable debug api in plicy
		fi
		SEV_POLICY=$(printf "policy=%#x" $POLICY)
		echo "SEV_POLICY is ${SEV_POLICY}"

		add_opts "-object sev-snp-guest,id=sev0,${SEV_POLICY},cbitpos=${CBITPOS},reduced-phys-bits=1"
	else
		if [ "${ALLOW_DEBUG}" = "1" -o "${SEV_ES}" = 1 ]; then
			POLICY=$((0x01))
			[ "${ALLOW_DEBUG}" = "1" ] && POLICY=$((POLICY & ~0x01))
			[ "${SEV_ES}" = "1" ] && POLICY=$((POLICY | 0x04))
			SEV_POLICY=$(printf ",policy=%#x" $POLICY)
		fi
		add_opts "-object sev-guest,id=sev0${SEV_POLICY},cbitpos=${CBITPOS},reduced-phys-bits=1"
	fi
fi

# if -kernel arg is specified then use the kernel provided in command line for boot
if [ "${KERNEL_FILE}" != "" ]; then
	add_opts "-kernel $KERNEL_FILE"
	add_opts "-append \"console=hvc0 root=/dev/sda2\""
	[ ! -z ${INITRD_FILE} ] && add_opts "-initrd ${INITRD_FILE}"
fi

add_opts "-chardev stdio,id=mux,mux=on,logfile=/tmp/vm_log_$(date +"%FT%H%M").log \
             -device virtio-serial,romfile= \
             -device virtconsole,chardev=mux -monitor chardev:mux \
             -serial chardev:mux -nographic"

# if console is serial then disable graphical interface
if [ "${CONSOLE}" = "serial" ]; then
	add_opts "-nographic"
else
	add_opts "-vga ${CONSOLE}"
fi

if [ "${CDROM}" != "" ]; then
	add_opts "-cdrom ${CDROM}"
fi


if [ "${VNC}" != "" ]; then
	add_opts "-vnc ${VNC}"
fi


# start monitor on pty and named socket 'monitor'
add_opts "-monitor pty -monitor unix:monitor,server,nowait"
# add_opts " -nographic "
# this allows qemu-affinity to pin QEMU's threads
add_opts "-name sev-step-vm,debug-threads=on"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
SHARE_DIR=$SCRIPT_DIR/../

add_opts " -virtfs local,path=$SHARE_DIR,mount_tag=host0,security_model=none,id=host0"

# log the console  output in stdout.log
QEMU_CONSOLE_LOG=`pwd`/stdout.log

# save the command line args into log file
# cat $QEMU_CMDLINE | tee ${QEMU_CONSOLE_LOG}
echo | tee -a ${QEMU_CONSOLE_LOG}





echo "never" | sudo tee /sys/kernel/mm/transparent_hugepage/enabled



echo "Launching VM ..."
echo "  $QEMU_CMDLINE"

QEMU_CMD_LINE="taskset -c 3 $QEMU_CMD_LINE"
bash ${QEMU_CMDLINE} 2>&1 | tee -a ${QEMU_CONSOLE_LOG}
rm -rf ${QEMU_CMDLINE}
