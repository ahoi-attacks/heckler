PWD=$$(pwd)
SCRIPT_DIR=$(shell cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
venv_dir = ../venv
env_file = ../sev-step/environment.env
SHELL := /bin/bash

all: build

venv: $(venv_dir)/touchfile

$(venv_dir)/touchfile: requirements.txt
	test -d $(venv_dir) || python -m venv $(venv_dir)
	. $(venv_dir)/bin/activate; pip install -r requirements.txt
	touch $(venv_dir)/touchfile

build: venv
	. $(venv_dir)/bin/activate; pip install -r requirements.txt
	. $(venv_dir)/bin/activate; pip install -e .
	echo "/dev/kvm needs to be accessible by current user. Sudo required to change permissions"
	sudo chmod 666 /dev/kvm

test: venv
	. $(venv_dir)/bin/activate; pip install -r requirements.txt
	sh -c "'$(SCRIPT_DIR)/run_tests.sh'"

clean:
	rm -rf heckler.egg-info
	rm -rf heckler_attack.egg-info