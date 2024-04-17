# SEV STEP Library

This repo contains the userspace library of the SEV STEP framework. To use it, you also need to run
the corresponding kernel. If you have not cloned this repo as a submodule of the `sev-step-meta` repo already, check out the manual there to setup the correct environment.

There also is an [experimental version](https://github.com/sev-step/sev-step-rust-userland) of this library that tries to make single-stepping easier by providing an API with high level abstractions. 

## Features

- **Interactive Page Granular Tracking:** Remove access rights from VM pages and observe the corresponding page faults. Upon a page fault, the VM is halted and the user space code is notified via a shared memory channel. The VM remains halted until the user space code finished processing the event. This allows the user space code to dynamically react to events, enabling is to implement complex attack logic.
- **Interactive Single Stepping:** Interrupt the VM after every instruction. Like for page granular tracking the VM is paused and the user space code can react to events interactively.
- **Interactive Cache Attacks:** Perform cache attacks from the Host against code running in the VM.
- **Interactive Read Register Values:** Use the SEV Debug API to access the VM's encrypted register values

## Component Overview

This repo has three main components

- `sev-step-lib`  : The actual library with the features described above
- `end2end-tests` : This contains a test suite, to verify that the different components of the library are working correctly on your system. It has two components: a client and a server. The server is intended to be executed inside as SEV (SNP) VM while the client is intended to run on the host system. The client uses the server to setup scenarios. For example it can request two pages to be written to in an alternating manner, which can then be used to verify the pagefault tracking functionality. The different tests are also a great starting place to explore the functionality of the library.
- `example-apps`: Code for the following tools
  - `kaslr-attack`: Break KASLR by injecting an NMI and observing pagefaults as presented in [1].
  - `nemesis_main.c`: Binary for the nemesis experiments from the paper.
  - `paper_experiments/` Binary for cache attack evaluation in Paper Section 5.3 and single step evaluation in Paper Section 5.1.

## Build

1. Issue `make dependencies` to pull and build any external dependencies used by this code. You only need to do this once.
2. Issue `make` to build all binaries and the sev step library

The SEV-Step library requires access to the header file for the IOCTL API exposed by the kernel part. If you build the library as part of the meta repository, this is already pre-configured. Otherwise, you need to edit the `environment.env` file to point the build system to the kernel's include directory.

After building, the binaries are in `sev-step-userland/build/binaries` (except for the server part of the end2end test, which is in `build/rust/target/release/`) and the library is in `sev-step-userland/build/libs`.

## Use

It is assumed, that you have followed the instructions in the `sev-step-meta` repo to install the correct kernel and configured a SEV SNP VM.

 Issue `source ./environment.env` to setup up the `LD_LIBRARY_PATH` variable to find the locally build libraries.

 Most binaries need to run with root privileges (to do virtual to physical memory translation). In order to use the tweaked `LD_LIBRARY_PATH` variable with sudo, you need to use `sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH <some program>`.

### System Setup

For single stepping to work, your VM must be pinned to a dedicated core that is not used by any other code and is operating with a fixed
frequency. After setting up core pinning and frequency fixing in the next sections, the final sections introduces a handy wrapper script.

#### Kernel config options

In this section you will tweak some kernel options.
Under Ubuntu, this can be done by editing
`GRUB_CMDLINE_LINUX_DEFAULT` in `/etc/default/grub`, issuing `update-grub2` and finally rebooting the system.
Use `cat /proc/cmdline` to check the currently configured options.

- For cpu core pinning, we need the following options, where `cpu` is a comma separated list of cores to isolate. If you system has Hyperthreading, you need to isolate both logical cores.

    `isolcpus=<cpu> nohz_full<cpu> rcu_nocbs=<cpu> rcu_nocb_poll=<cpu>`

- For frequency pinning, we need the following options:
    `processor.max_cstate=0 amd_cpufreq=enable`

- For pagefault tracking, to work, you need the following option
 `transparent_hugepage=never`

- In addition, we also set the following options:
    `log_buf_len=1024MB nosmap nosmep nox2apic dis_ucode_dlr msr.allow_writes=on nmi_watchdog=0 iomem=relaxed no_timer_check`

#### Core Pinning

The aforementioned kernel command line options ensure that the core is not used by any other part of the system. To actually pin the VM to a core, you need to install [qemu-affinity](https://github.com/zegelin/qemu-affinity).

#### Frequency Fixing

There are two options for frequency pinning. Some systems allow to disable frequency scaling in the BIOS, such that all cores
run with a fixed frequency. The other alternative is to use Linux's cpu frequency scaling driver to fixate the frequency.

In the first case, you can skip the rest of this section.

In the latter case, you can query `/sys/devices/system/cpu/<cpu core>/cpufreq/scaling_available_frequencies` to get a list of supported
target frequencies and use `cpufreq-set -r -c <cpu core>-g performance -u <freq> -d <freq>` to set the frequency. Afterwards use
`/sys/devices/system/cpu/<cpu core>/cpufreq/scaling_cur_freq` to query the current frequency to ensure that the pinning part actually worked.

#### Putting it all together

- If you run without frequency scaling, issue `./post-startup-script.sh <isolated cpu> NULL` to prepare the VM for single stepping.
- If you run with frequency scaling AND have determined that `cpufreq-set` reliably pins some core the the frequency `freq`, use
`./post-startup-script.sh <isolated cpu> <freq>` to prepare the VM for single stepping.

## End2End Tests

Contains a server binary for the VM an a client binary for the host system.
Copy the server binary `./build/rust/target/release/vm-server` to the VM and **start it with root privileges**.
Make sure you have ran the `./post-startup-script.sh` from the previous step!
Afterwards start the client binary on the host with `sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH ./build/binaries/end2end-tests-hv-client`.

The client will run a several tests  tests to ensure that the communication channel, page tracking, single stepping and cache attacks work properly.
If you want to skip some test, you have to edit `end2end-tests/host-client/main.c` and set the test's `.skip` attribute to `true`.

For the single stepping tests (e.g. `test_single_step_simple_long`) to work
you have to tweak the timer value at the start of the main function until you don't get any multisteps but still make some progress with single steps.

The end2end tests provide a great way to explore the functionality of the sev step library.

## Running the Experiments from the paper

This section requires that you have successfully performed
Once you have performed the other setup steps and successfully ran the
end-to-end tests.
The setup is the same as for the [end-to-end tests](#end2end-tests) :

1. Start the `vm-server` binary inside the VM
2. Run the `./post-startup-script.sh` script with the arguments matchign your setup

### Evaluation in Section 5.1 and Section 5.2

Run `sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH ./build/binaries/paper-experiments <experiment name> <apic timer value>`
where `<experiment name>` is one of the following

- single_step_no_tlb_no_a_bit
- single_step_no_tlb_reset_a_bit
- single_step_flush_tlb_no_a_bit
- single_step_flush_tlb_reset_a_bit
- prime_probe_lfence
- prime_probe_no_lfence

The experiments will print data to stdout.
The two prime and probe experiments will also store the cache trace to `./build/logs/`

### Nemesis Case Study

Run `run-nemesis-eval.sh <repetitions> <path> <apic timer value> <experiment number>`
where

- `repetitions` is the number of times the experiment is repeated
- `<path>` is the path to the folder where the data should be stored
- `<experiment number>` selects one of the following instructions benchmarks:

| experiment number  | benchmarked instruction |
|----|---|
|  0 | nop  |
|  1 |  add |
|  2 |  rdrand64 |
|  3 |  rdrand32 |
|  4 |  div16 |
|  5 |  div32 |
|  6 |  div64-0 |
|  7 |  div64-1 |
|  8 |  div64-2 |
|  9 |  div64-3 |
| 10 |  div64-4 |
| 11 |  mul |
| 12 |  rcl |
| 13 |  lar |

## References / Citations

[1] <https://arxiv.org/pdf/2105.13824.pdf>
