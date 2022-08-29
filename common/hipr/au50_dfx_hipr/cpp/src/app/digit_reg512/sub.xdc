

create_pblock update_knn1
resize_pblock [get_pblocks update_knn1] -add {SLICE_X24Y365:SLICE_X42Y419}
resize_pblock [get_pblocks update_knn1] -add {DSP48E2_X4Y146:DSP48E2_X7Y167}
resize_pblock [get_pblocks update_knn1] -add {RAMB18_X3Y146:RAMB18_X5Y167}
resize_pblock [get_pblocks update_knn1] -add {RAMB36_X3Y73:RAMB36_X5Y83}
set_property SNAPPING_MODE ON [get_pblocks update_knn1]
set_property IS_SOFT TRUE [get_pblocks update_knn1]
add_cells_to_pblock [get_pblocks update_knn1] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn1_inst]]


create_pblock update_knn10
resize_pblock [get_pblocks update_knn10] -add {SLICE_X72Y305:SLICE_X83Y359}
resize_pblock [get_pblocks update_knn10] -add {DSP48E2_X14Y122:DSP48E2_X15Y143}
resize_pblock [get_pblocks update_knn10] -add {RAMB18_X9Y122:RAMB18_X10Y143}
resize_pblock [get_pblocks update_knn10] -add {RAMB36_X9Y61:RAMB36_X10Y71}
set_property SNAPPING_MODE ON [get_pblocks update_knn10]
set_property IS_SOFT TRUE [get_pblocks update_knn10]
add_cells_to_pblock [get_pblocks update_knn10] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn10_inst]]


create_pblock update_knn11
resize_pblock [get_pblocks update_knn11] -add {SLICE_X81Y185:SLICE_X92Y239}
resize_pblock [get_pblocks update_knn11] -add {DSP48E2_X16Y74:DSP48E2_X17Y95}
resize_pblock [get_pblocks update_knn11] -add {RAMB18_X10Y74:RAMB18_X11Y95}
resize_pblock [get_pblocks update_knn11] -add {RAMB36_X10Y37:RAMB36_X11Y47}
set_property SNAPPING_MODE ON [get_pblocks update_knn11]
set_property IS_SOFT TRUE [get_pblocks update_knn11]
add_cells_to_pblock [get_pblocks update_knn11] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn11_inst]]


create_pblock update_knn12
resize_pblock [get_pblocks update_knn12] -add {SLICE_X76Y5:SLICE_X86Y59}
resize_pblock [get_pblocks update_knn12] -add {DSP48E2_X15Y2:DSP48E2_X15Y23}
resize_pblock [get_pblocks update_knn12] -add {RAMB18_X10Y2:RAMB18_X11Y23}
resize_pblock [get_pblocks update_knn12] -add {RAMB36_X10Y1:RAMB36_X11Y11}
set_property SNAPPING_MODE ON [get_pblocks update_knn12]
set_property IS_SOFT TRUE [get_pblocks update_knn12]
add_cells_to_pblock [get_pblocks update_knn12] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn12_inst]]


create_pblock update_knn13
resize_pblock [get_pblocks update_knn13] -add {SLICE_X1Y305:SLICE_X12Y359}
resize_pblock [get_pblocks update_knn13] -add {DSP48E2_X0Y122:DSP48E2_X1Y143}
resize_pblock [get_pblocks update_knn13] -add {RAMB18_X0Y122:RAMB18_X1Y143}
resize_pblock [get_pblocks update_knn13] -add {RAMB36_X0Y61:RAMB36_X1Y71}
set_property SNAPPING_MODE ON [get_pblocks update_knn13]
set_property IS_SOFT TRUE [get_pblocks update_knn13]
add_cells_to_pblock [get_pblocks update_knn13] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn13_inst]]


create_pblock update_knn14
resize_pblock [get_pblocks update_knn14] -add {SLICE_X26Y185:SLICE_X38Y239}
resize_pblock [get_pblocks update_knn14] -add {DSP48E2_X5Y74:DSP48E2_X6Y95}
resize_pblock [get_pblocks update_knn14] -add {RAMB18_X4Y74:RAMB18_X4Y95}
resize_pblock [get_pblocks update_knn14] -add {RAMB36_X4Y37:RAMB36_X4Y47}
set_property SNAPPING_MODE ON [get_pblocks update_knn14]
set_property IS_SOFT TRUE [get_pblocks update_knn14]
add_cells_to_pblock [get_pblocks update_knn14] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn14_inst]]


create_pblock update_knn15
resize_pblock [get_pblocks update_knn15] -add {SLICE_X14Y185:SLICE_X24Y239}
resize_pblock [get_pblocks update_knn15] -add {DSP48E2_X3Y74:DSP48E2_X3Y95}
resize_pblock [get_pblocks update_knn15] -add {RAMB18_X2Y74:RAMB18_X3Y95}
resize_pblock [get_pblocks update_knn15] -add {RAMB36_X2Y37:RAMB36_X3Y47}
set_property SNAPPING_MODE ON [get_pblocks update_knn15]
set_property IS_SOFT TRUE [get_pblocks update_knn15]
add_cells_to_pblock [get_pblocks update_knn15] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn15_inst]]


