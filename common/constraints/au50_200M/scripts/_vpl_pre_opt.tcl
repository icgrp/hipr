# This file was automatically generated by Vpl
if { ![info exists _is_init_cmds] } {
  source ./scripts/vpl_init.tcl
  source ./scripts/ocl_util.tcl
  source ./scripts/platform.tcl
  source ./scripts/debug_profile_hooks.tcl
  namespace import ocl_util::*

  set _is_init_cmds true
}


ocl_util::report_utilization_drc "luts -1 brams -1 registers -1 dsps -1" "level0_i/ulp" 0.8 $steps_log $vivado_output_dir