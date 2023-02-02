

create_pblock coloringFB
resize_pblock [get_pblocks coloringFB] -add {SLICE_X21Y305:SLICE_X59Y359}
resize_pblock [get_pblocks coloringFB] -add {DSP48E2_X2Y116:DSP48E2_X7Y137}
resize_pblock [get_pblocks coloringFB] -add {RAMB18_X2Y122:RAMB18_X4Y143}
resize_pblock [get_pblocks coloringFB] -add {RAMB36_X2Y61:RAMB36_X4Y71}
resize_pblock [get_pblocks coloringFB] -add {URAM288_X0Y84:URAM288_X0Y95}
set_property SNAPPING_MODE ON [get_pblocks coloringFB]
set_property IS_SOFT TRUE [get_pblocks coloringFB]
add_cells_to_pblock [get_pblocks coloringFB] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/coloringFB_inst]] -quiet


create_pblock projection
resize_pblock [get_pblocks projection] -add {SLICE_X13Y305:SLICE_X18Y359}
resize_pblock [get_pblocks projection] -add {DSP48E2_X1Y116:DSP48E2_X1Y137}
resize_pblock [get_pblocks projection] -add {RAMB18_X1Y122:RAMB18_X1Y143}
resize_pblock [get_pblocks projection] -add {RAMB36_X1Y61:RAMB36_X1Y71}
set_property SNAPPING_MODE ON [get_pblocks projection]
set_property IS_SOFT TRUE [get_pblocks projection]
add_cells_to_pblock [get_pblocks projection] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/projection_inst]] -quiet


create_pblock rasterization1
resize_pblock [get_pblocks rasterization1] -add {SLICE_X15Y365:SLICE_X20Y419}
resize_pblock [get_pblocks rasterization1] -add {DSP48E2_X1Y140:DSP48E2_X1Y161}
resize_pblock [get_pblocks rasterization1] -add {RAMB18_X1Y146:RAMB18_X1Y167}
resize_pblock [get_pblocks rasterization1] -add {RAMB36_X1Y73:RAMB36_X1Y83}
set_property SNAPPING_MODE ON [get_pblocks rasterization1]
set_property IS_SOFT TRUE [get_pblocks rasterization1]
add_cells_to_pblock [get_pblocks rasterization1] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/rasterization1_inst]] -quiet


create_pblock rasterization2
resize_pblock [get_pblocks rasterization2] -add {SLICE_X1Y365:SLICE_X10Y419}
resize_pblock [get_pblocks rasterization2] -add {DSP48E2_X0Y140:DSP48E2_X0Y161}
resize_pblock [get_pblocks rasterization2] -add {RAMB18_X0Y146:RAMB18_X0Y167}
resize_pblock [get_pblocks rasterization2] -add {RAMB36_X0Y73:RAMB36_X0Y83}
set_property SNAPPING_MODE ON [get_pblocks rasterization2]
set_property IS_SOFT TRUE [get_pblocks rasterization2]
add_cells_to_pblock [get_pblocks rasterization2] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/rasterization2_inst]] -quiet


create_pblock read_in
resize_pblock [get_pblocks read_in] -add {SLICE_X4Y305:SLICE_X10Y359}
resize_pblock [get_pblocks read_in] -add {RAMB18_X0Y122:RAMB18_X0Y143}
resize_pblock [get_pblocks read_in] -add {RAMB36_X0Y61:RAMB36_X0Y71}
set_property SNAPPING_MODE ON [get_pblocks read_in]
set_property IS_SOFT TRUE [get_pblocks read_in]
add_cells_to_pblock [get_pblocks read_in] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/read_in_inst]] -quiet


create_pblock zculling
resize_pblock [get_pblocks zculling] -add {SLICE_X37Y365:SLICE_X59Y419}
resize_pblock [get_pblocks zculling] -add {DSP48E2_X5Y140:DSP48E2_X7Y161}
resize_pblock [get_pblocks zculling] -add {RAMB18_X3Y146:RAMB18_X4Y167}
resize_pblock [get_pblocks zculling] -add {RAMB36_X3Y73:RAMB36_X4Y83}
resize_pblock [get_pblocks zculling] -add {URAM288_X0Y100:URAM288_X0Y111}
set_property SNAPPING_MODE ON [get_pblocks zculling]
set_property IS_SOFT TRUE [get_pblocks zculling]
add_cells_to_pblock [get_pblocks zculling] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/data_proc1_inst/zculling_inst]] -quiet