create_pblock update_knn16
resize_pblock [get_pblocks update_knn16] -add {SLICE_X67Y185:SLICE_X78Y239}
resize_pblock [get_pblocks update_knn16] -add {DSP48E2_X13Y74:DSP48E2_X15Y95}
resize_pblock [get_pblocks update_knn16] -add {RAMB18_X9Y74:RAMB18_X9Y95}
resize_pblock [get_pblocks update_knn16] -add {RAMB36_X9Y37:RAMB36_X9Y47}
set_property SNAPPING_MODE ON [get_pblocks update_knn16]
set_property IS_SOFT TRUE [get_pblocks update_knn16]
add_cells_to_pblock [get_pblocks update_knn16] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn16_inst]]


create_pblock update_knn17
resize_pblock [get_pblocks update_knn17] -add {SLICE_X10Y365:SLICE_X21Y419}
resize_pblock [get_pblocks update_knn17] -add {DSP48E2_X1Y146:DSP48E2_X3Y167}
resize_pblock [get_pblocks update_knn17] -add {RAMB18_X2Y146:RAMB18_X2Y167}
resize_pblock [get_pblocks update_knn17] -add {RAMB36_X2Y73:RAMB36_X2Y83}
set_property SNAPPING_MODE ON [get_pblocks update_knn17]
set_property IS_SOFT TRUE [get_pblocks update_knn17]
add_cells_to_pblock [get_pblocks update_knn17] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn17_inst]]


create_pblock update_knn18
resize_pblock [get_pblocks update_knn18] -add {SLICE_X58Y365:SLICE_X69Y419}
resize_pblock [get_pblocks update_knn18] -add {DSP48E2_X12Y146:DSP48E2_X13Y167}
resize_pblock [get_pblocks update_knn18] -add {RAMB18_X7Y146:RAMB18_X8Y167}
resize_pblock [get_pblocks update_knn18] -add {RAMB36_X7Y73:RAMB36_X8Y83}
set_property SNAPPING_MODE ON [get_pblocks update_knn18]
set_property IS_SOFT TRUE [get_pblocks update_knn18]
add_cells_to_pblock [get_pblocks update_knn18] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn18_inst]]


create_pblock update_knn19
resize_pblock [get_pblocks update_knn19] -add {SLICE_X81Y125:SLICE_X92Y179}
resize_pblock [get_pblocks update_knn19] -add {DSP48E2_X16Y50:DSP48E2_X17Y71}
resize_pblock [get_pblocks update_knn19] -add {RAMB18_X10Y50:RAMB18_X11Y71}
resize_pblock [get_pblocks update_knn19] -add {RAMB36_X10Y25:RAMB36_X11Y35}
set_property SNAPPING_MODE ON [get_pblocks update_knn19]
set_property IS_SOFT TRUE [get_pblocks update_knn19]
add_cells_to_pblock [get_pblocks update_knn19] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn19_inst]]


create_pblock update_knn2
resize_pblock [get_pblocks update_knn2] -add {SLICE_X1Y245:SLICE_X12Y299}
resize_pblock [get_pblocks update_knn2] -add {DSP48E2_X0Y98:DSP48E2_X1Y119}
resize_pblock [get_pblocks update_knn2] -add {RAMB18_X0Y98:RAMB18_X1Y119}
resize_pblock [get_pblocks update_knn2] -add {RAMB36_X0Y49:RAMB36_X1Y59}
set_property SNAPPING_MODE ON [get_pblocks update_knn2]
set_property IS_SOFT TRUE [get_pblocks update_knn2]
add_cells_to_pblock [get_pblocks update_knn2] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn2_inst]]


create_pblock update_knn20
resize_pblock [get_pblocks update_knn20] -add {SLICE_X20Y305:SLICE_X36Y359}
resize_pblock [get_pblocks update_knn20] -add {DSP48E2_X4Y122:DSP48E2_X6Y143}
resize_pblock [get_pblocks update_knn20] -add {RAMB18_X3Y122:RAMB18_X4Y143}
resize_pblock [get_pblocks update_knn20] -add {RAMB36_X3Y61:RAMB36_X4Y71}
set_property SNAPPING_MODE ON [get_pblocks update_knn20]
set_property IS_SOFT TRUE [get_pblocks update_knn20]
add_cells_to_pblock [get_pblocks update_knn20] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn20_inst]]


