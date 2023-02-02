#!/bin/bash

# Exit when any command fails
set -e
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
export PLATFORM_REPO_PATHS=/scratch/unsafe/Xilinx/platforms/xilinx_u50_gen3x16_xdma_5_202210_1/
#source /opt/xilinx/xrt/setup.sh
freq=200

# Make sure everything is up to date
make TARGET=hw FREQ=${freq} PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 -f ../Makefile build_dir.hw.xilinx_u50_gen3x16_xdma_5_202210_1/krnl_incr.xclbin -j$(nproc) 
#make TARGET=hw FREQ=${freq} PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 -f ../Makefile all -j$(nproc) 


