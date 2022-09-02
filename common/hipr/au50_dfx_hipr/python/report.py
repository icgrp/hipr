#!/usr/bin/env python
import sys
import os
import argparse
import re
import math
import subprocess

class report():

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
      
  def gen_resource_report(self, rpt_path, operators_list, app_name):
    resource_report_dict = {}

    for fun_name in operators_list:
      resource_report_dict[fun_name] = fun_name.ljust(30)
      try:
        file_name = rpt_path+'/utilization_'+fun_name+'.rpt'

        file_list = self.file_to_list(file_name)
        for idx, line in enumerate(file_list):
          if self.have_target_string(line, 'Instance'):
            resource_list =  file_list[idx+2].replace(' ', '').split('|')
            resource_report_dict[fun_name] += 'hipr  '
            resource_report_dict[fun_name] += '\t' + resource_list[3]
            resource_report_dict[fun_name] += '\t' + resource_list[7]
            bram_num = int(resource_list[8])*2+int(resource_list[9])+10
            resource_report_dict[fun_name] += '\t' + str(bram_num)
            resource_report_dict[fun_name] += '\t' + resource_list[11]
      except:
        print ('Something is wrong with '+file_name) 



    resource_report_file = open('./cpp/src/app/'+app_name+'/dfx.txt', 'w')
    #resource_report_file.write('operator                  \tLUTs\tFFs\tBRAM18s\tDSPs\n')
    #resource_report_file.write(str(len(resource_report_dict))+'\n')
   
    for key, value in sorted(resource_report_dict.items()):
      resource_report_file.write(value+'\n')  

    print ('\n                               operator                  \tLUTs\tFFs\tBRAM18s\tDSPs')
    print ('--------------------------------------------------------------------------------------------')
    self.print_dict(resource_report_dict)

 
  def run(self, rpt_path, operators_str, app_name):
    operators_list = operators_str.split() 
    self.gen_resource_report(rpt_path, operators_list, app_name)


if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('rpt_path')
  parser.add_argument('-op',        '--operators',        help="choose which function to be regenrated",    type=str,                default="no_func"  )
  parser.add_argument('-app',       '--app_name',         help="choose which function to be regenrated",    type=str,                default="synthetic"  )

  args           = parser.parse_args()
  rpt_path       = args.rpt_path  
  operators_str  = args.operators 
  app_name       = args.app_name 
 
  rpt_inst = report()
  rpt_inst.run(rpt_path, operators_str, app_name)


    


