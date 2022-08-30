# -*- coding: utf-8 -*-   
# Company: IC group, University of Pennsylvania
# Contributor: Yuanlong Xiao
#
# Create Date: 02/11/2021
# Design Name: syn.py
# Project Name: PLD
# Versions: 1.0
# Description: This is a python script to prepare the script for Out-Context-Synthesis 
#              from verilog to DCP for PRFlow
# Dependencies: python, gen_basic.py hls.py config.py
# Revision:
# Revision 0.01 - File Created
# Revision 0.02 - Update cotents for HiPR
#
# Additional Comments:

import os  
import subprocess
from pr_flow.gen_basic import gen_basic

class syn(gen_basic):

  def return_bit_size(self, num):
    bit_size = 1
    num_local = int(num)
    while (True):
      if (num_local >> 1) != 0:
        num_local = num_local >> 1 
        bit_size = bit_size + 1
      else:
        return bit_size

  def ceiling_mem_size(self, size_in):
    size_out = 1
    # use 3/4 of the power-of-2 size to improve BRAM effeciency
    is_triple = 0
    while(size_out < int(size_in)):
      size_out = size_out * 2

    if size_out/4*3 < int(size_in):
      is_triple = 0
      return is_triple, size_out
    else:
      is_triple = 1
      return is_triple, size_out/4*3

  def return_bram18_number(self, size_in, input_num, output_num):
    out= int(size_in)/2048 + int(self.prflow_params['input_port_bram_cost'])*input_num+int(self.prflow_params['output_port_bram_cost'])*output_num + 1
    return out

  def prepare_HW(self, operator):
    # If the map target is Hardware, we need to prepare the HDL files and scripts to compile it.
    self.shell.mkdir(self.syn_dir+'/'+operator+'/src')
    file_list = [ 'Config_Controls.v', 'write_queue.v',        'rise_detect.v',         'read_queue.v',     'converge_ctrl.v',
                  'ExtractCtrl.v',     'Input_Port_Cluster.v', 'Input_Port.v',          'leaf_interface.v', 'Output_Port_Cluster.v',
                  'Output_Port.v',     'read_b_in.v',          'ram0.v',                'single_ram.v',     'SynFIFO.v',
                  'instr_config.v',    'picorv32_wrapper.v',   'picorv32.v',            'picorv_mem.v',     'xram2.v',
                  'xram_triple.v',     'riscv2consumer.v',     'Stream_Flow_Control.v', 'write_b_in.v',     'write_b_out.v',
                  'stream_shell.v']

    # copy the necessary leaf interface verilog files for out-of-context compilation
    for name in file_list: self.shell.cp_file(self.overlay_dir+'/src/'+name, self.syn_dir+'/'+operator+'/src/'+name)

    # prepare the tcl files for out-of-context compilation
    if self.prflow_params['overlay_type'] == 'psnos':
      self.shell.write_lines(self.syn_dir+'/'+operator+'/syn_page.tcl', self.tcl.return_syn_page_tcl_list(operator, ['./leaf.v']))
    elif self.prflow_params['overlay_type'] == 'hipr':
      self.shell.write_lines(self.syn_dir+'/'+operator+'/syn_page.tcl', self.tcl.return_syn_page_tcl_list(operator, [], top_name=operator+'_top', rpt_name='utilization.rpt'))
    else:
      self.shell.write_lines(self.syn_dir+'/'+operator+'/syn_page.tcl', self.tcl.return_syn_page_tcl_list(operator, ['./leaf.v']))


    # prepare the leaf verilog files.
    # Id depends on the IO numbers and operator name

    # extract the stream arguments and types (in/out and width) for all the operators
    operator_arg_dict, operator_width_dict = self.dataflow.return_operator_io_argument_dict(operator)
    in_width_list, out_width_list = self.dataflow.return_io_width(operator_width_dict[operator], operator_arg_dict[operator])
    input_num  = len(in_width_list) 
    output_num = len(out_width_list) 

    # prepare the leaf Verilog file for the DFX page
    if self.prflow_params['overlay_type'] == 'psnoc':
      self.shell.write_lines(self.syn_dir+'/'+operator+'/leaf.v',
                           self.verilog.return_page_v_list(page_num,
                                                           operator,
                                                           input_num,
                                                           output_num,
                                                           operator_arg_dict[operator],
                                                           operator_width_dict[operator],
                                                           True),
                           False)
    elif self.prflow_params['overlay_type'] == 'hipr':
      addr_width_dict = {}
      for i in range(1, 8):  addr_width_dict['Output_'+str(i)] = self.prflow_params['bram_addr_bits']
      print (addr_width_dict)
      for arg in  operator_arg_dict[operator]:
        port_depth_exist, depth = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', arg+'_depth')
        if port_depth_exist: addr_width_dict[arg] = depth
      self.shell.write_lines(self.syn_dir+'/'+operator+'/src/'+operator+'_top.v',
                           self.verilog.return_hipr_page_v_list(operator,
                                                                operator_arg_dict[operator],
                                                                operator_width_dict[operator],
                                                                addr_width_dict),
                           False)
    else:
      print("please specify the correct overlay_type")

 

    # Prepare the shell script to run vivado
    self.shell.write_lines(self.syn_dir+'/'+operator+'/run.sh', self.shell.return_run_sh_list(self.prflow_params['Xilinx_dir'], 'syn_page.tcl', self.prflow_params['back_end']), True)

  def prepare_false(self, operator):
    # If the map target is Hardware, we need to prepare the HDL files and scripts to compile it.
    self.shell.mkdir(self.syn_dir+'/'+operator+'/src')

    # Prepare the shell script to run vivado
    str_list = ['#!/bin/bash -e',
                'touch page_netlist.dcp',
                'echo \'syn: 0 seconds\' > runLog_'+operator+'.log']
    self.shell.write_lines(self.syn_dir+'/'+operator+'/run.sh', str_list, True)



  # create one directory for each page 
  def create_page(self, operator):
    self.shell.re_mkdir(self.syn_dir+'/'+operator)
    map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target')
    page_num_exist,   page_num   =  self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target')
    self.shell.write_lines(self.syn_dir+'/'+operator+'/main.sh', self.shell.return_main_sh_list(
                                                                                                  './run.sh', 
                                                                                                  self.prflow_params['back_end'], 
                                                                                                  'hls_'+operator, 
                                                                                                  'syn_'+operator, 
                                                                                                  self.prflow_params['grid'], 
                                                                                                  'qsub@qsub.com',
                                                                                                  self.prflow_params['mem'], 
                                                                                                  self.prflow_params['node'], 
                                                                                                   ), True)
 
    if map_target == 'HIPR': 
      self.prepare_HW(operator)
    else:
      self.prepare_false(operator)


  def run(self, operator):
    # mk work directory
    self.shell.mkdir(self.syn_dir)
    print (self.syn_dir)
    # create ip directories for the operator
    self.create_page(operator)

     
