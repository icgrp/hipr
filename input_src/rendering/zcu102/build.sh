#!/bin/bash

# Exit when any command fails
set -e
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh

# Make sure everything is up to date
make TARGET=hw PLATFORM=xilinx_zcu102_base_dfx_202210_1 EDGE_COMMON_SW=/scratch/unsafe/Xilinx/xilinx-zynqmp-common-v2022.1/sdk/ ./app.exe 


