
# These pblocks ranges are generated from the BLP write_xdc.
#create_pblock  pblock_dynamic_region 

add_cells_to_pblock [get_pblocks pblock_dynamic_region] [get_cells  [list level0_i/ulp]]

resize_pblock [get_pblocks pblock_dynamic_region] -add {SLICE_X117Y660:SLICE_X145Y719 SLICE_X176Y60:SLICE_X196Y659 SLICE_X220Y540:SLICE_X221Y599 SLICE_X117Y180:SLICE_X145Y239 SLICE_X206Y0:SLICE_X232Y59}
resize_pblock [get_pblocks pblock_dynamic_region] -add {BLI_HBM_APB_INTF_X30Y0:BLI_HBM_APB_INTF_X31Y0}
resize_pblock [get_pblocks pblock_dynamic_region] -add {BLI_HBM_AXI_INTF_X30Y0:BLI_HBM_AXI_INTF_X31Y0}
resize_pblock [get_pblocks pblock_dynamic_region] -add {CONFIG_SITE_X0Y2:CONFIG_SITE_X0Y2}
resize_pblock [get_pblocks pblock_dynamic_region] -add {DSP48E2_X16Y258:DSP48E2_X19Y281 DSP48E2_X25Y18:DSP48E2_X28Y257 DSP48E2_X16Y66:DSP48E2_X19Y89 DSP48E2_X30Y0:DSP48E2_X31Y17}
resize_pblock [get_pblocks pblock_dynamic_region] -add {GTYE4_CHANNEL_X1Y36:GTYE4_CHANNEL_X1Y39}
resize_pblock [get_pblocks pblock_dynamic_region] -add {GTYE4_COMMON_X1Y9:GTYE4_COMMON_X1Y9}
resize_pblock [get_pblocks pblock_dynamic_region] -add {LAGUNA_X16Y480:LAGUNA_X19Y599 LAGUNA_X24Y0:LAGUNA_X27Y479 LAGUNA_X16Y0:LAGUNA_X19Y119}
resize_pblock [get_pblocks pblock_dynamic_region] -add {RAMB18_X8Y264:RAMB18_X9Y287 RAMB18_X11Y24:RAMB18_X11Y263 RAMB18_X8Y72:RAMB18_X9Y95 RAMB18_X12Y0:RAMB18_X13Y23}
resize_pblock [get_pblocks pblock_dynamic_region] -add {RAMB36_X8Y132:RAMB36_X9Y143 RAMB36_X11Y12:RAMB36_X11Y131 RAMB36_X8Y36:RAMB36_X9Y47 RAMB36_X12Y0:RAMB36_X13Y11}
resize_pblock [get_pblocks pblock_dynamic_region] -add {URAM288_X2Y176:URAM288_X2Y191 URAM288_X4Y16:URAM288_X4Y175 URAM288_X2Y48:URAM288_X2Y63}
resize_pblock [get_pblocks pblock_dynamic_region] -add {CLOCKREGION_X5Y11:CLOCKREGION_X7Y11 CLOCKREGION_X0Y11:CLOCKREGION_X3Y11 CLOCKREGION_X0Y4:CLOCKREGION_X5Y10 CLOCKREGION_X5Y3:CLOCKREGION_X5Y3 CLOCKREGION_X0Y3:CLOCKREGION_X3Y3 CLOCKREGION_X0Y1:CLOCKREGION_X5Y2 CLOCKREGION_X0Y0:CLOCKREGION_X6Y0}
set_property SNAPPING_MODE ON [get_pblocks pblock_dynamic_region]
set_property IS_SOFT FALSE [get_pblocks pblock_dynamic_region]

