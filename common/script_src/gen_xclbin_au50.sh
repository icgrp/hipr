#!/bin/bash -e
# source /opt/Xilinx/Vivado/2018.2/Settings64.sh 
bitstream=$1
xmlfile=$2
xclbin=$3
workspace=$4
freq=$5

xclbinutil --add-section BITSTREAM:RAW:${bitstream} \
           --force  \
           --target hw  \
           --key-value SYS:dfx_enable:true  \
           --add-section :JSON:../F000_increment_${freq}/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/int/krnl_incr.link.rtd  \
           --append-section :JSON:../F000_increment_${freq}/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/int/appendSection.rtd  \
           --add-section CLOCK_FREQ_TOPOLOGY:JSON:../F000_increment_${freq}/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/int/krnl_incr.link_xml.rtd  \
           --add-section BUILD_METADATA:JSON:../F000_increment_${freq}/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/int/krnl_incr.link_build.rtd  \
           --add-section EMBEDDED_METADATA:RAW:${xmlfile}  \
           --add-section SYSTEM_METADATA:RAW:../F000_increment_${freq}/increment/au50/_x.hw.xilinx_u50_gen3x16_xdma_5_202210_1/link/int/systemDiagramModelSlrBaseAddress.json  \
           --key-value SYS:PlatformVBNV:xilinx_u50_gen3x16_xdma_5_202210_1  \
           --output ./${xclbin}
