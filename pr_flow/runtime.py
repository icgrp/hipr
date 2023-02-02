# -*- coding: utf-8 -*-   

import os  
import subprocess
from pr_flow.gen_basic import gen_basic
import re

class runtime(gen_basic):
  def __init__(self, prflow_params):
    gen_basic.__init__(self, prflow_params)
    self.packet_bits        = int(self.prflow_params['packet_bits'])
    self.addr_bits          = int(self.prflow_params['addr_bits']) 
    self.port_bits          = int(self.prflow_params['port_bits'])
    self.payload_bits       = int(self.prflow_params['payload_bits'])
    self.bram_addr_bits     = int(self.prflow_params['bram_addr_bits'])
    self.freespace          = int(self.prflow_params['freespace'])
    self.page_addr_offset   = self.packet_bits - 1 - self.addr_bits
    self.port_offset        = self.packet_bits - 1 - self.addr_bits - self.port_bits
    self.config_port_offset = self.payload_bits - self.port_bits 
    self.dest_page_offset   = self.payload_bits - self.port_bits - self.addr_bits
    self.dest_port_offset   = self.payload_bits - self.port_bits - self.addr_bits - self.port_bits
    self.src_page_offset    = self.payload_bits - self.port_bits - self.addr_bits
    self.src_port_offset    = self.payload_bits - self.port_bits - self.addr_bits - self.port_bits
    self.freespace_offset   = self.payload_bits - self.port_bits - self.addr_bits - self.port_bits - self.bram_addr_bits - self.bram_addr_bits


  def return_sh_list_local(self, command):
    return ([
      '#!/bin/bash -e',
      command,
      ''])

  
  # prepare the run_app.sh for embedded system
  def gen_sd_run_app_sh(self, operators):
    op_list = operators.split()
    tmp_list = ['#!/bin/bash -e', 'date', './load.exe dynamic_region.xclbin']
    for idx, op in enumerate(op_list):
      if idx == len(op_list)-1: 
        tmp_list.append('./app.exe '+op+'.xclbin')
      else:
        tmp_list.append('./load.exe '+op+'.xclbin')

    self.shell.re_mkdir(self.bit_dir+'/sd_card')
    self.shell.cp_file('input_src/'+self.prflow_params['benchmark_name']+'/'+self.prflow_params['board']+'/xrt.ini', self.bit_dir)
    self.shell.cp_file(self.overlay_dir+'/dynamic_region.xclbin', self.bit_dir+'/sd_card')
    self.shell.cp_file(self.overlay_dir+'/dynamic_region.xclbin', self.bit_dir)
    self.shell.write_lines(self.bit_dir+ '/sd_card/run_app.sh', tmp_list, True)

  # prepare the run_app.sh for Data Center Card 
  def gen_run_app_sh(self, operators):
    self.shell.cp_file('common/script_src/gen_runtime_'+self.prflow_params['board']+'.sh', self.bit_dir+'/run_app.sh')
    tmp_dict = {'Vitis'               : 'source '+self.prflow_params['Xilinx_dir'],
                'Xilinx_dir'          : 'source '+self.prflow_params['Xilinx_dir'],
                'xrt_dir'             : 'source '+self.prflow_params['xrt_dir'],
                'make'                : 'make app.exe\ncp ./app.exe ../../\ncp ./app.exe ../../sd_card\n cp ../../sd_card/dynamic_region.xclbin ../../',
                'XCL_EMULATION_MODE'  : '',
                'PLATFORM_REPO_PATHS' : 'export PLATFORM_REPO_PATHS='+self.prflow_params['PLATFORM_REPO_PATHS'],
                'ROOTFS'              : 'export ROOTFS='+self.prflow_params['ROOTFS'],
                'sdk_dir'             : 'source '+self.prflow_params['sdk_dir']}
    xclbin_list_str = 'dynamic_region.xclbin' 
    for operator in operators.split():
      map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'map_target')
      if map_target == 'HIPR': xclbin_list_str += ' '+operator+'.xclbin'

    self.shell.replace_lines(self.bit_dir+'/run_app.sh', tmp_dict)
    self.shell.replace_lines(self.bit_dir+'/run_app.sh', {'g++': './app.exe '+xclbin_list_str})
    os.system('chmod +x '+ self.bit_dir+'/run_app.sh')

  def gen_runtime_sh(self):
    self.shell.cp_file('common/script_src/gen_runtime_'+self.prflow_params['board']+'.sh', self.bit_dir+'/'+self.prflow_params['benchmark_name']+'/host/gen_runtime.sh')
    tmp_dict = {'Vitis'               : 'source '+self.prflow_params['Xilinx_dir'],
                'Xilinx_dir'          : 'source '+self.prflow_params['Xilinx_dir'],
                'xrt_dir'          : 'source '+self.prflow_params['xrt_dir'],
                'make'                : 'make app.exe\ncp ./app.exe ../../\ncp ./app.exe ../../sd_card\n cp ../../sd_card/dynamic_region.xclbin ../../',
                'cp_cmd'              : 'cp ./app.exe ../../sd_card\ncp ./app.exe ../../\ncp ../../sd_card/dynamic_region.xclbin ../../',
                'XCL_EMULATION_MODE'  : '',
                'PLATFORM_REPO_PATHS' : 'export PLATFORM_REPO_PATHS='+self.prflow_params['PLATFORM_REPO_PATHS'],
                'ROOTFS'              : 'export ROOTFS='+self.prflow_params['ROOTFS'],
                'sdk_dir'             : 'source '+self.prflow_params['sdk_dir']}

    self.shell.replace_lines(self.bit_dir+'/'+self.prflow_params['benchmark_name']+'/host/gen_runtime.sh', tmp_dict)
    os.system('chmod +x '+ self.bit_dir+'/'+self.prflow_params['benchmark_name']+'/host/gen_runtime.sh')

  def return_run_sh_list_local(self):
    cmd_str  = 'cd '+self.prflow_params['benchmark_name'] +'/' + self.prflow_params['board'] + '\n'
    cmd_str += './build.sh \n'
    cmd_str += 'cp ./app.exe ../../sd_card \n'
    cmd_str += 'cp ./app.exe ../../ \n'
    return(self.return_sh_list_local(cmd_str))


  def run(self, operators):
    # mk work directory
    if self.prflow_params['gen_runtime']==True:
      self.shell.mkdir(self.bit_dir)
    
    # prepare the host driver source for vitis runtime
    self.shell.cp_file('input_src/'+self.prflow_params['benchmark_name'], self.bit_dir)
 
    # prepare the gen_runtime.sh to generate the app.exe 
    self.gen_runtime_sh()

    # generate the run_app.sh for embedded platform
    self.gen_sd_run_app_sh(operators)

    # generate the run_app.sh for data center card
    self.gen_run_app_sh(operators)

    # generate the main and run shell to generate the host driver app.exe
    self.shell.write_lines(self.bit_dir+ '/run.sh', self.return_run_sh_list_local(), True)
    self.shell.write_lines(self.bit_dir+ '/main.sh', self.shell.return_main_sh_list('./run.sh', self.prflow_params['back_end']), True)



