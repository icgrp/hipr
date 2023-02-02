open_checkpoint ./checkpoint/overlay.dcp
update_design -cell level0_i/ulp/increment_1/mono_inst/data_proc1_inst  -black_box
lock_design -level routing
write_abstract_shell -force -cell level0_i/ulp/increment_1/mono_inst/data_proc1_inst  ./checkpoint/data_proc1.dcp
