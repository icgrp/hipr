#!/bin/bash
# Xilinx_dir
# sdk_dir=
# export PLATFORM_REPO_PATHS=
# export PLATFORM=

make -f ../Makefile TARGET=hw PLATFORM=${PLATFORM}  EDGE_COMMON_SW=${sdk_dir} all -j$(nproc)
