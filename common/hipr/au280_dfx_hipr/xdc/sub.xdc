

create_pblock coloringFB
resize_pblock [get_pblocks coloringFB] -add {SLICE_X19Y60:SLICE_X93Y119}
resize_pblock [get_pblocks coloringFB] -add {DSP48E2_X2Y18:DSP48E2_X11Y41}
resize_pblock [get_pblocks coloringFB] -add {RAMB18_X2Y24:RAMB18_X6Y47}
resize_pblock [get_pblocks coloringFB] -add {RAMB36_X2Y12:RAMB36_X6Y23}
resize_pblock [get_pblocks coloringFB] -add {URAM288_X0Y16:URAM288_X0Y31}
set_property SNAPPING_MODE ON [get_pblocks coloringFB]
set_property IS_SOFT TRUE [get_pblocks coloringFB]
add_cells_to_pblock [get_pblocks coloringFB] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/coloringFB_inst]] -quiet


create_pblock projection
resize_pblock [get_pblocks projection] -add {SLICE_X2Y120:SLICE_X16Y179}
resize_pblock [get_pblocks projection] -add {DSP48E2_X1Y42:DSP48E2_X1Y65}
resize_pblock [get_pblocks projection] -add {RAMB18_X0Y48:RAMB18_X1Y71}
resize_pblock [get_pblocks projection] -add {RAMB36_X0Y24:RAMB36_X1Y35}
set_property SNAPPING_MODE ON [get_pblocks projection]
set_property IS_SOFT TRUE [get_pblocks projection]
add_cells_to_pblock [get_pblocks projection] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/projection_inst]] -quiet


create_pblock rasterization1
resize_pblock [get_pblocks rasterization1] -add {SLICE_X25Y120:SLICE_X33Y179}
resize_pblock [get_pblocks rasterization1] -add {DSP48E2_X2Y42:DSP48E2_X3Y65}
resize_pblock [get_pblocks rasterization1] -add {RAMB18_X2Y48:RAMB18_X2Y71}
resize_pblock [get_pblocks rasterization1] -add {RAMB36_X2Y24:RAMB36_X2Y35}
set_property SNAPPING_MODE ON [get_pblocks rasterization1]
set_property IS_SOFT TRUE [get_pblocks rasterization1]
add_cells_to_pblock [get_pblocks rasterization1] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/rasterization1_inst]] -quiet


create_pblock rasterization2
resize_pblock [get_pblocks rasterization2] -add {SLICE_X42Y120:SLICE_X55Y179}
resize_pblock [get_pblocks rasterization2] -add {DSP48E2_X6Y42:DSP48E2_X6Y65}
resize_pblock [get_pblocks rasterization2] -add {RAMB18_X3Y48:RAMB18_X3Y71}
resize_pblock [get_pblocks rasterization2] -add {RAMB36_X3Y24:RAMB36_X3Y35}
set_property SNAPPING_MODE ON [get_pblocks rasterization2]
set_property IS_SOFT TRUE [get_pblocks rasterization2]
add_cells_to_pblock [get_pblocks rasterization2] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/rasterization2_inst]] -quiet


create_pblock read_in
resize_pblock [get_pblocks read_in] -add {SLICE_X6Y60:SLICE_X12Y119}
resize_pblock [get_pblocks read_in] -add {RAMB18_X0Y24:RAMB18_X0Y47}
resize_pblock [get_pblocks read_in] -add {RAMB36_X0Y12:RAMB36_X0Y23}
set_property SNAPPING_MODE ON [get_pblocks read_in]
set_property IS_SOFT TRUE [get_pblocks read_in]
add_cells_to_pblock [get_pblocks read_in] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/read_in_inst]] -quiet


create_pblock zculling
resize_pblock [get_pblocks zculling] -add {SLICE_X63Y120:SLICE_X113Y179}
resize_pblock [get_pblocks zculling] -add {DSP48E2_X9Y42:DSP48E2_X14Y65}
resize_pblock [get_pblocks zculling] -add {RAMB18_X5Y48:RAMB18_X7Y71}
resize_pblock [get_pblocks zculling] -add {RAMB36_X5Y24:RAMB36_X7Y35}
resize_pblock [get_pblocks zculling] -add {URAM288_X1Y32:URAM288_X1Y47}
set_property SNAPPING_MODE ON [get_pblocks zculling]
set_property IS_SOFT TRUE [get_pblocks zculling]
add_cells_to_pblock [get_pblocks zculling] [get_cells -quiet [list level0_i/ulp/increment_1/mono_inst/zculling_inst]] -quiet
