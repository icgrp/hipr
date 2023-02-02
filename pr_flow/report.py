#!/usr/bin/env python
import sys
import os
import xml.etree.ElementTree
import argparse
import re
import math
import subprocess
from pr_flow.gen_basic import gen_basic


class report(gen_basic):
  def gen_compile_time_report(self, benchmark_name, operators_list):
    time_report_dict = {}
    time_data_dict = {}
    for fun_name in sorted(operators_list):
      map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'map_target')
      page_exist, page_num = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'page_num')
      time_report_dict[fun_name] = fun_name.ljust(30) + '\t' + str(map_target) + '\t' + str(page_num) 
      time_data_dict[fun_name] = []
      #process hls timing
      try:
        file_name = './workspace/F002_hls_'+benchmark_name + '_' + self.prflow_params['freq'] + '/runLog' + fun_name + '.log'
        file_in = open(file_name, 'r')
        for line in file_in:
          run_time = re.findall(r"\d+", line)
          time_report_dict[fun_name] += '\t' + run_time[0] 
          time_data_dict[fun_name].append(int(run_time[0]))
        file_in.close()
      except:
        print ('Something is wrong with '+file_name) 

      #process syn timing
      try:
        file_name = './workspace/F003_syn_'+benchmark_name+ '_' + self.prflow_params['freq'] +'/' + fun_name + '/runLog_' + fun_name + '.log'
        file_in = open(file_name, 'r')
        for line in file_in:
          #run_time = re.findall(r"\d+", line)
          run_time = line.split()
          time_report_dict[fun_name] += '\t' + run_time[1]
          time_data_dict[fun_name].append(run_time[1])
        file_in.close()
      except:
        print ('Something is wrong with '+file_name) 

      #process impl timing
      run_time_list = []
      try: 
        file_name = './workspace/F004_impl_'+benchmark_name + '_' + self.prflow_params['freq'] + '/'  + fun_name + '/runLogImpl_' + fun_name + '.log'
        file_in = open(file_name, 'r')
        for line in file_in:
          run_time = re.findall(r"\d+", line)
          run_time_list.append(int(run_time[0])) 
          time_data_dict[fun_name].append(int(run_time[0]))
        file_in.close()
        for i in range(6): time_report_dict[fun_name] += '\t' + str(run_time_list[i])
        total_time = 0
        for i in range(8): total_time += float(time_data_dict[fun_name][i])
        run_time_list.append(float(total_time))
        time_report_dict[fun_name] += '\t' + str(run_time_list[6])
        #time_report_dict[fun_name] += '\t\t' + str(run_time_list[5])
      except:
        print ("Something is wrong with "+file_name)

    
    time_report_file = open('./workspace/report/time_report_'+benchmark_name+'.csv', 'w')
    time_report_file.write('operator                  \ttarget\tpage\thls\tsyn\trdchk\topt\tplace\tpopt\troute\tbitgen\ttotal\n')
    for key, value in sorted(time_report_dict.items()):
      time_report_file.write(value+'\n')  
    print ('\n                               operator                  \ttarget\tpage\thls\tsyn\trdchk\topt\tplace\tpopt\troute\tbitgen\ttotal')
    print ('--------------------------------------------------------------------------------------------------------------------------------------------------------------')
    self.print_dict(time_report_dict)

  def gen_resource_report(self, benchmark_name, operators_list):
    resource_report_dict = {}
    LUT_SUM  = 0
    FF_SUM   = 0
    BRAM_SUM = 0
    URAM_SUM = 0
    DSP_SUM  = 0
    for fun_name in operators_list:
      # print(fun_name)
      map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'map_target')
      page_exist, page_num = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'page_num')
      resource_report_dict[fun_name] = fun_name.ljust(30) + '\t' + str(map_target) + '\t' + str(page_num)
      #####################################################################################
      #process resource utilization
      try:
        file_name = './workspace/F003_syn_'+benchmark_name+'_'+self.prflow_params['freq']+'/' + fun_name + '/utilization.rpt'
        file_list = self.shell.file_to_list(file_name)
        for idx, line in enumerate(file_list):
          if self.shell.have_target_string(line, 'Instance'):
            resource_list =  file_list[idx+2].replace(' ', '').split('|')
            resource_report_dict[fun_name] += '\t' + resource_list[3]
            LUT_SUM                        += int(resource_list[3])
            resource_report_dict[fun_name] += '\t' + resource_list[7]
            FF_SUM                         += int(resource_list[7])
            bram_num = int(resource_list[8])*2+int(resource_list[9])
            resource_report_dict[fun_name] += '\t' + str(bram_num)
            BRAM_SUM                       += int(bram_num)
            resource_report_dict[fun_name] += '\t' + resource_list[10]
            URAM_SUM                       += int(resource_list[10])
            resource_report_dict[fun_name] += '\t' + resource_list[11]
            DSP_SUM                       += int(resource_list[11])
      except:
        print ('Something is wrong with '+file_name) 



    list_line = 'total                                                           HIPR\t0\t'+str(LUT_SUM)+'\t'+str(FF_SUM)+'\t'+str(BRAM_SUM)+'\t'+str(URAM_SUM)+'\t'+str(DSP_SUM)
    resource_report_file = open('./workspace/report/resource_report_'+benchmark_name+'.csv', 'w')
    resource_report_file.write('operator                  \ttarget\tpage\tLUTs\tFFs\tBRAM18s\tURAM\tDSPs\n')
    for key, value in sorted(resource_report_dict.items()):
      resource_report_file.write(value+'\n')  
    print ('\n                               operator                  \ttarget\tpage\tLUTs\tFFs\tBRAM18s\tURAM\tDSPs')
    print ('------------------------------------------------------------------------------------------------------------------')
    self.print_dict(resource_report_dict)
    print(list_line)

  def gen_timing_report(self, benchmark_name, operators_list):
    timing_report_dict = {}
    for fun_name in operators_list:
      map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'map_target')
      page_exist, page_num = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'page_num')
      timing_report_dict[fun_name] = fun_name.ljust(30) + '\t' + str(map_target) + '\t' + str(page_num)
      #####################################################################################
      #process timing report
      try:
        file_name = './workspace/F004_impl_'+benchmark_name + '_' + self.prflow_params['freq'] +'/' + fun_name + '/timing_page.rpt'
        file_in = open(file_name, 'r')
        find_summary_flag = False
        line_offset = 0
        for line in file_in:
          if line.replace('Design Timing Summary', '') != line:
            find_summary_flag = True
          if find_summary_flag:
            line_offset += 1
          if line_offset == 7:
            timing_list =  line.split()
            timing_report_dict[fun_name] += '\t' + timing_list[0]
        file_in.close()
      except:
        print ('Something is wrong with '+file_name) 



    resource_report_file = open('./workspace/report/timing_report_'+benchmark_name+'.csv', 'w')
    resource_report_file.write('operator                  \ttarget\tpage\tslack\n')
    for key, value in sorted(timing_report_dict.items()):
      resource_report_file.write(value+'\n')  
    print ('\n                               operator                  \ttarget\tpage\tslack')
    print ('-------------------------------------------------------------------------------------')
    self.print_dict(timing_report_dict)

  def gen_pragma_report(self, benchmark_name, operators_list):
    pragma_dict = {}
    for fun_name in operators_list:
      # print(fun_name)
      map_target_exist, map_target = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'map_target')
      page_exist,       page_num   = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'page_num')
      clb_exist,        clb_num    = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'clb')
      ff_exist,         ff_num     = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'ff')
      bram_exist,       bram_num   = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'bram')
      dsp_exist,        dsp_num    = self.pragma.return_pragma('./input_src/'+self.prflow_params['benchmark_name']+'/operators/'+fun_name+'.h', 'dsp')

      pragma_dict[fun_name] = fun_name.ljust(30) + '\t' + str(map_target) + '\t' + str(page_num)
      if clb_exist : pragma_dict[fun_name] += '\t'+str(clb_num) 
      else         : pragma_dict[fun_name] += '\tx' 
      if ff_exist  : pragma_dict[fun_name] += '\t'+str(ff_num) 
      else         : pragma_dict[fun_name] += '\tx' 
      if bram_exist: pragma_dict[fun_name] += '\t'+str(bram_num) 
      else         : pragma_dict[fun_name] += '\tx' 
      if dsp_exist : pragma_dict[fun_name] += '\t'+str(dsp_num) 
      else         : pragma_dict[fun_name] += '\tx' 



    pragma_report_file = open('./workspace/report/pragma_report_'+benchmark_name+'.csv', 'w')
    pragma_report_file.write('operator                  \ttarget\tpage\tLUTs\tFFs\tBRAM18s\tDSPs\n')
    for key, value in sorted(pragma_dict.items()):
      pragma_report_file.write(value+'\n')  
    print ('\n                               operator                  \ttarget\tpage\tLUTs\tFFs\tBRAM18s\tDSPs')
    print ('------------------------------------------------------------------------------------------------------------------')
    self.print_dict(pragma_dict)



  def grab_runtime(self):
    try:
      file_name = self.bit_dir+'/summary.csv'
      file_in = open(file_name, 'r')
      find_summary_flag = False
      line_offset = 0
      read_start = 0.0
      write_end = 0.0
      unit = 's'
      for line in file_in:
        if (self.shell.have_target_string(line, 'mem_read2') and self.shell.have_target_string(line, 'dma')):
          words_list=line.split(',')
          print(words_list)
          read_start = float(words_list[5])
        if (self.shell.have_target_string(line, 'mem_write2') and self.shell.have_target_string(line, 'dma')):
          words_list=line.split(',')
          print(words_list)
          write_end = float(words_list[5])+float(words_list[6])
        if (self.shell.have_target_string(line, 'Kernel Instance Address')):
          words_list=line.split(',')
          unit = words_list[5].split('(')[1].split(')')[0]
        
      file_in.close()
      print("read_start: "+str(read_start)+' '+unit)
      print(" write_end: "+str(write_end)+' '+unit)
      print("  duration: "+str(round(write_end-read_start, 3))+' '+unit)
      print(" bandwidth: "+str(round(1024000*512/(write_end-read_start)/1000/8, 3))+" MB/s")
    except:
      print ('Something is wrong with '+file_name) 


    
 
  def run(self, operators_str):

    self.shell.mkdir(self.rpt_dir)
    benchmark_name = self.prflow_params['benchmark_name']
    operators_list = operators_str.split() 
    self.gen_pragma_report     (benchmark_name, operators_list)
    self.gen_timing_report       (benchmark_name, operators_list)
    self.grab_runtime()
    self.gen_compile_time_report (benchmark_name, operators_list)
    self.gen_resource_report     (benchmark_name, operators_list)
    print ('You can find the comile time report and resource report under: ./workspace/report')

    


