#!/bin/bash

pgrep qemu | xargs -I{} sudo kill -9 {}