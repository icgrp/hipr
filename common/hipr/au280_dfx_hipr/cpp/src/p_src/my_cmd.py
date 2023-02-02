#!/usr/bin/env python
import os



class my_cmd():
  def __init__(self, para_dict):
    self.trial_num = para_dict['trial_num']
    self.t_init    = para_dict['t_init'] 
    self.my_seed   = para_dict['my_seed']
    self.t_min     = para_dict['t_min']
    self.app_path  = para_dict['app_path']
    self.device    = para_dict['device']
    self.out_log   = para_dict['out_log']
    self.seed_num  = para_dict['seed_num']
    self.opt       = para_dict['opt']
  
  def reset(self, para_dict):
    self.trial_num = para_dict['trial_num']
    self.t_init    = para_dict['t_init'] 
    self.my_seed   = para_dict['my_seed']
    self.t_min     = para_dict['t_min']
    self.app_path  = para_dict['app_path']
    self.device    = para_dict['device']
    self.out_log   = para_dict['out_log']
    self.seed_num  = para_dict['seed_num']
    self.opt       = para_dict['opt']

  def run(self):
    os.system("make main")
    os.system('date > out.log')
    cmdStr = './main -TRIAL_NUM ' + self.trial_num \
               + ' -T_init '+self.t_init \
               + ' -MY_SEED '+self.my_seed\
               + ' -T_MIN '+self.t_min\
               + ' -P '+self.app_path\
               + ' -DEVICE '+self.device\
               + ' -OPT '+self.opt\
               + ' -OL '+self.out_log + ' >> out.log'
    print(cmdStr)
    os.system(cmdStr)





