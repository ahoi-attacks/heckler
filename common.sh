#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

run_cmd()
{
	echo "$*"

	eval "$*" || {
		echo "ERROR: $*"
		exit 1
	}
}

build_kernel()
{
	set -x
	kernel_type="${1:-}"
	shift
	mkdir -p linux
	pushd linux >/dev/null

	if [ ! -d guest ]; then
		run_cmd git clone ${KERNEL_GIT_URL} guest
		pushd guest >/dev/null
		run_cmd git checkout "${KERNEL_GUEST_BRANCH}"
		popd
	fi

	if [ ! -d host ]; then
		# use a copy of guest repo as the host repo
		run_cmd cp -r guest host
		pushd host >/dev/null
		run_cmd git reset --hard
		run_cmd git checkout "${KERNEL_HOST_BRANCH}"
	fi

	for V in guest host; do
		# Check if only a "guest" or "host" or kernel build is requested
		if [ "$kernel_type" != "" ]; then
			if [ "$kernel_type" != "$V" ]; then
				continue
			fi
		fi

		rm -f linux-*-snp-${V}*

		VER="-snp-${V}"

		MAKE="make -C ${V} -j $(getconf _NPROCESSORS_ONLN) LOCALVERSION="

		run_cmd $MAKE distclean

		pushd ${V} >/dev/null
	
			COMMIT=$(git log --format="%h" -1 HEAD)

			run_cmd "cp /boot/config-$(uname -r) .config"

			run_cmd ./scripts/config --set-str LOCALVERSION "$VER-$COMMIT"
			run_cmd ./scripts/config --disable LOCALVERSION_AUTO
			run_cmd ./scripts/config --enable  EXPERT
			run_cmd ./scripts/config --enable  DEBUG_INFO
			run_cmd ./scripts/config --enable  DEBUG_INFO_REDUCED
			run_cmd ./scripts/config --enable  AMD_MEM_ENCRYPT
			run_cmd ./scripts/config --disable AMD_MEM_ENCRYPT_ACTIVE_BY_DEFAULT
			run_cmd ./scripts/config --module KVM
			run_cmd ./scripts/config --module KVM_AMD
			run_cmd ./scripts/config --enable KVM_AMD_SEV
			run_cmd ./scripts/config --module  CRYPTO_DEV_CCP_DD
			run_cmd ./scripts/config --disable SYSTEM_TRUSTED_KEYS
			run_cmd ./scripts/config --disable SYSTEM_REVOCATION_KEYS
			run_cmd ./scripts/config --disable MODULE_SIG_KEY
			run_cmd ./scripts/config --module  SEV_GUEST
			run_cmd ./scripts/config --disable IOMMU_DEFAULT_PASSTHROUGH
			run_cmd ./scripts/config --disable PREEMPT_COUNT
			run_cmd ./scripts/config --disable PREEMPTION
			run_cmd ./scripts/config --disable PREEMPT_DYNAMIC
			run_cmd ./scripts/config --disable DEBUG_PREEMPT
			run_cmd ./scripts/config --enable  CGROUP_MISC
			run_cmd ./scripts/config --module  X86_CPUID
			run_cmd ./scripts/config --disable UBSAN
			run_cmd ./scripts/config --disable MODULE_COMPRESS_ZSTD
			
			

			if [ "$V" == "guest" ]; then
				# guest support
				cp -rf $SCRIPT_DIR/misc/sev-guest-kernel-config .config							
			fi


			run_cmd echo $COMMIT >../../source-commit.kernel.$V
		popd >/dev/null

		
		yes "" | $MAKE olddefconfig || true

		if [ "$V" == "guest" ]; then
		    # for guest we just need bzImage, the rest we take from distro
			MAKE="$MAKE bzImage"
		fi

		# Build 
		run_cmd $MAKE 
		

		if [ "$ID" = "debian" ] || [ "$ID_LIKE" = "debian" ]; then
			run_cmd $MAKE bindeb-pkg
		else
			run_cmd $MAKE "RPMOPTS='--define \"_rpmdir .\"'" binrpm-pkg
			run_cmd mv ${V}/x86_64/*.rpm .
		fi
	done

	popd
}

build_install_ovmf_edk2()
{
	DEST="$1"

	if [ -d edk2 ]; then
		echo "./edk2 directory already exist. do not re-init"
		popd >/dev/null
	else
		echo "cloning repository ${OVMF_GIT_URL}"
		run_cmd git clone --single-branch -b ${OVMF_BRANCH} ${OVMF_GIT_URL} edk2
		pushd edk2 >/dev/null
		run_cmd git submodule update --init --recursive
		popd >/dev/null
	fi

	GCC_VERSION=$(gcc -v 2>&1 | tail -1 | awk '{print $3}')
	GCC_MAJOR=$(echo $GCC_VERSION | awk -F . '{print $1}')
	GCC_MINOR=$(echo $GCC_VERSION | awk -F . '{print $2}')
	if [ "$GCC_MAJOR" == "4" ]; then
		GCCVERS="GCC${GCC_MAJOR}${GCC_MINOR}"
	else
		GCCVERS="GCC5"
	fi

	BUILD_CMD="nice build -q --cmd-len=64436 -DDEBUG_ON_SERIAL_PORT=TRUE -n $(getconf _NPROCESSORS_ONLN) ${GCCVERS:+-t $GCCVERS} -a X64 -p OvmfPkg/OvmfPkgX64.dsc"

    pushd edk2 >/dev/null
        run_cmd make -C BaseTools
        . ./edksetup.sh --reconfig
        run_cmd $BUILD_CMD

        mkdir -p $DEST
        run_cmd cp -f Build/OvmfX64/DEBUG_$GCCVERS/FV/OVMF_CODE.fd $DEST
        run_cmd cp -f Build/OvmfX64/DEBUG_$GCCVERS/FV/OVMF_VARS.fd $DEST
	popd >/dev/null
}


# XXX: We are using older edk2 version, use build_install_ovmf_edk2 instead
build_install_ovmf()
{
	DEST="$1"

	GCC_VERSION=$(gcc -v 2>&1 | tail -1 | awk '{print $3}')
	GCC_MAJOR=$(echo $GCC_VERSION | awk -F . '{print $1}')
	GCC_MINOR=$(echo $GCC_VERSION | awk -F . '{print $2}')
	if [ "$GCC_MAJOR" == "4" ]; then
		GCCVERS="GCC${GCC_MAJOR}${GCC_MINOR}"
	else
		GCCVERS="GCC5"
	fi

	BUILD_CMD="nice build -q --cmd-len=64436 -DDEBUG_ON_SERIAL_PORT=TRUE -n $(getconf _NPROCESSORS_ONLN) ${GCCVERS:+-t $GCCVERS} -a X64 -p OvmfPkg/OvmfPkgX64.dsc"


	# initialize git repo, or update existing remote to currently configured one
#	if [ -d ovmf ]; then
#		pushd ovmf >/dev/null
#		if git remote get-url current 2>/dev/null; then
#			run_cmd git remote set-url current ${OVMF_GIT_URL}
#		else
#			run_cmd git remote add current ${OVMF_GIT_URL}
#		fi
#		popd >/dev/null
#	else
#		run_cmd git clone --single-branch -b ${OVMF_BRANCH} ${OVMF_GIT_URL} ovmf
#		pushd ovmf >/dev/null
#		run_cmd git remote add current ${OVMF_GIT_URL}
#		popd >/dev/null
#	fi

	pushd ovmf >/dev/null
		#run_cmd git fetch current
		#run_cmd git checkout ${OVMF_BRANCH}
		#run_cmd git submodule update --init --recursive
		run_cmd make -C BaseTools
		. ./edksetup.sh --reconfig
		run_cmd $BUILD_CMD

		mkdir -p $DEST
		run_cmd cp -f Build/OvmfX64/DEBUG_$GCCVERS/FV/OVMF_CODE.fd $DEST
		run_cmd cp -f Build/OvmfX64/DEBUG_$GCCVERS/FV/OVMF_VARS.fd $DEST

		COMMIT=$(git log --format="%h" -1 HEAD)
		run_cmd echo $COMMIT >../source-commit.ovmf
	popd >/dev/null
}

build_install_qemu()
{
	DEST="$1"

	if [ -d qemu ]; then
		echo "./qemu directory already exist. do not re-init"
		popd >/dev/null
	else
		echo "cloning repository ${QEMU_GIT_URL}"
		run_cmd git clone --single-branch -b ${QEMU_BRANCH} ${QEMU_GIT_URL} qemu
		pushd qemu >/dev/null
		run_cmd git submodule update --init --recursive
		popd >/dev/null
	fi

	MAKE="make -j $(getconf _NPROCESSORS_ONLN) LOCALVERSION="

	pushd qemu >/dev/null
		run_cmd CFLAGS=-Wno-error ./configure --target-list=x86_64-softmmu --prefix=$DEST --enable-slirp --enable-virtfs 
		run_cmd $MAKE
		run_cmd $MAKE install

		COMMIT=$(git log --format="%h" -1 HEAD)
		run_cmd echo $COMMIT >../source-commit.qemu
	popd >/dev/null
}
