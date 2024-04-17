#!/bin/bash
HERE=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd $HERE
source $HERE/../../../venv/bin/activate

pgrep qemu | xargs -I{} sudo kill -9 {}
pgrep my_custom_stuff | xargs -I{} sudo kill -9 {}
pgrep ssh_attack.py | xargs -I{} sudo kill -9 {}

bash -c "python -u $HERE/ssh_attack.py"


