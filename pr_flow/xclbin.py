# -*- coding: utf-8 -*-   

import os  
import subprocess
from pr_flow.gen_basic import gen_basic
import re
import pr_flow.syn 

class xclbin(gen_basic):
  
  # create one directory for each page 
  def create_page(self, operator, page_num):
    self.shell.cp_file('common/script_src/gen_xclbin_'+self.prflow_params['board']+'.sh', self.bit_dir+'/run_'+operator+'.sh')
    tmp_dict = {'bitstream=' : 'bitstream='+operator+'.bit',
                'xmlfile='   : 'xmlfile='+operator+'.xml',
                'source'     : 'source '+self.prflow_params['Xilinx_dir'],
                'xclbin='    : 'xclbin='+operator+'.xclbin',
                'workspace=' : 'workspace=../F000_ydma_'+self.prflow_params['freq']}
    self.shell.replace_lines(self.bit_dir+'/run_'+operator+'.sh', tmp_dict)
    tmp_dict = {'./ydma/' : '../F001_overlay_'+self.prflow_params['benchmark_name']+'_'+self.prflow_params['freq']+'/ydma/'}
    self.shell.my_sed(self.bit_dir+'/run_'+operator+'.sh', tmp_dict)



    self.shell.cp_file('common/metadata/'+self.prflow_params['board']+'_'+self.prflow_params['freq']+'/ydma.xml', self.bit_dir+'/'+operator+'.xml')
    os.system('chmod +x '+self.bit_dir+'/run_'+operator+'.sh')
    self.shell.write_lines(self.bit_dir+'/main_'+operator+'.sh', self.shell.return_main_sh_list(
                                                                                                  './run_'+operator+'.sh', 
                                                                                                  self.prflow_params['back_end'], 
                                                                                                  'impl_'+operator, 
                                                                                                  'xclbin_'+operator, 
                                                                                                  self.prflow_params['grid'], 
                                                                                                  'qsub@qsub.com',
                                                                                                  self.prflow_params['mem'], 
                                                                                                  self.prflow_params['node'], 
                                                                                                   ), True)

  def create_shell_file(self):
  # local run:
  #   main.sh <- |_ execute each impl_page.tcl
  #
  # qsub run:
  #   qsub_main.sh <-|_ Qsubmit each qsub_run.sh <- impl_page.tcl
    pass   

  def run(self, operator):
    # mk work directory
    if self.prflow_params['gen_xclbin']==True:
      self.shell.mkdir(self.bit_dir)
    
    # create ip directories for all the pages
    page_num_exist, page_num = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+operator+'.h', 'page_num') 
    if page_num_exist==True:
      self.create_page(operator, page_num)

