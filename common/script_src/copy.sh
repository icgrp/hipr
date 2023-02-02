#!/bin/bash -e




listVar="debug_profile_hooks.tcl _full_init_post.tcl _full_init_pre.tcl _full_opt_post.tcl _full_opt_pre.tcl _full_place_post.tcl _full_place_pre.tcl _full_route_post.tcl ocl_util.tcl platform.tcl postopt.tcl postroute.tcl preopt.tcl vpl_init.tcl _vpl_post_init.tcl _vpl_post_opt.tcl _vpl_post_place.tcl _vpl_post_route.tcl _vpl_pre_init.tcl _vpl_pre_opt.tcl _vpl_pre_place.tcl"


for i in $listVar; do
  cp $(find -name $i) ../../common/constraints/au50_200M/scripts/
done

listVar="dont_partition.xdc impl.xdc ulp_ooc_copy.xdc"

for i in $listVar; do
  cp $(find -name $i) ../../common/constraints/au50_200M/xdc/
done


