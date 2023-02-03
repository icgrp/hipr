#!/usr/bin/env python
import argparse
import os


parser = argparse.ArgumentParser()
parser.add_argument('impl_dir')
parser.add_argument('-t', '--top',       type=str, default="no_func", help="set top function name for out of context synthesis")
parser.add_argument('-f', '--file_name', type=str, default="no_func", help="set output file name prefix")
parser.add_argument('-a', '--app_name',  type=str, default="no_func", help="set output file name prefix")

args      = parser.parse_args()
impl_dir  = args.impl_dir
top_name  = args.top
file_name = args.file_name
app_name  = args.app_name

board='yboard_101'
F001_dir='F001_overlay_zcu102_1_200M'



# prepare the tcl file to restore the top dcp file
file_in = open(impl_dir+'/'+file_name+'.tcl', 'r')
file_out = open(impl_dir+'/'+file_name+'_mk_overlay_'+app_name+'.tcl', 'w')

copy_enable = True
for line in file_in:
  if copy_enable:
    if (line.replace('add_files', '') != line):
      file_out.write('# ' + line)
    elif (line.replace('write_checkpoint -force', '') != line):
      file_out.write('write_checkpoint -force design_route_'+app_name+'.dcp\n')
    elif (line.replace('write_bitstream -force', '') != line):
      file_out.write('\n')
      file_out.write('report_timing_summary > ../../../../../../../../../../'+str(F001_dir)+'/'+board+'_dfx_hipr/checkpoint/overlay_timing.rpt \n')
      file_out.write('pr_recombine -cell level0_i/ulp/increment_1/mono_inst/data_proc1_inst\n')
      file_out.write('write_bitstream -force -cell level0_i/ulp/increment_1/mono_inst/data_proc1_inst ./data_proc1_'+app_name+'.bit\n')
    elif (line.replace('set_property SCOPED_TO_CELLS', '') != line):
      file_out.write('# ' + line)
      file_out.write('add_files ../../../../../../../../../../'+str(F001_dir)+'/'+board+'_dfx_hipr/checkpoint/hw_bb_divided.dcp\n')
      # file_out.write('add_files ../../../../../../../../../../'+str(F001_dir)+'/place_holder/data_proc1_netlist_false.dcp\n')
      #page_dcp_anchor 

      file_out.write('add_files ../../../../../../../../../../'+str(F001_dir)+'/'+board+'_dfx_hipr/xdc/sub.xdc\n') 
      # file_out.write('set_property SCOPED_TO_CELLS {level0_i/ulp/increment_1/mono_inst/data_proc1_inst }  [get_files ../../../../../../../../../../'+str(F001_dir)+'/place_holder/data_proc1_netlist.dcp]\n')
      #scope_anchor

      file_out.write('set_property USED_IN {implementation} [get_files ../../../../../../../../../../'+str(F001_dir)+'/'+board+'_dfx_hipr/xdc/sub.xdc]\n')
      file_out.write('set_property PROCESSING_ORDER LATE [get_files ../../../../../../../../../../'+str(F001_dir)+'/'+board+'_dfx_hipr/xdc/sub.xdc]\n')
    elif (line.replace('reconfig_partitions', '') != line):
      file_out.write('# ' + line)
      file_out.write('link_design -mode default -part xcu50-fsvh2104-2-e -reconfig_partitions {level0_i/ulp/increment_1/mono_inst/data_proc1_inst } -top level0_wrapper\n')
    elif (line.replace('_full_route_post', '') != line):
      file_out.write('# ' + line)
    else:
      file_out.write(line)
      
file_in.close()
file_out.close()

try:
  file_in   = open(impl_dir+'/../../../.local/hw_platform/tcl_hooks/impl.xdc', 'r')
  file_out  = open(impl_dir+'/../../../.local/hw_platform/tcl_hooks/.impl.xdc', 'w')
  
  for line in file_in:
    if (line.replace('SLR', '') != line):
      file_out.write('# ' + line)
    else:
      file_out.write(line)
  
  file_in.close()
  file_out.close()
  os.system('mv '+impl_dir+'/../../../.local/hw_platform/tcl_hooks/.impl.xdc ' + impl_dir+'/../../../.local/hw_platform/tcl_hooks/impl.xdc')
   
  file_in   = open(impl_dir+'/../../../.local/hw_platform/tcl_hooks/preopt.tcl', 'r')
  file_out  = open(impl_dir+'/../../../.local/hw_platform/tcl_hooks/.preopt.tcl', 'w')
  
  for line in file_in:
    if (line.replace('SLR', '') != line):
      file_out.write('# ' + line)
    else:
      file_out.write(line)
  
  file_in.close()
  file_out.close()
  os.system('mv '+ impl_dir+'/../../../.local/hw_platform/tcl_hooks/.preopt.tcl ' + impl_dir+'/../../../.local/hw_platform/tcl_hooks/preopt.tcl')
except:
  print("cannot modify")
 











