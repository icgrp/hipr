#!/bin/bash

# Exit when any command fails
set -e
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
source /opt/xilinx/xrt/setup.sh

# Make sure everything is up to date
make TARGET=sw_emu PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 -f ../Makefile all -j$(nproc) 

# Run the application in HW emulation mode
make TARGET=sw_emu PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 -f ../Makefile run

