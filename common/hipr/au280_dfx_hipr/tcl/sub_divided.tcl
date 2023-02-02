
open_checkpoint ./checkpoint/hw_bb_locked.dcp
pr_subdivide -cell level0_i/ulp -subcells {level0_i/ulp/increment_1/mono_inst/data_proc1_inst } ./checkpoint/pfm_dynamic_new_bb.dcp
write_checkpoint -force ./checkpoint/hw_bb_divided.dcp

