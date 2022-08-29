# -*- coding: utf-8 -*-   
# Company: IC group, University of Pennsylvania
# Contributor: Yuanlong Xiao
#
# Create Date: 02/02/2022
# Design Name: overlay hipr
# Project Name: hipr
# Versions: 1.0
# Description: This is a python script to prepare the script for static region 
#              compile for PLD (https://github.com/icgrp/pld2022).
# Dependencies: python, gen_basic.py hls.py
# Revision:
# Revision 0.01 - File Created (02/02/2022)
#
# Additional Comments:


import os  
import subprocess
from pr_flow.gen_basic import gen_basic
from pr_flow.hls       import hls

class overlay(gen_basic):
  def __init__(self, prflow_params):
    gen_basic.__init__(self, prflow_params)

  def update_connect_for_hipr(self, connection_list):
    list_out  = []
    for connect in connection_list:
      connect_list = connect.split('->')
      str_ele = connect_list[0].split('.')[0]+'\t'+connect_list[1].split('.')[0]
      # if (str_ele.replace('DMA', '') == str_ele): list_out.append(str_ele) 
      list_out.append(str_ele) 
      #list_out.append(connect_list[0].split('.')[0]+'\t'+connect_list[1].split('.')[0]) 
    # list_out = set(list_out)
    file_out = open(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/cpp/src/app/connect.txt', 'w')
    for line in list_out: file_out.write(line+'\n')
    file_out.close()

  # create dummy directory for each empty block
  def create_place_holder(self, operators):
    # extract the stream arguments and types (in/out and width) for all the operators
    operator_arg_dict, operator_width_dict = self.dataflow.return_operator_io_argument_dict(operators)

    # extract the variables used in top.cpp 
    operator_var_dict = self.dataflow.return_operator_inst_dict(operators)
   
    # extract the how different operators are connected from top.cpp 
    connection_list=self.dataflow.return_operator_connect_list(operator_arg_dict, operator_var_dict, operator_width_dict)
    self.update_connect_for_hipr(connection_list)

    # generate Verilog netlist for the dataflow graph
    mono_v_list = self.verilog.return_operator_inst_v_list(operator_arg_dict, connection_list, operator_var_dict, operator_width_dict)

    # write the Verilog netlist to hipr directory
    self.shell.write_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/src4level2/ydma_bb/mono.v', mono_v_list)

    # Utilize hls class to prepare the high-level-synthesis work directory
    hls_inst = hls(self.prflow_params)
    for operator in operators.split():
      operator_arg_dict, operator_width_dict = self.dataflow.return_operator_io_argument_dict(operator)
      in_width_list, out_width_list = self.dataflow.return_io_width(operator_width_dict[operator], operator_arg_dict[operator])
      target_exist, target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target') 
      # prepare the hls workspace

      hls_inst.run(operator, self.overlay_dir+'/place_holder', '../../..', ['syn_'+operator+'.tcl', 'syn_'+operator+'_dummy.tcl'])
      if target_exist == True and target == 'HIPR':
        # if the operator is hipr type, use dummy as dcp for initial overlay P&R
        self.shell.cp_file('./common/verilog_src/stream_shell.v',  self.overlay_dir+'/place_holder')
        self.shell.write_lines(self.overlay_dir+'/place_holder/syn_'+operator+'.tcl',       self.tcl.return_syn_page_tcl_list(operator,  [operator+'_top.v', 'stream_shell.v'], top_name=operator+'_top', hls_src='./'+operator+'_prj/'+operator+'/syn/verilog',   dcp_name=operator+'_netlist_false.dcp', rpt_name='utilization_'+operator+'.rpt'))
        self.shell.write_lines(self.overlay_dir+'/place_holder/syn_'+operator+'_dummy.tcl', self.tcl.return_syn_page_tcl_list(operator,                                     [], top_name=operator       , hls_src='./'+operator+'_prj/'+operator+'/verilog_dummy', dcp_name=operator+'_netlist.dcp', rpt_name='utilization.rpt'))
        operator_arg_dict, operator_width_dict = self.dataflow.return_operator_io_argument_dict(operator)
        in_width_list, out_width_list = self.dataflow.return_io_width(operator_width_dict[operator], operator_arg_dict[operator])
        input_num  = len(in_width_list) 
        output_num = len(out_width_list) 
        addr_width_dict = {}
        for i in range(1, 8):  addr_width_dict['Output_'+str(i)] = self.prflow_params['bram_addr_bits']
        for arg in  operator_arg_dict[operator]:
          port_depth_exist, depth = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', arg+'_depth')
          if port_depth_exist: addr_width_dict[arg] = depth

        self.shell.write_lines(self.overlay_dir+'/place_holder/'+operator+'_top.v',
                           self.verilog.return_hipr_page_v_list(operator,
                                                                operator_arg_dict[operator],
                                                                operator_width_dict[operator],
                                                                addr_width_dict),
                                                                False)
 

      else:
        # if the operator is not hipr type, use real dcp as dcp for initial overlay P&R
        self.shell.write_lines(self.overlay_dir+'/place_holder/syn_'+operator+'.tcl',       self.tcl.return_syn_page_tcl_list(operator,  [], top_name=operator, hls_src='./'+operator+'_prj/'+operator+'/syn/verilog',   dcp_name=operator+'_netlist.dcp', rpt_name='utilization_'+operator+'.rpt'))
        self.shell.write_lines(self.overlay_dir+'/place_holder/syn_'+operator+'_dummy.tcl', self.tcl.return_syn_page_tcl_list(operator,  [], top_name=operator, hls_src='./'+operator+'_prj/'+operator+'/verilog_dummy', dcp_name=operator+'netlist.dcp_false', rpt_name='utilization.rpt'))
      self.shell.write_lines(self.overlay_dir+'/place_holder/'+operator+'_prj/'+operator+'/verilog_dummy/'+operator+'.v', self.verilog.return_place_holder_v_list(operator, in_width_list, out_width_list)) 


      # write the dummy netlist with only ports definitions to the hipr workspace
      self.shell.write_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/src4level2/ydma_bb/'+operator+'.v', self.verilog.return_place_holder_v_list(operator, in_width_list, out_width_list, is_dummy=True)) 

    str_list = [\
       'operators='+operators, \
       'targets=$(foreach n, $(operators),  utilization_$(n).rpt)', \
       'all:$(targets)', \
       '', \
       '$(targets):utilization_%.rpt:main_%.sh', \
       '\t./$<', \
       'clean:', \
       '\trm -rf *.dcp *.log *.rpt *.jou' \
    ]

    self.shell.write_lines(self.overlay_dir+'/place_holder/Makefile', str_list)

  # run.sh will be used for generating the overlay.dcp 
  def return_run_sh_list_local(self, operators):
    lines_list = []
    lines_list.append('#!/bin/bash -e')
    lines_list.append('#place_holder anchor')
    lines_list.append('cd place_holder')
    lines_list.append('make -j$(nproc)')
    lines_list.append('cd -')

    # launch hls for each operator
    print(operators) 
    # generate the 2nd-level DFX regions 
    if self.prflow_params['overlay_type'] == 'hipr': # generate abstract shell dcps for hipr overlay 
      lines_list.append('cd '+self.prflow_params['board']+'_dfx_hipr')
    else: # generate abstract shell dcps for psnoc overlay
      lines_list.append('cd '+self.prflow_params['board']+'_dfx_manual')
    lines_list.append('source '+self.prflow_params['Xilinx_dir'])
    lines_list.append('make -j3')
    lines_list.append('cd -')
    
    # copy the dcps and xclbins from overlay workspace
    lines_list.append('cp ../F000_ydma_'+self.prflow_params['freq']+'/ydma/'+self.prflow_params['board']+'/_x/link/int/ydma.xml ./dynamic_region.xml')
    lines_list.append('cp ../F000_ydma_'+self.prflow_params['freq']+'/ydma/'+self.prflow_params['board']+'/_x/link/vivado/vpl/prj/prj.runs/impl_1/dynamic_region.bit ./')
    lines_list.append('./gen_xclbin_'+self.prflow_params['board']+'.sh dynamic_region.bit dynamic_region.xml dynamic_region.xclbin ../F000_ydma_'+self.prflow_params['freq'])
    return lines_list

 
  def create_shell_file(self, operators):
    # copy the shell script to generate xclbin
    self.shell.cp_file('./common/script_src/gen_xclbin_'+self.prflow_params['board']+'.sh ', self.overlay_dir)

    # generate the shell script to generate the overlay
    self.shell.write_lines(self.overlay_dir+'/run.sh', self.return_run_sh_list_local(operators), True)
    
    # generate the shell script to call run.sh depends on the scheduler.
    # scheduler: slurm, qsub, local 
    self.shell.write_lines(self.overlay_dir+'/main.sh', self.shell.return_main_sh_list(\
                                                       './run.sh', \
                                                       self.prflow_params['back_end'], \
                                                       'NONE',\
                                                       'overlay', \
                                                       self.prflow_params['grid'],  \
                                                       self.prflow_params['email'], \
                                                       self.prflow_params['mem'],  \
                                                       self.prflow_params['maxThreads']), True)
 
  def update_cad_path(self, operators, hipr_pages_list):
    # replace fifo_512X1024 with mono instance
    subs_str='\n'
    subs_str+='mono mono_inst(\n'
    subs_str+='  .ap_clk(ap_clk),\n'
    subs_str+='  .ap_rst_n(ap_rst_n),\n'
    subs_str+='  .Input_1_V_TDATA(Loop_VITIS_LOOP_35_3_proc3_U0_v2_buffer_V_din),\n'
    subs_str+='  .Input_1_V_TVALID(Loop_VITIS_LOOP_35_3_proc3_U0_v2_buffer_V_write),\n'
    subs_str+='  .Input_1_V_TREADY(v2_buffer_V_full_n),\n'
    subs_str+='  .Output_1_V_TDATA(v2_buffer_V_dout),\n'
    subs_str+='  .Output_1_V_TVALID(v2_buffer_V_empty_n),\n'
    subs_str+='  .Output_1_V_TREADY(Loop_VITIS_LOOP_36_4_proc4_U0_v2_buffer_V_read),\n'
    subs_str+='  .ap_start(ap_start)\n'
    subs_str+=');\n'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/src4level2/ydma_bb/ydma.v', {'ydma_fifo_w512_d1024_A': subs_str})

    # update the second-level definition tcl scripts 
    str_line =  'pr_subdivide -cell '+self.prflow_params['inst_name'].replace('/ydma_1', '') +' -subcells {'
    for operator in hipr_pages_list: str_line += self.prflow_params['inst_name'] + '/mono_inst/'+operator+'_inst '
    str_line += '} ./checkpoint/pfm_dynamic_new_bb.dcp'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/tcl/sub_divided.tcl', {'pr_subdivide': str_line})

    # update the tcl script to place&route the overlay with dummy logic
    str_line = 'F001_dir=\'F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'\''
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'F001_dir=': str_line})
    str_line = ''
    for operator in operators.split():
      str_line += '      file_out.write(\'set_property SCOPED_TO_CELLS {'+self.prflow_params['inst_name']+'/mono_inst/'+operator+'_inst }  [get_files ../../../../../../../../../../F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'/place_holder/'+operator+'_netlist_false.dcp]\\n\')\n'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'scope_anchor': str_line})

    str_line = ''
    for operator in operators.split():
      str_line += '      file_out.write(\'add_files ../../../../../../../../../../F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'/place_holder/'+operator+'_netlist_false.dcp\\n\')\n'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'page_dcp_anchor': str_line})
        
    str_line = '      file_out.write(\'link_design -mode default -part '+self.prflow_params['part']+' -reconfig_partitions {'
    for operator in hipr_pages_list:
      str_line += self.prflow_params['inst_name']+'/mono_inst/'+operator+'_inst '
    str_line += '} -top '+self.prflow_params['top_name']+'\\n\')'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'link_design': str_line})

    str_line = ''
    for operator in hipr_pages_list: str_line += '      file_out.write(\'report_utilization -pblocks '+operator+' > ../../../../../../../../../../F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'/utilization_'+operator+'.rpt\\n\')\n'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'utilization_anchor': str_line})
    str_line = '      file_out.write(\'add_files ../../../../../../../../../../F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'/au50_dfx_hipr/checkpoint/hw_bb_divided.dcp\\n\')'
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/python/mk_overlay_tcl.py', {'hw_bb_divided_anchor': str_line})

    str_line = 'base_list='
    str_hipr_line = 'base_hipr_list='
    for operator in operators.split():
      str_line += operator+' '
      # construct the tcl script to generate the abstract shell 
      if operator in hipr_pages_list:
        str_hipr_line += operator+' '
        self.shell.write_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/tcl/gen_abs_'+operator+'.tcl',\
                               self.tcl.return_gen_abs_tcl_list('./checkpoint/overlay.dcp', self.prflow_params['inst_name']+'/mono_inst/'+operator+'_inst ', './checkpoint/'+operator+'.dcp'))

    # update the DFX region shell targets according to the benchmark in the Makefile
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/Makefile', {'workspace='          : 'workspace=../../F000_ydma_'+self.prflow_params['freq']+'/ydma/'+self.prflow_params['board']})
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/Makefile', {'base_list='          : str_line+'\n'+str_hipr_line})
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/Makefile', {'vitis_impl_tcl_name=': 'vitis_impl_tcl_name='+self.prflow_params['top_name']})
    self.shell.replace_lines(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/Makefile', {'app='                : 'app='+self.prflow_params['benchmark_name']})

  def update_resource_pragma(self, operators):
    pragma_dict = {}
    str_operators = ''
    for operator in operators.split():
      target_exist, target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target') 
      if target_exist == True and target == 'HIPR':
        cls_exist,  value_c = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'clb') 
        ff_exist,   value_f = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'ff') 
        bram_exist, value_b = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'bram') 
        dsp_exist,  value_d = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'dsp') 
        pragma_dict[operator] = [value_c, value_f, value_b, value_d]
        str_operators += operator
      else:
        pragma_dict[operator] = ['1', '1', '1', '1']

    file_out = open(self.overlay_dir+'/'+self.prflow_params['board']+'_dfx_hipr/cpp/src/app/'+self.prflow_params['benchmark_name']+'/pragma.txt', 'w')
    for key, value in sorted(pragma_dict.items()):
      # print key.ljust(30)+'\t'+'\t'.join(value)+'\n'
      file_out.write(key.ljust(30)+'\t'+'\t'.join(value)+'\n')
    file_out.close()
    return str_operators

  def return_hipr_list(self, operators): 
    out_list = []
    for operator in operators.split():
      target_exist, target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target') 
      if target_exist == True and target == 'HIPR': out_list.append(operator)
    return out_list

  def run(self, operators):
    # make work directory
    self.shell.mkdir(self.prflow_params['workspace'])
    self.shell.re_mkdir(self.overlay_dir)

    # find all hipr pages
    hipr_pages_list = self.return_hipr_list(operators)
   

    # copy the hld/xdc files from input source directory
    self.shell.del_dir(self.overlay_dir+'/src')
    self.shell.cp_dir('./common/verilog_src', self.overlay_dir+'/src')

    # copy the hipr work_dir to F001
    self.shell.cp_dir('./common/hipr/'+self.prflow_params['board']+'_dfx_hipr', self.overlay_dir)

    # update the cad tool path
    self.update_cad_path(operators, hipr_pages_list)

    # update the pragma for hipr ovelay generation
    self.update_resource_pragma(operators)

    # generate shell files for local run
    self.create_shell_file(operators)

    # create dummy logic place and route the overlay.dcp
    self.create_place_holder(operators)

    # create a folder to store the partial bitstreams for different versions of riscv
    # implementations for different pages
    self.shell.re_mkdir(self.overlay_dir+'/riscv_bit_lib')



