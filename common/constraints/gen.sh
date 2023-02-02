#!/bin/bash


for i in $(seq 100 10 400)
do
  #cp -rf backup/au50_200M au50_${i}M
  cp ../../workspace/F000_increment_${i}M/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/vivado/vpl/scripts/_vpl_post_init.tcl ./au50_${i}M/scripts/
  cp ../../workspace/F000_increment_${i}M/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/vivado/vpl/output/ulp_ooc_copy.xdc    ./au50_${i}M/xdc
  sed -i 's/$vivado_output_dir/"output"/g' ./au50_${i}M/scripts/_vpl_post_init.tcl

done
