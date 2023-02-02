#!/bin/bash
# Xilinx_dir
# sdk_dir=
# export PLATFORM_REPO_PATHS=
# export PLATFORM=
make -f ../Makefile TARGET=hw PLATFORM=xilinx_zcu102_base_dfx_202210_1 EDGE_COMMON_SW=/scratch/unsafe/Xilinx/xilinx-zynqmp-common-v2022.1/sdk streaming_free_running_k2k
