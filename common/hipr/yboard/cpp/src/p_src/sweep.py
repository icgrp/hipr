#!/usr/bin/env python
import os
from _file import _file
import matplotlib
import matplotlib.pyplot as plt
out_dir = './'

class sweep(_file):

  def sweep_T(self, T_list):
    os.system("make main")
    for T in T_list: os.system('./main -TRIAL_NUM 100000 -T_init '+str(T)+' -OL '+' out_T'+str(T)+'.log'+' -POS '+'position_T'+str(T)+'.txt')

  def run_compare_SQSA_XYSA(self):
    os.system("make main")
    os.system('./main -A digit_reg_dyn__4480_20_2 -OPT SQSA -TRIAL_NUM 100000 -T_init 100 -OL out_SQSA.log -POS position_SQSA.txt')
    os.system('./main -A digit_reg_dyn__4480_20_2 -OPT XYSA -TRIAL_NUM 100000 -T_init 100 -OL out_XYSA.log -POS position_XYSA.txt')
    

  def run_sweep_SQSA_XYSA(self):
    for r in range(50, 81, 10): os.system('./main -A b'+str(r)+' -OPT XYSA -TRIAL_NUM 100000 -T_init 100 -OL out_XYSA_b'+str(r)+'.log -POS position_XYSA_b'+str(r)+'.txt')
    for r in range(50, 81, 10): os.system('./main -A b'+str(r)+' -OPT SQSA -TRIAL_NUM 100000 -T_init 100 -OL out_SQSA_b'+str(r)+'.log -POS position_SQSA_b'+str(r)+'.txt')

  def sweep_num(self, num_list):
    os.system("make main")
    for num in num_list:
      print('./main -TRIAL_NUM '+str(num)+' -T_init 100 out_num'+str(num)+'.log'+' -POS '+'position_num'+str(num)+'.txt')
      os.system('./main -TRIAL_NUM '+str(num)+' -T_init 100 -OL out_num'+str(num)+'.log'+' -POS '+'position_num'+str(num)+'.txt')


  def parse_t_log(self, T):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table('out_T'+str(T)+'.log', ' ')
    for line_list in log_table:
      if line_list[0] == 'Time:':
        time_list.append(float(line_list[1] ))
        temp_list.append(float(line_list[3] ))
        cost_list.append(float(line_list[14]))
    
    return time_list, temp_list, cost_list

  def parse_num_log(self, num):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table('out_num'+str(num)+'.log', ' ')
    for line_list in log_table:
      if line_list[0] == 'Time:':
        time_list.append(float(line_list[1] ))
        temp_list.append(float(line_list[3] ))
        cost_list.append(float(line_list[14]))
    
    return time_list, temp_list, cost_list

  def parse_log(self, log_name):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table(log_name, ' ')
    self.print_list(log_table)
    for line_list in log_table:
      if line_list[0] == 'Time:':
        time_list.append(float(line_list[1] ))
        temp_list.append(float(line_list[3] ))
        cost_list.append(float(line_list[14]))
    
    return time_list, temp_list, cost_list


  def plot_cost_t(self, T_list):
    # f, ax = plt.subplots(len(T_list), 2, figsize=(10, 5*len(T_list)))
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    for t_idx, T in enumerate(T_list): 
      time_list, temp_list, cost_list = self.parse_t_log(T)
      legend_list.append("T="+str(T))
      ax[0].plot(time_list, cost_list)
      ax[1].plot(temp_list, cost_list)
    
    ax[0].legend(legend_list)
    ax[1].legend(legend_list)
    ax[0].set_ylim([-5, 100])
    ax[1].set_xlim([1e-12, 1e5])
    ax[1].invert_xaxis()
    ax[1].set_xscale("log")
    ax[0].set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    ax[1].set_xlabel('Temperature\n(b) Cost Function Changes with Temperature')
    ax[0].set_ylabel('Cost Function')
    ax[1].set_ylabel('Cost Function')
    # ax[0].set_title(str(size)+' Samples per Digit')
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    # plt.show()
    plt.savefig(out_dir+"/sweep_T.pdf", bbox_inches='tight')

  def plot_cost_num(self, num_list):
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    for t_idx, num in enumerate(num_list): 
      time_list, temp_list, cost_list = self.parse_num_log(num)
      legend_list.append("trial_num="+str(num))
      ax[0].plot(time_list, cost_list)
      ax[1].plot(temp_list, cost_list)
    
    ax[0].legend(legend_list)
    ax[1].legend(legend_list)
    #ax[0].set_ylim([-5, 100])
    ax[1].set_xlim([1e-12, 1e3])
    ax[1].invert_xaxis()
    ax[0].set_xscale("log")
    ax[1].set_xscale("log")
    ax[0].set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    ax[1].set_xlabel('Temperature\n(b) Cost Function Changes with Temperature')
    ax[0].set_ylabel('Cost Function')
    ax[1].set_ylabel('Cost Function')
    # ax[0].set_title(str(size)+' Samples per Digit')
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    # plt.show()
    plt.savefig(out_dir+"/sweep_num.pdf", bbox_inches='tight')

  def plot_optimal_t(self, T_list):
    f, ax = plt.subplots(1, 1, figsize=(5, 5))
    plt.rcParams.update({'font.size': 10})
    optimal_list = []
    for t_idx, T in enumerate(T_list): 
      time_list, temp_list, cost_list = self.parse_t_log(T)
      optimal_list.append(cost_list[-1])

    print(optimal_list)
    print(T_list)
    ax.plot(T_list, optimal_list, marker='*')
    ax.set_xscale("log")
    ax.set_xlabel('Initial Temperature')
    ax.set_ylabel('Final Cost Function')
    # plt.show()
    plt.savefig(out_dir+"/optimal_T.pdf", bbox_inches='tight')

  def plot_optimal_num(self, num_list):
    f, ax = plt.subplots(1, 1, figsize=(5, 5))
    plt.rcParams.update({'font.size': 10})
    optimal_list = []
    for t_idx, num in enumerate(num_list): 
      time_list, temp_list, cost_list = self.parse_num_log(num)
      optimal_list.append(cost_list[-1])

    ax.plot(num_list, optimal_list, marker='*')
    ax.set_xscale("log")
    ax.set_xlabel('Trial Number per 10$\times$ Scale')
    ax.set_ylabel('Final Cost Function')
    # plt.show()
    plt.savefig(out_dir+"/optimal_num.pdf", bbox_inches='tight')

  def plot_compare_SQSA_XYSA(self):
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    time_list, temp_list, cost_list  = self.parse_log('out_SQSA.log')
    print(time_list)
    ax[0].plot(time_list, cost_list)
    ax[1].plot(temp_list, cost_list)
    time_list, temp_list, cost_list  = self.parse_log('out_XYSA.log')
    ax[0].plot(time_list, cost_list)
    ax[1].plot(temp_list, cost_list)

    ax[0].legend(['SQSA', 'XYSA'])
    ax[1].legend(['SQSA', 'XYSA'])

    ax[0].set_xscale("log")
    ax[1].set_xscale("log")
    ax[0].set_yscale("log")
    ax[1].set_yscale("log")
    #ax[0].set_ylim([-5, 20])
    #ax[1].set_xlim([1e-0, 1e3])
    ax[1].invert_xaxis()

    ax[0].set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    ax[1].set_xlabel('Temperature\n(b) Cost Function Changes with Temperature')
    ax[0].set_ylabel('Cost Function')
    ax[1].set_ylabel('Cost Function')
    plt.savefig(out_dir+"/SQSA_XYSA.pdf", bbox_inches='tight')

  def plot_sweep_SQSA_XYSA(self):
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    for r in range(50, 81, 10):
      time_list, temp_list, cost_list  = self.parse_log('out_SQSA_b'+str(r)+'.log')
      time_list, temp_list, cost_list  = self.parse_log('out_XYSA_b'+str(r)+'.log')
      

    # ax[0].legend(['SQSA', 'XYSA'])
    # ax[1].legend(['SQSA', 'XYSA'])

    # ax[0].set_xscale("log")
    # ax[1].set_xscale("log")
    # ax[0].set_yscale("log")
    # ax[1].set_yscale("log")
    #ax[0].set_ylim([-5, 20])
    #ax[1].set_xlim([1e-0, 1e3])
    # ax[1].invert_xaxis()

    # ax[0].set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    # ax[1].set_xlabel('Temperature\n(b) Cost Function Changes with Temperature')
    # ax[0].set_ylabel('Cost Function')
    # ax[1].set_ylabel('Cost Function')
    # plt.savefig(out_dir+"/SQSA_XYSA.pdf", bbox_inches='tight')

if __name__ == '__main__':
  T_list         = []
  num_list_sweep = []
  num_list       = []
  for i in range(-7, 5): T_list.append(pow(10, i))
  for i in range(4, 8): num_list_sweep.append(pow(10, i))
  for i in range(4, 7):
    for j in range(int(pow(10, i)), int(pow(10, i+1)), 3*int(pow(10, i))):
      num_list.append(j)
  
  print(num_list)
  print(len(num_list))


  s = sweep()
  # s.sweep_T_XYSA(T_list)
  # s.plot_cost_t(T_list)
  # s.plot_optimal_t(T_list)
  
  # s.sweep_num(num_list)
  # s.plot_cost_num(num_list_sweep)
  # s.plot_optimal_num(num_list)

  # s.run_compare_SQSA_XYSA()
  # s.plot_compare_SQSA_XYSA()

  # s.run_sweep_SQSA_XYSA()
  s.plot_sweep_SQSA_XYSA()

  s.plot_XYSA_MILP_t()



  # show_pic(m2)




