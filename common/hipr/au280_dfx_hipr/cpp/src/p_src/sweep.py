#!/usr/bin/env python
import os
from _file import _file
import matplotlib
import matplotlib.pyplot as plt
import argparse
out_dir = './'

class sweep(_file):

  def sweep_T(self, app, T_list):
    os.system("make main")
    for T in T_list: os.system('./main -TRIAL_NUM 100000 -T_init '+str(T)+' -OL '+' out_T'+str(T)+'.log'+' -POS '+'position_T'+str(T)+'.txt')

  def sweep_T_XYSA(self, app, T_list):
    os.system("make main")
    for T in T_list: os.system('./main -A '+app+' -TRIAL_NUM 100000 -T_init '+str(T)+' -OL '+' out_T'+str(T)+'_'+app+'.log'+' -POS '+'position_T'+str(T)+'.txt')


  def run_compare_SQSA_XYSA(self, app):
    os.system("make main")
    os.system('./main -A '+app+' -OPT SQSA -TRIAL_NUM 100000 -T_init 100 -OL out_SQSA.log -POS position_SQSA.txt')
    os.system('./main -A '+app+' -OPT XYSA -TRIAL_NUM 100000 -T_init 100 -OL out_XYSA.log -POS position_XYSA.txt')

  def run_compare_XYSA_MILP(self, app):
    os.system("make main")
    os.system('./main -A '+app+' -OPT XYSA -TRIAL_NUM 100000 -T_init 100 -OL out_XYSA.log -POS position_XYSA.txt')
    os.system('./main -A '+app+' -OPT MILP -TRIAL_NUM 100000 -T_init 100 -OL out_MILP.log -POS position_MILP.txt')
    

  def run_sweep_SQSA_XYSA(self):
    for r in range(50, 81, 10): os.system('./main -A b'+str(r)+' -OPT XYSA -TRIAL_NUM 100000 -T_init 100 -OL out_XYSA_b'+str(r)+'.log -POS position_XYSA_b'+str(r)+'.txt')
    for r in range(50, 81, 10): os.system('./main -A b'+str(r)+' -OPT SQSA -TRIAL_NUM 100000 -T_init 100 -OL out_SQSA_b'+str(r)+'.log -POS position_SQSA_b'+str(r)+'.txt')


  def sweep_num(self, app, num_list):
    os.system("make main")
    for num in num_list:
      print('./main -A  -TRIAL_NUM '+str(num)+' -T_init 100 out_num'+str(num)+'.log'+' -POS '+'position_num'+str(num)+'.txt')
      os.system('./main -A '+app+' -TRIAL_NUM '+str(num)+' -T_init 100 -OL out_num'+str(num)+'.log'+' -POS '+'position_num'+str(num)+'.txt')


  def parse_t_log(self, app, T):
    time_list = []
    temp_list = []
    cost_list = []
    log_table=self.file2table('out_T'+str(T)+'_'+app+'.log', ' ')
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
    # self.print_list(log_table)
    for line_list in log_table:
      if line_list[0] == 'Time:':
        time_list.append(float(line_list[1] ))
        temp_list.append(float(line_list[3] ))
        cost_list.append(float(line_list[14]))
    
    return time_list, temp_list, cost_list


  def plot_cost_t(self, app, T_list):
    # f, ax = plt.subplots(len(T_list), 2, figsize=(10, 5*len(T_list)))
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    for t_idx, T in enumerate(T_list): 
      time_list, temp_list, cost_list = self.parse_t_log(app, T)
      legend_list.append(r'$T_0$='+str(T))
      ax[0].plot(time_list, cost_list)
      ax[1].plot(temp_list, cost_list)
    
    ax[0].legend(legend_list)
    ax[1].legend(legend_list)
    #ax[0].set_ylim([-5, 100])
    ax[1].set_xlim([1e-12, 1e5])
    ax[1].invert_xaxis()
    ax[1].set_xscale("log")
    # ax[1].set_yscale("log")
    # ax[0].set_yscale("log")
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
      legend_list.append("TRIAL_NUM="+"{:.0e}".format(num))
      ax[0].plot(time_list, cost_list)
      ax[1].plot(temp_list, cost_list)
    
    ax[0].legend(legend_list)
    ax[1].legend(legend_list)
    #ax[0].set_ylim([-5, 100])
    ax[1].set_xlim([1e-12, 1e3])
    ax[1].invert_xaxis()
    ax[0].set_xscale("log")
    ax[1].set_xscale("log")
    ax[0].set_yscale("log")
    ax[1].set_yscale("log")
    ax[0].set_xlabel('Runtime/s\n(a) Cost Function Changes with Runtime')
    ax[1].set_xlabel('Temperature\n(b) Cost Function Changes with Temperature')
    ax[0].set_ylabel('Cost Function')
    ax[1].set_ylabel('Cost Function')
    # ax[0].set_title(str(size)+' Samples per Digit')
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    # plt.show()
    plt.savefig(out_dir+"/sweep_num.pdf", bbox_inches='tight')

  def plot_optimal_t(self, app_list, util_list, T_list):
    f, ax = plt.subplots(1, 1, figsize=(5, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    for idx, app in enumerate(app_list):
      par_list = app.split('_')
      legend_list.append('N='+str(par_list[4])+', P='+str(par_list[5])+', PAR='+str(par_list[6])+', util='+str(util_list[idx])+'%')
      optimal_list = []
      for t_idx, T in enumerate(T_list): 
        time_list, temp_list, cost_list = self.parse_t_log(app, T)
        optimal_list.append(float(cost_list[-1]))
      ax.plot(T_list, optimal_list, marker='*')
      # ax.scatter(T_list, optimal_list, marker='^')
    ax.legend(legend_list)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_ylim([1e-2, 5])
    ax.set_xlabel('Initial Temperature')
    ax.set_ylabel('Final Cost Function')
    # plt.show()
    plt.savefig(out_dir+"/optimal_T.pdf", bbox_inches='tight')

  def plot_optimal_num(self, num_list):
    f, ax = plt.subplots(1, 2, figsize=(12, 5))
    plt.rcParams.update({'font.size': 10})
    optimal_list = []
    runtime_list = []
    for t_idx, num in enumerate(num_list): 
      time_list, temp_list, cost_list = self.parse_num_log(num)
      optimal_list.append(float(cost_list[-1]))
      runtime_list.append(time_list[-1])

    ax[0].plot(num_list[1:len(num_list)], optimal_list[1:len(optimal_list)], marker='*')
    ax[0].set_xscale("log")
    #ax[0].set_yscale("log")
    ax[1].plot(runtime_list[1:len(runtime_list)], optimal_list[1:len(optimal_list)], marker='*')
    #ax[1].set_xscale("log")
    #ax[1].set_yscale("log")
    ax[0].set_xlabel('TRIAL_NUM per 10'+r'$\times$'+' Scale\n(a) Cost Function vs. TRIAL_NUM')
    ax[1].set_xlabel('Runtime / Seconds\n(b) Cost Function vs. Runtime')
    ax[0].set_ylabel('Final Cost Function')
    ax[1].set_ylabel('Final Cost Function')
    # plt.show()
    plt.savefig(out_dir+"/optimal_num.pdf", bbox_inches='tight')

  def plot_compare_SQSA_XYSA(self):
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    time_list, temp_list, cost_list  = self.parse_log('out_SQSA.log')
    # print(time_list)
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


  def plot_xy_sq(self):
    f, ax = plt.subplots(1, 2, figsize=(50, 20))
    plt.rcParams.update({'font.size': 30})
    device = 'au50'
  
    m1 = {}
    position_in = open('position_XYSA.txt', 'r')
    for idx, line in enumerate(position_in):
      if idx != 0:
        line_ele = line.replace('\n', '').split()
        m1[line_ele[0]] = line_ele
  
    m2 = {}
    position_in = open('position_SQSA.txt', 'r')
    for idx, line in enumerate(position_in):
      if idx != 0:
        line_ele = line.replace('\n', '').split()
        m2[line_ele[0]] = line_ele
  
    self.plot_floorplan(device, m1, ax[0], '(a) XYSA_floorplan')
    self.plot_floorplan(device, m2, ax[1], '(b) SQSA_floorplan')
    #ax[0].xaxis.get_label().set_fontsize(40)
    # plt.show()
    plt.savefig(out_dir+"/floorplan_xy_sq.pdf", bbox_inches='tight')
  
  def plot_floorplan(self, device, m, ax, x_label):
    arch_file_name='./src/arch/'+device+'.txt'
    device_info_file=open('./src/arch/'+device+'_invalid.txt', 'r')
    max_row = 0
    for line in device_info_file:
      if(line.replace('MAX_ROW', '') != line):
        max_row=int(line.split()[1])
  
  
  
    color_dict = {'CLB'   : 'cornflowerblue',
                  'BRAM18': 'brown',   
                  'DSP2'  : 'green',   
                  'URAM'  : 'yellow',   
                  'PCIE'  : 'black',   
                  'IOB'   : 'orange'}
  
    # attach the resource of the device 
    tile_max = 0
    for row in range(max_row):
      tile_num = 0 
      file_in = open(arch_file_name, 'r')
      for line in file_in:
        line_list = line.split()
        rect1 = matplotlib.patches.Rectangle((tile_num, row), 0.3, 0.9, linewidth=1, color=color_dict[line_list[0]] )
        ax.add_patch(rect1)
        rect2 = matplotlib.patches.Rectangle((tile_num+0.35, row), 0.3, 0.9, linewidth=1, color=color_dict[line_list[2]] )
        ax.add_patch(rect2)
        tile_num += 1
        if tile_max < tile_num: tile_max = tile_num
      file_in.close()
      tile_num = 0
  
    # attach the invalid areas 
    file_in = open(arch_file_name.replace('.txt', '_invalid.txt'), 'r')
    for line in file_in:
      if line.startswith('INVALID'):
        line_list = line.split()
        rect1 = matplotlib.patches.Rectangle((float(line_list[1]), float(line_list[2])), 
                                              float(line_list[3])-0.3,
                                              float(line_list[4])-0.1,
                                              linewidth=1, color='grey')
        ax.add_patch(rect1)
        
    file_in.close()
  
    # attach the pblocks 
    for key in m:
      if key.startswith('hhhh') or key.startswith('vvvv'):
        if int(m[key][1]) !=0:
          rect1 = matplotlib.patches.Rectangle((float(m[key][2])-0.05, float(m[key][3])-0.025),
                                            float(m[key][4])-0.2, float(m[key][5])-0.05,
                                            fill=False,
                                            linewidth=3,
                                            edgecolor='black' )
          ax.add_patch(rect1)
          xc = float(m[key][2])
          yc = float(m[key][3]) + 0.2
          ax.text(xc-0.2, yc-0.2, key, fontsize=20, color='black')
      else: 
        if int(m[key][1]) !=0:
          rect1 = matplotlib.patches.Rectangle((float(m[key][2])-0.05, float(m[key][3])-0.025),
                                            float(m[key][4])-0.2, float(m[key][5])-0.05,
                                            # fill=False,
                                            color='purple',
                                            alpha=0.5,
                                            linewidth=3,
                                            edgecolor='purple' )
          ax.add_patch(rect1)
    
          rect1 = matplotlib.patches.Rectangle((float(m[key][2])-0.05, float(m[key][3])-0.025),
                                            float(m[key][4])-0.2, float(m[key][5])-0.05,
                                            fill=False,
                                            linewidth=3,
                                            edgecolor='black' )
          ax.add_patch(rect1)
     
          xc = float(m[key][2])+float(m[key][4])/2.0-0.15
          yc = float(m[key][3])+float(m[key][5])/3.0
          #plt.text(xc-0.2, yc-0.2, key, fontsize=20, color='purple')
          ax.text(float(m[key][2]), yc-0.2, key, fontsize=20, color='white')
          #rect2 = matplotlib.patches.Circle((xc, yc), 1, color='red')
          #ax.add_patch(rect2)
  
    ax.set_xlim([-2, tile_max+2])
    ax.set_ylim([-0.3, max_row])
    ax.set_xlabel(x_label, fontsize = 40) # X label
    # plt.show()

if __name__ == '__main__':
    # Use argparse to parse the input arguments
  parser = argparse.ArgumentParser()
  parser.add_argument('-t', '--sweep_t',                          action='store_true', help="sweep temperature ")
  parser.add_argument('-n', '--sweep_n',                          action='store_true', help="sweep trial number")
  parser.add_argument('-q', '--xysq',                             action='store_true', help="sweep trial number")
  parser.add_argument('-m', '--xymi',                             action='store_true', help="sweep trial number")
  parser.add_argument('-a', '--app',     type=str, default='digit_reg_dyn__4480_20_2', help="application name  ")
  
  args      = parser.parse_args()
  sweep_t   = args.sweep_t
  sweep_n   = args.sweep_n
  cmp_xy_sq = args.xysq
  cmp_xy_mi = args.xymi
  app       = args.app

  T_list         = []
  T_list_sweep   = []
  num_list       = []
  num_list_sweep = []
  for i in range(-5, 5): T_list.append(pow(10, i))
  for i in range(-1, 5): T_list_sweep.append(pow(10, i))
  for i in range(4, 8): num_list_sweep.append(pow(10, i))
  for i in range(4, 7):
    for j in range(int(pow(10, i)), int(pow(10, i+1))+1, 3*int(pow(10, i))):
      num_list.append(j)
  
  print(num_list)
  print(num_list_sweep)
  
  s = sweep()
  if sweep_t:
    app_list = ['digit_reg_dyn__1280_20_1', 
                'digit_reg_dyn__1280_20_2', 
                'digit_reg_dyn__2560_20_1',
                'digit_reg_dyn__2560_20_2',
                'digit_reg_dyn__4480_20_1',
                'digit_reg_dyn__4480_20_2',
                'digit_reg_dyn__8320_20_1',
                'digit_reg_dyn__8320_20_2']
    for app in app_list:
      ########################################################## 
      # caution!!! This will remove the previous run directories!
      # s.sweep_T_XYSA(app, T_list)
      # caution!!! This will remove the previous run directories!
      ##########################################################
      pass
    # app_list = ['digit_reg_dyn__1280_20_2', 'digit_reg_dyn__2560_20_2', 'digit_reg_dyn__4480_20_2', 'digit_reg_dyn__8320_20_2']
    # util_list = ['18', '31', '61', '90']
    app_list = ['digit_reg_dyn__4480_20_2']
    util_list = ['61']
    s.plot_cost_t('digit_reg_dyn__4480_20_2', T_list_sweep)
    s.plot_optimal_t(app_list, util_list, T_list)
    pass
  
  if sweep_n:
    ########################################################## 
    # caution!!! This will remove the previous run directories!
    # s.sweep_num(app, num_list)
    # caution!!! This will remove the previous run directories!
    ########################################################## 
    s.plot_cost_num(num_list_sweep)
    s.plot_optimal_num(num_list)
    pass

  app = 'digit_reg_dyn__4480_20_1'
  if cmp_xy_sq:
    ########################################################## 
    # caution!!! This will remove the previous run directories!
    # s.run_compare_SQSA_XYSA(app)
    # caution!!! This will remove the previous run directories!
    ########################################################## 
    s.plot_compare_SQSA_XYSA()
    s.plot_xy_sq()

  app = 'digit_reg_dyn__4480_20_2'
  if cmp_xy_mi:
    s.run_compare_XYSA_MILP(app)
    s.plot_XYSA_MILP_t()

  # s.run_sweep_SQSA_XYSA()
  # s.plot_sweep_SQSA_XYSA()

  # 



  # show_pic(m2)




