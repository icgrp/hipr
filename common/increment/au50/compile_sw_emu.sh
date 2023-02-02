#!/bin/bash
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
source /opt/xilinx/xrt/setup.sh
make -f ../Makefile TARGET=sw_emu PLATFORM=xilinx_u50_gen3x16_xdma_5_202210_1 all
