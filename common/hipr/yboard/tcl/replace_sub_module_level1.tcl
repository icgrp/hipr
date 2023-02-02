set kl_name increment_bb

add_files ./checkpoint/pfm_dynamic_bb.dcp
add_files ./checkpoint/${kl_name}.dcp
set_property SCOPED_TO_CELLS { increment_1 } [get_files ./checkpoint/${kl_name}.dcp]
link_design -mode default -part xcu50-fsvh2104-2-e -top ulp
write_checkpoint -force ./checkpoint/pfm_dynamic_new_bb.dcp


