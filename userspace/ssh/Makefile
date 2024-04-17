PWD=$$(pwd)
SCRIPT_DIR=$(shell cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJ_ROOT=$(SCRIPT_DIR)

build: ssh_helper sshd

sshd:
	sh -c "'$(SCRIPT_DIR)/build_sshd.sh'"

ssh_helper:
	sh -c "'$(SCRIPT_DIR)/build_ssh_helper.sh'"

.PHONY:
clean:
	rm -rf build || true