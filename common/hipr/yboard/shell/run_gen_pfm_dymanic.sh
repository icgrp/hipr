#!/bin/bash -e

impl_dir=$1
app=$2

cd ${impl_dir}
vivado -mode batch -source  gen_pfm_dynamic_${app}.tcl
cd -
cp ${impl_dir}/pfm_dynamic_${app}.dcp ./checkpoint/pfm_dynamic.dcp
