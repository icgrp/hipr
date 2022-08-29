#!/usr/bin/env python
import sys
import os
import argparse
import re
import math
import subprocess

class report():
  def __init__(self, benchmark_name):
    self.benchmark_name = benchmark_name

  def print_dict(self, in_dict):
    for key, value in sorted(in_dict.items()):
      print (str(key).ljust(30)+'->'+str(value))

  def file_to_list(self, file_name):
    file_list = []
    file_in = open(file_name, 'r')
    for line in file_in:
      file_list.append(line.replace('\n',''))
    return file_list

  def have_target_string(self, string_in, target_string):
    if string_in.replace(target_string, '') == string_in:
      return False
    else:
      return True

  def return_pragma(self, file_name, pragma_name):
    src_list = self.file_to_list(file_name)
    if_exist = False
    value = 0
    for line in src_list:
      if(line.replace("#pragma", "") != line):
        line_list = line.replace('=', ' ').split()
        if(line_list[0].startswith('//')):
          continue
        for idx, ele in enumerate(line_list):
          if(ele == pragma_name):
            if_exist = True
            value = line_list[idx+1]
    return if_exist, value 
 
  def return_hipr_list(self, operators): 
    out_list = []
    for operator in operators:
      target_exist, target = self.return_pragma('../../../../../input_src/'+self.benchmark_name+'/operators/'+operator+'.h', 'map_target') 
      if target_exist and target == 'HIPR': out_list.append(operator)
    return out_list

      
  def gen_resource_report(self, rpt_path, operators_list):
    resource_report_dict = {}

    for fun_name in operators_list:
      resource_report_dict[fun_name] = fun_name.ljust(30)
      target_exist, target = self.return_pragma('../../../../../input_src/'+self.benchmark_name+'/operators/'+fun_name+'.h', 'map_target') 
      print (fun_name, target_exist, target)
      if target_exist and target == 'HIPR':
        resource_report_dict[fun_name] += 'hipr'
      else:
        resource_report_dict[fun_name] += 'nopr'
 
      try:
        file_name = rpt_path+'/utilization_'+fun_name+'.rpt'

        file_list = self.file_to_list(file_name)
        for idx, line in enumerate(file_list):
          if self.have_target_string(line, 'Instance'):
            resource_list =  file_list[idx+2].replace(' ', '').split('|')
            resource_report_dict[fun_name] += '\t' + resource_list[3]
            resource_report_dict[fun_name] += '\t' + resource_list[7]
            bram_num = int(resource_list[8])*2+int(resource_list[9])+4
            resource_report_dict[fun_name] += '\t' + str(bram_num)
            resource_report_dict[fun_name] += '\t' + resource_list[11]
      except:
        print ('Something is wrong with '+file_name) 



    resource_report_file = open('./cpp/src/app/dfx.txt', 'w')
    #resource_report_file.write('operator                  \tLUTs\tFFs\tBRAM18s\tDSPs\n')
   
    for key, value in sorted(resource_report_dict.items()):
      resource_report_file.write(value+'\n')  

    print ('\n                               operator                       type\tLUTs\tFFs\tBRAM18s\tDSPs')
    print ('----------------------------------------------------------------------------------------------------')
    self.print_dict(resource_report_dict)

 
  def run(self, rpt_path, operators_str):
    operators_list = operators_str.split() 
    self.gen_resource_report(rpt_path, operators_list)
    print(self.return_hipr_list(operators_list))

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('rpt_path')
  parser.add_argument('-op',        '--operators',        help="choose which function to be regenrated",    type=str,                default="no_func"  )
  parser.add_argument('-app',       '--benchmark_name',   help="choose which function to be regenrated",    type=str,                default="digit_reg512")

  args           = parser.parse_args()
  rpt_path       = args.rpt_path  
  operators_str  = args.operators 
  benchmark_name = args.benchmark_name 
 
  rpt_inst = report(benchmark_name)
  rpt_inst.run(rpt_path, operators_str)


    


