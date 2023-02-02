#!/usr/bin/env python
import sys
import os
import xml.etree.ElementTree
import argparse
import re
import math

def return_pragma(file_name, pragma_name, pragma_object=''):
  file_in = open(file_name, 'r')
  src_list = []
  for line in file_in: src_list.append(line.replace('\n',''))
  file_in.close()
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
 
    
def load_prflow_params(filename, benchmark_name):
  prflow_params = {
  }

  # parse the common specifications 
  root = xml.etree.ElementTree.parse(filename).getroot()
  specs = root.findall('spec')
  network = root.findall('network')
  clock =root.findall('clock')

  for child in specs: prflow_params[child.get('name')] = child.get('value')
  for child in clock: prflow_params[child.get('name')] = child.get('period')

  # print (filename.replace('configure/', 'configure/'+prflow_params['board']+'/'))
  board_root = root = xml.etree.ElementTree.parse(filename.replace('configure/', 'configure/'+prflow_params['board']+'/')).getroot()
  specs = board_root.findall('spec')
  for child in specs: prflow_params[child.get('name')] = child.get('value')

  board_exist, board = return_pragma('./input_src/'+benchmark_name+'/host/top.h', 'board')
  if board_exist: prflow_params['board'] = board

  return prflow_params



