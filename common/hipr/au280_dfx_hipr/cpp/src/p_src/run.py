#!/usr/bin/env python
from cProfile import run
import os
import matplotlib
import matplotlib.pyplot as plt
import argparse

import my_cmd
import my_plot


if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('-t', '--run_t',  help="default: plot the fiture with differnt temperature", action='store_true')
  parser.add_argument('-n', '--name', type=str, help="default: plot the fiture with differnt temperature", default='sb')

  

  option_dict = {}
  args = parser.parse_args()
  # benchmark_name = args.benchmark_name  
  option_dict['run_t']        = args.run_t

  para_dict = {'trial_num': '1000000',
               't_init'   : '1000.0',
               'my_seed'  : '199',
               't_min'    : '1e-5',
               'app_path' : './src/app/digit_reg512',
               'device'   : 'zcu102',
               'out_log'  : 'out_xy.log',
               'opt'      : 'XYSA',
               'seed_num' : 20}

  cmd_inst = my_cmd.my_cmd(para_dict)
  plt_inst = my_plot.my_plot(para_dict)

  t_init_list = [10000.0, 1000.0, 100.0, 10.0, 1.0, 0.1, 0.01, 0.001, 0.0001]
  # t_init_list = [10000.0, 1000.0, 100.0, 10.0, 1.0, 0.1, 0.01, 0.001]
  # trial_num_list = [1000000000, 100000000,10000000, 1000000, 100000, 10000, 1000]
  trial_num_sq_list = [          1000000, 100000, 10000, 8000, 6000, 4000, 3000, 2000, 1000]
  trial_num_xy_list = [10000000, 1000000, 100000, 10000, 8000, 6000, 4000, 3000, 2000, 1000]
  seed_list = range(para_dict['seed_num'])

  # sweep trial number
  for seed_val in seed_list:
    for trial_num in trial_num_xy_list:
      cmd_inst.my_seed   = str(seed_val)
      cmd_inst.trial_num = str(trial_num)
      cmd_inst.opt       = 'XYSA'
      cmd_inst.out_log   = 'output/output_xysa_log_seed_' +str(seed_val)+'_trial_num_'+str(trial_num)+'.log'
      cmd_inst.run()
    for trial_num in trial_num_sq_list:
      cmd_inst.my_seed   = str(seed_val)
      cmd_inst.trial_num = str(trial_num)
      cmd_inst.opt       = 'SQSA'
      cmd_inst.out_log   = 'output/output_sqsa_log_seed_' +str(seed_val)+'_trial_num_'+str(trial_num)+'.log'
      cmd_inst.run()
  # plt_inst.plot_cost_vs_trial_num(trial_num_list)
  # plt_inst.plot_cost_vs_runtime(trial_num_list, trial_num_xy_list, 'trial_num')
  
  # sweep t_initial number
  cmd_inst.reset(para_dict)
  for seed_val in seed_list:
    for t_init_val in t_init_list:
      cmd_inst.my_seed   = str(seed_val)
      cmd_inst.t_init    = str(t_init_val)
      cmd_inst.out_log   = 'output/output_xy_log_seed_' +str(seed_val)+'_t_init_'+str(t_init_val)+'.log'
      # cmd_inst.run()
  # plt_inst.plot_cost_vs_t_init(t_init_list)
  # plt_inst.plot_cost_vs_runtime(t_init_list, 't_init')


  
  
  

  


  
 





