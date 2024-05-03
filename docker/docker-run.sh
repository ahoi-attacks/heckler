#!/bin/bash
here_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
proj_dir="$here_dir/.."


name=beancd/ahoi-heckler:latest

# XXX: --mount: to compile host kernel inside container
# XXX: --priviledged: we need this to run snp guest inside docker

docker run --privileged  --name heckler -v "$proj_dir:/heckler" \
    --device /dev/net/tun:/dev/net/tun \
    --device /dev/kvm:/dev/kvm \
    --device /dev/sev:/dev/sev \
    --mount type=bind,source=/boot/config-$(uname -r),target=/boot/config-$(uname -r),readonly \
    -w /heckler  -it -d $name /bin/bash || true

docker start heckler || true

CMD=${1:-}
if [ -z "$CMD" ]; then
    docker attach heckler
else
    docker exec -it heckler $CMD
fi


