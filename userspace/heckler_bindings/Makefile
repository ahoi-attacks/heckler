ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
venv_dir = $(ROOT_DIR)/../venv
sev_step_dir = $(ROOT_DIR)/../sev-step
env_file = $(sev_step_dir)/environment.env
sev_step_lib = $(sev_step_dir)/build/libs/libsevstep.so

SHELL := /bin/bash 


all: build

venv: $(venv_dir)/touchfile

$(venv_dir)/touchfile: requirements.txt
	test -d $(venv_dir) || python -m venv $(venv_dir)
	. $(venv_dir)/bin/activate; pip install -r requirements.txt
	touch $(venv_dir)/touchfile

build: venv sev-step
ifeq (,$(wildcard $(sev_step_lib)))
	$(error Build sev-step first by invoking make in ../sev-step)
endif
	. $(venv_dir)/bin/activate; pip install -e .
	echo "/dev/kvm needs to be accessible by current user. Sudo required to change permissions"
	sudo chmod 666 /dev/kvm


test: venv
	. $(venv_dir)/bin/activate; . $(env_file); python test/test.py

sev-step:
	cd $(sev_step_dir) && $(MAKE) 

clean:
	rm -r build/ || true
	rm -r heckler_bindings.egg-info || true
	rm -r heckler_bindings.cpython* || true