# # create_pblock pblock_dynamic_SLR0
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {SLICE_X206Y0:SLICE_X232Y59 SLICE_X176Y60:SLICE_X196Y239 SLICE_X117Y180:SLICE_X145Y239}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {DSP48E2_X25Y18:DSP48E2_X28Y89 DSP48E2_X16Y66:DSP48E2_X19Y89 DSP48E2_X30Y0:DSP48E2_X31Y17}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {LAGUNA_X24Y0:LAGUNA_X27Y119 LAGUNA_X16Y0:LAGUNA_X19Y119}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {RAMB18_X11Y24:RAMB18_X11Y95 RAMB18_X8Y72:RAMB18_X9Y95 RAMB18_X12Y0:RAMB18_X13Y23}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {RAMB36_X11Y12:RAMB36_X11Y47 RAMB36_X8Y36:RAMB36_X9Y47 RAMB36_X12Y0:RAMB36_X13Y11}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {URAM288_X4Y16:URAM288_X4Y63 URAM288_X2Y48:URAM288_X2Y63}
# # resize_pblock [get_pblocks pblock_dynamic_SLR0] -add {CLOCKREGION_X5Y3:CLOCKREGION_X5Y3 CLOCKREGION_X0Y3:CLOCKREGION_X3Y3 CLOCKREGION_X0Y1:CLOCKREGION_X5Y2 CLOCKREGION_X0Y0:CLOCKREGION_X6Y0}
# # set_property SNAPPING_MODE ON [get_pblocks pblock_dynamic_SLR0]
# # set_property IS_SOFT FALSE [get_pblocks pblock_dynamic_SLR0]


# # create_pblock pblock_dynamic_SLR1
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {SLICE_X176Y240:SLICE_X196Y479}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {DSP48E2_X25Y90:DSP48E2_X28Y185}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {LAGUNA_X24Y120:LAGUNA_X27Y359}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {RAMB18_X11Y96:RAMB18_X11Y191}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {RAMB36_X11Y48:RAMB36_X11Y95}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {URAM288_X4Y64:URAM288_X4Y127}
# # resize_pblock [get_pblocks pblock_dynamic_SLR1] -add {CLOCKREGION_X0Y4:CLOCKREGION_X5Y7}
# # set_property SNAPPING_MODE ON [get_pblocks pblock_dynamic_SLR1]
# # set_property IS_SOFT FALSE [get_pblocks pblock_dynamic_SLR1]


# # create_pblock pblock_dynamic_SLR2
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {SLICE_X117Y660:SLICE_X145Y719 SLICE_X176Y480:SLICE_X196Y659 SLICE_X220Y540:SLICE_X221Y599}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {DSP48E2_X16Y258:DSP48E2_X19Y281 DSP48E2_X25Y186:DSP48E2_X28Y257}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {LAGUNA_X16Y480:LAGUNA_X19Y599 LAGUNA_X24Y360:LAGUNA_X27Y479}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {RAMB18_X8Y264:RAMB18_X9Y287 RAMB18_X11Y192:RAMB18_X11Y263}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {RAMB36_X8Y132:RAMB36_X9Y143 RAMB36_X11Y96:RAMB36_X11Y131}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {URAM288_X2Y176:URAM288_X2Y191 URAM288_X4Y128:URAM288_X4Y175}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {CLOCKREGION_X5Y11:CLOCKREGION_X7Y11 CLOCKREGION_X0Y11:CLOCKREGION_X3Y11 CLOCKREGION_X0Y8:CLOCKREGION_X5Y10}
# # resize_pblock [get_pblocks pblock_dynamic_SLR2] -add {CONFIG_SITE_X0Y2:CONFIG_SITE_X0Y2}
# # set_property SNAPPING_MODE ON [get_pblocks pblock_dynamic_SLR2]
# # set_property IS_SOFT FALSE [get_pblocks pblock_dynamic_SLR2]

# # ### SLR0 #############
# # add_cells_to_pblock [get_pblocks pblock_dynamic_SLR0] [get_cells level0_i/ulp/SLR0]
# # add_cells_to_pblock [get_pblocks pblock_dynamic_SLR0] [get_cells level0_i/ulp/ulp_ucs/inst/frequency_counters/frequency_counter_aclk_hbm]

# # ### SLR1 #############
# # add_cells_to_pblock [get_pblocks pblock_dynamic_SLR1] [get_cells level0_i/ulp/SLR1]

# # ### SLR2 #############
# # add_cells_to_pblock [get_pblocks pblock_dynamic_SLR2] [get_cells level0_i/ulp/SLR2]
# # add_cells_to_pblock [get_pblocks pblock_dynamic_SLR2] [get_cells level0_i/ulp/axi_data_sc] -quiet

