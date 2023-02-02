#!/bin/bash -e

tcl_name=$1
tcl_argv=$2


start=$(date +%s)
vivado -mode batch -source  ${tcl_name} -log vivado_${tcl_name}.log -tclargs $2  
end=$(date +%s)    
runtime=$((${end}-${start}))
echo "runtime: $runtime" > $1.runtime
