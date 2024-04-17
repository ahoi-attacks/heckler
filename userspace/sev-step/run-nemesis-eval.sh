#!/usr/bin/bash

#./post-startup-script.sh 0,1 NULL
#source ./environment.env

if [ "$#" -le 1  ]; then
	echo "./run.sh <num> <path> <apic timer> <instr num>"
	echo "or to run all divs:"
	echo "./run divs <apic timer>"
	echo "or run all except divs:"
	echo "./run all <apic timer>"
	echo "<instr num> :
	                 0 nop
	                 1 add
			 2 rdrand64
			 3 rdrand32
			 4 div16
			 5 div32
			 6 div64-0
			 7 div64-1
			 8 div64-2
			 9 div64-3
			 10 div64-4
			 11 mul64
			 12 rcl
			 13 lar "
	exit
fi

# note: test that all gpas of the slides can be resolved prior to running
if [[ "$1" == "divs" ]]; then
	div_paths=(div16 div32 div64-0 div64-1 div64-2 div64-3)
	for j in {4..9}
	do
		cur_div=${div_paths[$j-4]}
		if [ ! -d "$cur_div" ]; then
		    mkdir "$cur_div"
		fi
		for i in $(seq 1 300)
		do
			sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH ./build/binaries/nemesis-eval $2 $j
			if [ -e out.json ]; then
			    sudo mv out.json "$cur_div/out-$(date +%s).json"
			fi
			sleep 3
		done
	done
elif [[ "$1" == "all" ]]; then
        paths=(nop add rdrand64 rdrand32 mul64 rcl lar)
	num=(0 1 2 3 11 12 13)
	for j in {0..6}
	do
		cur=${paths[$j]}
		cur_num=${num[$j]}
		if [ ! -d "$cur" ]; then
		    mkdir "$cur"
		fi
		for i in $(seq 1 100)
		do
			sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH ./build/binaries/nemesis-eval $2 $cur_num
			if [ -e out.json ]; then
			    sudo mv out.json "$cur/out-$(date +%s).json"
			fi
			sleep 3
		done
	done
else
	for i in $(seq 1 $1)
	do
		sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH ./build/binaries/nemesis-eval $3 $4
		if [ -e out.json ]; then
		    sudo mv out.json "$2/out-$(date +%s).json"
		fi
		sleep 3
	done
fi
