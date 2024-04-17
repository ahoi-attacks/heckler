#!/bin/bash
HERE=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd $HERE
source $HERE/../../../venv/bin/activate

pgrep qemu | xargs -I{} sudo kill -9 {}

bash -c "python -u $HERE/sudo_attack.py"


