#!/bin/bash
here_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
proj_dir="$here_dir/.."
set -euo pipefail

name=beancd/ahoi-heckler:latest

docker build -t $name .