#-----------------
# # # A 2-SLR crossing axi_register_slice pipeline is inserted between axi_data_sc in slr2 and plram in slr0
#-----------------
# # add_cells_to_pblock  [get_pblocks pblock_dynamic_SLR2] [get_cells -hierarchical -filter "NAME=~level0_i/ulp/axi_regslice*_slr0/inst/*slr_master*"] -quiet

# # add_cells_to_pblock  [get_pblocks pblock_dynamic_SLR1] [get_cells -hierarchical -filter "NAME=~level0_i/ulp/axi_regslice*_slr0/inst/*slr_middle*"] -quiet

# # add_cells_to_pblock  [get_pblocks pblock_dynamic_SLR0] [get_cells -hierarchical -filter "NAME=~level0_i/ulp/axi_regslice*_slr0/inst/*slr_slave*"] -quiet

#-----------------
# # # A 1-SLR crossing axi_register_slice pipeline is inserted between axi_data_sc in slr2 and plram in slr1
#-----------------
# # add_cells_to_pblock  [get_pblocks pblock_dynamic_SLR2] [get_cells -hierarchical -filter "NAME=~level0_i/ulp/axi_regslice*_slr1/inst/*slr_master*"] -quiet

# # add_cells_to_pblock  [get_pblocks pblock_dynamic_SLR1] [get_cells -hierarchical -filter "NAME=~level0_i/ulp/axi_regslice*_slr1/inst/*slr_slave*"] -quiet


# #######################################################################
# WARNING: WORKAROUND!
# #######################################################################
#
# These constraints are added as a workaround to CR-1038346
# Remove these constraints when CR is resolved.
#
# Error codes: ERROR: [VPL 30-1112]
#
# # set_property CONTAIN_ROUTING 0 [get_pblocks pblock_dynamic_SLR0]
# # set_property EXCLUDE_PLACEMENT 0 [get_pblocks pblock_dynamic_SLR0]
# # set_property CONTAIN_ROUTING 0 [get_pblocks pblock_dynamic_SLR1]
# # set_property EXCLUDE_PLACEMENT 0 [get_pblocks pblock_dynamic_SLR1]
# # set_property CONTAIN_ROUTING 0 [get_pblocks pblock_dynamic_SLR2]
# # set_property EXCLUDE_PLACEMENT 0 [get_pblocks pblock_dynamic_SLR2]


set_false_path -through  [get_pins -hierarchical -filter {NAME=~level0_i/ulp/*lp_s_irq_cu_00*}]
set_false_path -through  [get_pins -hier -regexp .*mss_0/inst/hbm_inst/inst/.*_STACK.*/AXI_.*_ARESET_N.*]

# Need to Confirm that this BACKBONE constraint is needed in ulp's impl.xdc
set_property CLOCK_DEDICATED_ROUTE BACKBONE [get_nets level0_i/blp/blp_i/freerun_clk/bufg_div/U0/BUFGCE_O[0]]

set_property LOC MMCM_X0Y0 [get_cells level0_i/ulp/ulp_ucs/inst/aclk_hbm_hierarchy/clkwiz_hbm/inst/CLK_CORE_DRP_I/clk_inst/mmcme4_adv_inst]


#########################################################################
# Give high priority to kernel clock generation and HBM clock
#########################################################################
set_property HIGH_PRIORITY true [get_nets -of_objects [get_pins level0_i/ulp/ulp_ucs/inst/aclk_kernel_00_hierarchy/clock_throttling_aclk_kernel_00/Clk_Out]]
set_property HIGH_PRIORITY true [get_nets -of_objects [get_pins level0_i/ulp/ulp_ucs/inst/aclk_kernel_01_hierarchy/clock_throttling_aclk_kernel_01/Clk_Out]]



set_property HIGH_PRIORITY true [get_nets -of_objects [get_pins level0_i/ulp/ulp_ucs/inst/aclk_hbm_hierarchy/clkwiz_hbm/inst/CLK_CORE_DRP_I/clk_inst/mmcme4_adv_inst/CLKOUT0]]

# JUSTIFICATION: Like the interrupt signals, the DDR calibration signal originates in asynchronous clock domains
# that differ from pipeline clock in the isolation interface but since the signal is largely static a false path is
# sufficient to resolve the subsequent timing failures.
set_false_path -through [get_pins -hierarchical -filter {NAME=~level0_i/ii_level0_pipe/inst/pxi_ii_core/inst/*data_memory_calib_complete_00*}]
