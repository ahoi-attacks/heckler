HERE_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

ENV_DIR=$HERE_DIR/../venv
source $ENV_DIR/bin/activate
source $HERE_DIR/../sev-step/environment.env

sudo chmod 777 /dev/kvm

cd $HERE_DIR
pip install -e .