create_pblock update_knn3
resize_pblock [get_pblocks update_knn3] -add {SLICE_X1Y185:SLICE_X12Y239}
resize_pblock [get_pblocks update_knn3] -add {DSP48E2_X0Y74:DSP48E2_X1Y95}
resize_pblock [get_pblocks update_knn3] -add {RAMB18_X0Y74:RAMB18_X1Y95}
resize_pblock [get_pblocks update_knn3] -add {RAMB36_X0Y37:RAMB36_X1Y47}
set_property SNAPPING_MODE ON [get_pblocks update_knn3]
set_property IS_SOFT TRUE [get_pblocks update_knn3]
add_cells_to_pblock [get_pblocks update_knn3] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn3_inst]]


create_pblock update_knn4
resize_pblock [get_pblocks update_knn4] -add {SLICE_X56Y125:SLICE_X66Y179}
resize_pblock [get_pblocks update_knn4] -add {DSP48E2_X12Y50:DSP48E2_X12Y71}
resize_pblock [get_pblocks update_knn4] -add {RAMB18_X7Y50:RAMB18_X8Y71}
resize_pblock [get_pblocks update_knn4] -add {RAMB36_X7Y25:RAMB36_X8Y35}
set_property SNAPPING_MODE ON [get_pblocks update_knn4]
set_property IS_SOFT TRUE [get_pblocks update_knn4]
add_cells_to_pblock [get_pblocks update_knn4] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn4_inst]]


create_pblock update_knn5
resize_pblock [get_pblocks update_knn5] -add {SLICE_X73Y245:SLICE_X84Y299}
resize_pblock [get_pblocks update_knn5] -add {DSP48E2_X14Y98:DSP48E2_X15Y119}
resize_pblock [get_pblocks update_knn5] -add {RAMB18_X10Y98:RAMB18_X11Y119}
resize_pblock [get_pblocks update_knn5] -add {RAMB36_X10Y49:RAMB36_X11Y59}
set_property SNAPPING_MODE ON [get_pblocks update_knn5]
set_property IS_SOFT TRUE [get_pblocks update_knn5]
add_cells_to_pblock [get_pblocks update_knn5] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn5_inst]]


create_pblock update_knn6
resize_pblock [get_pblocks update_knn6] -add {SLICE_X48Y305:SLICE_X59Y359}
resize_pblock [get_pblocks update_knn6] -add {DSP48E2_X9Y122:DSP48E2_X11Y143}
resize_pblock [get_pblocks update_knn6] -add {RAMB18_X6Y122:RAMB18_X6Y143}
resize_pblock [get_pblocks update_knn6] -add {RAMB36_X6Y61:RAMB36_X6Y71}
set_property SNAPPING_MODE ON [get_pblocks update_knn6]
set_property IS_SOFT TRUE [get_pblocks update_knn6]
add_cells_to_pblock [get_pblocks update_knn6] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn6_inst]]


create_pblock update_knn7
resize_pblock [get_pblocks update_knn7] -add {SLICE_X81Y365:SLICE_X92Y419}
resize_pblock [get_pblocks update_knn7] -add {DSP48E2_X16Y146:DSP48E2_X17Y167}
resize_pblock [get_pblocks update_knn7] -add {RAMB18_X10Y146:RAMB18_X11Y167}
resize_pblock [get_pblocks update_knn7] -add {RAMB36_X10Y73:RAMB36_X11Y83}
set_property SNAPPING_MODE ON [get_pblocks update_knn7]
set_property IS_SOFT TRUE [get_pblocks update_knn7]
add_cells_to_pblock [get_pblocks update_knn7] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn7_inst]]


create_pblock update_knn8
resize_pblock [get_pblocks update_knn8] -add {SLICE_X35Y245:SLICE_X47Y299}
resize_pblock [get_pblocks update_knn8] -add {DSP48E2_X7Y98:DSP48E2_X8Y119}
resize_pblock [get_pblocks update_knn8] -add {RAMB18_X5Y98:RAMB18_X5Y119}
resize_pblock [get_pblocks update_knn8] -add {RAMB36_X5Y49:RAMB36_X5Y59}
set_property SNAPPING_MODE ON [get_pblocks update_knn8]
set_property IS_SOFT TRUE [get_pblocks update_knn8]
add_cells_to_pblock [get_pblocks update_knn8] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn8_inst]]


create_pblock update_knn9
resize_pblock [get_pblocks update_knn9] -add {SLICE_X61Y5:SLICE_X72Y59}
resize_pblock [get_pblocks update_knn9] -add {DSP48E2_X12Y2:DSP48E2_X13Y23}
resize_pblock [get_pblocks update_knn9] -add {RAMB18_X8Y2:RAMB18_X9Y23}
resize_pblock [get_pblocks update_knn9] -add {RAMB36_X8Y1:RAMB36_X9Y11}
set_property SNAPPING_MODE ON [get_pblocks update_knn9]
set_property IS_SOFT TRUE [get_pblocks update_knn9]
add_cells_to_pblock [get_pblocks update_knn9] [get_cells -quiet [list pfm_top_i/dynamic_region/ydma_1/mono_inst/update_knn9_inst]]
