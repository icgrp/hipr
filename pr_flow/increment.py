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

class increment(gen_basic):
  def __init__(self, prflow_params):
    gen_basic.__init__(self, prflow_params)

  # run.sh will be used for generating the increment.xclbin 
  def return_run_sh_list_local(self):
    lines_list = []
    lines_list.append('#!/bin/bash -e')

    # launch the vitis compilation for increment kernel
    lines_list.append('cd increment/'+self.prflow_params['board'])
    lines_list.append('./build.sh')
    lines_list.append('cd ../../')
    lines_list.append('touch __increment_is_ready__')
    
    return lines_list

  def update_cad_path(self):
    # update the cad path for build.sh
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'export ROOTFS'      : 'export ROOTFS='+self.prflow_params['ROOTFS']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'export PLATFORM_REPO_PATHS=': 'export PLATFORM_REPO_PATHS='+self.prflow_params['PLATFORM_REPO_PATHS']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'export PLATFORM='   : 'export PLATFORM='+self.prflow_params['PLATFORM']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'xrt_dir'            : 'source '+self.prflow_params['xrt_dir']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'sdk_dir'            : 'source '+self.prflow_params['sdk_dir']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'Xilinx_dir'         : 'source '+self.prflow_params['Xilinx_dir']})
    self.shell.replace_lines(self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh', {'freq='              : 'freq='+self.prflow_params['freq'].replace('M', '')})
    os.system('chmod +x '+self.increment_dir+'/increment/'+self.prflow_params['board']+'/build.sh')

    # replace device definistion in cfg file
    self.shell.replace_lines(self.increment_dir+'/increment/src/'+self.prflow_params['board']+'_dfx.cfg', {'platform'         : 'platform='+self.prflow_params['PLATFORM']})

 
  def create_shell_file(self):
    # generate the shell script to generate the overlay
    self.shell.write_lines(self.increment_dir+'/run.sh', self.return_run_sh_list_local(), True)
    
    # generate the shell script to call run.sh depends on the scheduler.
    # scheduler: slurm, qsub, local 
    self.shell.write_lines(self.increment_dir+'/main.sh', self.shell.return_main_sh_list(\
                                                       './run.sh', \
                                                       self.prflow_params['back_end'], \
                                                       'NONE',\
                                                       'increment', \
                                                       self.prflow_params['grid'],  \
                                                       self.prflow_params['email'], \
                                                       self.prflow_params['mem'],  \
                                                       self.prflow_params['maxThreads']), True)
 

  def run(self):
    # make work directory
    self.shell.mkdir(self.prflow_params['workspace'])
    self.shell.del_dir(self.increment_dir)
    self.shell.re_mkdir(self.increment_dir)

    # copy the initial source files for vitis compile
    self.shell.cp_dir('./common/increment', self.increment_dir)
  
    # update the cad tool path
    self.update_cad_path()
    
    # update shell script
    self.create_shell_file()



