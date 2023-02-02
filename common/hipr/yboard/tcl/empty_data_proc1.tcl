set cell_name data_proc1
open_checkpoint ./checkpoint/hw_bb_locked.dcp
update_design -black_box -cells level0_i/ulp/increment_1/mono_inst/${cell_name}_inst
lock_design -level routing
write_checkpoint -force ./checkpoint/hw_bb_empty.dcp

