#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "This script pins the victims qemu vcpu thread to the given cpu core and also fixates the clock frequency on that core."
	echo "If your system is configured to run at a fixed frequency and does thus not support cpufreq-set, pass \"NULL\" for the frequency"
	echo "It is assumed that only one instance of qemu-system-x86_64 is running"
	echo "Usage: post-startup-script <cpu to pin to> <{target cpu freq,NULL}>"
	echo "On most machines the lowest frequency in /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies works well"
	exit
fi


#
# Check kvm module config
#
if [[ "$(cat /sys/module/kvm/parameters/tdp_mmu)" == "Y" ]]; then
    echo 'Error: tdp_mmu param of kvm module must be set to "N"'
    exit 1
fi

#kvm_amd sev-snp=1 sev=1 sev-es=1
if [[ "$(cat /sys/module/kvm_amd/parameters/sev_snp)" != "Y" ]]; then
    echo 'Error: sev_snp param of kvm_amd module must be set to "Y"'
    exit 1
fi

if [[ "$(cat /sys/module/kvm_amd/parameters/sev)" != "Y" ]]; then
    echo 'Error: sev param of kvm_amd module must be set to "Y"'
    exit 1
fi

if [[ "$(cat /sys/module/kvm_amd/parameters/sev_es)" != "Y" ]]; then
    echo 'Error: sev_es param of kvm_amd module must be set to "Y"'
    exit 1
fi

#
# Disable some magic prefetches by msr writes
#

sudo modprobe msr
sudo wrmsr -a 0xc001102b  0x00000000200fcc16
sudo wrmsr -a  0xc0011022 0xc000000002512000
echo "Disabled prefetch"

#
# Pin frequency and vm thread
#

CPU=$1
FREQ=$2
sudo qemu-affinity $(pidof qemu-system-x86_64 ) -k $CPU || exit 1
echo "Pinned vcpu thread to CPU $CPU"

if [[ ${FREQ} == "NULL" ]]; then
	echo "FREQ=NULL provied, assuming cpu frequency is already fixed"
else
	sudo cpufreq-set -r -c $CPU -g performance -u "${FREQ}" -d "${FREQ}" || exit 1
	echo "Set frequency"
fi
