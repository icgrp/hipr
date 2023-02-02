#!/bin/bash

# Exit when any command fails
set -e
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
source /opt/xilinx/xrt/setup.sh

export PLATFORM_REPO_PATHS=/scratch/unsafe/Xilinx/platforms/xilinx_u50_gen3x16_xdma_5_202210_1

# Make sure everything is up to date
freq=200
make ACCLE=MONO app.exe 
#make ACCLE=MONO FREQ=${freq} TARGET=hw PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 build_dir.hw.xilinx_u50_gen3x16_xdma_5_202210_1/krnl_incr.xclbin -j$(nproc) 
