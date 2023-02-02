#!/bin/bash

# Exit when any command fails
set -e
source /scratch/unsafe/Xilinx/Vitis/2022.1/settings64.sh
source /opt/xilinx/xrt/setup.sh

# Make sure everything is up to date
# make ACCEL=SEP all  -j8
make ACCEL=MONO all  -j8

# Run the application in HW emulation mode
# make ACCEL=SEP run
make ACCEL=MONO run

