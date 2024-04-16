#!/bin/bash
here_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
proj_dir="$here_dir/.."


docker rm heckler
docker ps --all