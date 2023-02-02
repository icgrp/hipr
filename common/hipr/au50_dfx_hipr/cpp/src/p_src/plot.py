#!/usr/bin/env python
import matplotlib
import matplotlib.pyplot as plt
import os
import numpy as np
import math
import os
import argparse
from _file import _file

class plot_figure(_file):
  def parse_sa_log(self, log_path):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table(log_path, ' ')
    for line_list in log_table:
      if line_list[0] == 'Time:':
        time_list.append(float(line_list[1] ))
        temp_list.append(float(line_list[3] ))
        cost_list.append(float(line_list[14]))
    
    return time_list, temp_list, cost_list

  def parse_milp_log(self, log_path):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table(log_path)
    start_row = len(log_table)
    for idx, line_list in enumerate(log_table):
      if len(line_list) > 0 and line_list[0] == 'Expl': start_row = idx+2
      if idx >= start_row:
        if len(line_list) == 0: break
        if line_list[-5] != '-': 
          time_list.append(float(line_list[-1].replace('s', '')))
          cost_list.append(float(line_list[-5]))
        
    
    return time_list, cost_list

  def plot_xysa_milp_t(self, app_name):
    time_list, temp_list, cost_list = self.parse_sa_log('xysa_'+app_name+'_out.log')
    time_list_milp, cost_list_milp  = self.parse_milp_log('milp_'+app_name+'_out.log')
    time_list.append(time_list_milp[-1])
    cost_list.append(cost_list[-1])
    f, ax = plt.subplots(1, 1, figsize=(5, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    ax.plot(time_list, cost_list)
    ax.plot(time_list_milp, cost_list_milp)
    
    ax.legend(['XYSA', 'MILP'])
    #ax[1].legend(legend_list)
    ax.set_ylim([0.005, 100])
    #ax[1].set_xlim([1e-12, 1e5])
    #ax[1].invert_xaxis()
    ax.set_yscale("log")
    ax.set_xscale("log")
    ax.set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    ax.set_ylabel('Cost Function')
    # ax[0].set_title(str(size)+' Samples per Digit')
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    plt.show()
    # plt.savefig(out_dir+"/xysa_milp_t.pdf", bbox_inches='tight')




if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('-r', '--report', action='store_false', help="Generate Report"          )
  parser.add_argument('-q', '--qsub'  , action='store_true' , help="Re-run  compile with qsub")
  parser.add_argument('-g', '--graph' , action='store_true' , help="Generate Graph"           )
  parser.add_argument('-c', '--clean' , action='store_true' , help="Generate Graph"           )
  parser.add_argument('-a', '--app'   , type=str            , help="default: freqence"        ,default='rendering')

 
  args       = parser.parse_args()
  app_name   = args.app

  p = plot_figure()
  p.plot_xysa_milp_t(app_name)
  
