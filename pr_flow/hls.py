# -*- coding: utf-8 -*-   
# Company: IC group, University of Pennsylvania
# Contributor: Yuanlong Xiao
#
# Create Date: 02/23/2021
# Design Name: overlay
# Project Name: PLD
# Versions: 1.0
# Description: This is a python script to prepare the script for High Level Synthesis 
#              for PRflow.
# Dependencies: python, gen_basic.py hls.py
# Revision:
# Revision 0.01 - File Created
# Revision 0.02 - Update cotents for HiPR
#
# Additional Comments:



import os  
import subprocess
from pr_flow.gen_basic import gen_basic



class hls(gen_basic):
 
  def get_file_name(self, hls_path):                                            
    for root, dirs, files in os.walk(hls_path):                                 
      return files  

  # create one directory for each page 
  def create_page(self, fun_name, hls_path, src_path, syn_tcl_file, isOverlay):
    map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'map_target')
    self.shell.re_mkdir(hls_path+'/'+fun_name+'_prj')
    self.shell.re_mkdir(hls_path+'/'+fun_name+'_prj/'+fun_name)
    self.shell.write_lines(hls_path+'/main_'+fun_name+'.sh', self.shell.return_main_sh_list(
                                                                                                  './run_'+fun_name+'.sh', 
                                                                                                  self.prflow_params['back_end'], 
                                                                                                  'NONE', 
                                                                                                  'hls_'+fun_name+'_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq'], 
                                                                                                  self.prflow_params['grid'], 
                                                                                                  'qsub@qsub.com',
                                                                                                  self.prflow_params['mem'], 
                                                                                                  self.prflow_params['node'], 
                                                                                                   ), True)
        
    self.shell.write_lines(hls_path+'/'+fun_name+'_prj/hls.app', self.tcl.return_hls_prj_list(fun_name))
    self.shell.write_lines(hls_path+'/'+fun_name+'_prj/'+fun_name+'/script.tcl', self.tcl.return_hls_tcl_list(fun_name, src_path))
    if map_target == 'HW' or map_target == 'HIPR' or isOverlay:
      # if the map target is Hardware, we need to compile the c code through vivado_hls 
      self.shell.write_lines(hls_path+'/run_'+fun_name+'.sh', self.shell.return_run_hls_sh_list(self.prflow_params['Xilinx_dir'], './'+fun_name+'_prj/'+fun_name+'/script.tcl', syn_tcl_file, self.prflow_params['back_end']), True)
    else:
      # if the map target is riscv, we can still generate a psuedo shell script and generate the runLog<operator>.log for Makefile to process the rest flow
      self.shell.write_lines(hls_path+'/run_'+fun_name+'.sh', self.shell.return_empty_sh_list(), True)
      self.shell.write_lines(hls_path+'/runLog'+fun_name+'.log', ['hls: 0 senconds'], False)

  def run(self, operator, path=None, src_path='../..', syn_tcl_file=[], isOverlay=False):
    if path == None:
      hls_path = self.hls_dir
    else:
      hls_path = path
    # mk work directory
    self.shell.mkdir(hls_path)
    
    # create ip directories for all the pages
    self.create_page(operator, hls_path, src_path, syn_tcl_file, isOverlay)
    
 



