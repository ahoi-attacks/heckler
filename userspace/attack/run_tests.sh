#!/bin/bash
HERE=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $HERE
source $HERE/../venv/bin/activate
bash -c "pytest"


