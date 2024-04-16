#!/bin/bash
here_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
proj_dir="$here_dir/.."
set -euo pipefail

name=heckler-docker:latest

docker build -t $name .

