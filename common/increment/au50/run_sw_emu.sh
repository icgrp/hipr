#!/bin/bash
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
source /opt/xilinx/xrt/setup.sh

XCL_EMULATION_MODE=sw_emu ./streaming_free_running_k2k ./build_dir.sw_emu.xilinx_u50_gen3x16_xdma_5_202210_1/krnl_incr.xclbin
