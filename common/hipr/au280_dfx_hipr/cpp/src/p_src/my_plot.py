#!/usr/bin/env python
import matplotlib
import matplotlib.pyplot as plt

class my_plot():
  def __init__(self, para_dict):
    self.para_dict = para_dict

  def return_target_list(self, idx, file_name):
    outList=[]
    file_in = open(file_name, 'r')
    for line in file_in:
      if line.startswith('Time:'):
        line_list = line.split()
        outList.append(float(line_list[idx]))
    file_in.close()
    return outList

  def return_min_cost(self, file_name):
    file_in = open(file_name, 'r')
    cost_min = 0
    for line in file_in:
      if line.startswith('cost_min = '):
        line_list = line.split()
        cost_min = float(line_list[2])
    file_in.close()
    return cost_min

  def return_runtime(self, file_name):
    file_in = open(file_name, 'r')
    runtime = 0
    for line in file_in:
      if line.startswith('SA took'):
        line_list = line.split()
        runtime = float(line_list[2])
    file_in.close()
    return runtime

  def plot_cost_vs_runtime(self, num_list, num2_list, feature_name='trial_num'):
    cost_xy_list = []
    cost_seq_list = []
    runtime_xy_list = []
    runtime_seq_list = []

    # parse sequence pair runtime
    for i, num in enumerate(num_list): 
      cost_seed_seq_list = []
      runtime_seed_seq_list = []
      for  seed in range(self.para_dict['seed_num']):
        cost_seed_seq_list.append(self.return_min_cost( 'output_'+feature_name+'_seq/output_xy_log_seed_'+str(seed)+'_'+feature_name+'_'+str(num)+'.log'))
        runtime_seed_seq_list.append(self.return_runtime( 'output_'+feature_name+'_seq/output_xy_log_seed_'+str(seed)+'_'+feature_name+'_'+str(num)+'.log'))
      cost_seq_list.append(float(sum(cost_seed_seq_list))/len(cost_seed_seq_list))
      runtime_seq_list.append(float(sum(runtime_seed_seq_list))/len(runtime_seed_seq_list))

    # parse xy runtime
    for i, num in enumerate(num2_list): 
      cost_seed_xy_list = []
      runtime_seed_xy_list = []
      for  seed in range(self.para_dict['seed_num']):
        cost_seed_xy_list.append (self.return_min_cost( 'output_'+feature_name+'_xy/output_xy_log_seed_'+str(seed)+'_'+feature_name+'_'+str(num)+'.log'))
        runtime_seed_xy_list.append (self.return_runtime( 'output_'+feature_name+'_xy/output_xy_log_seed_'+str(seed)+'_'+feature_name+'_'+str(num)+'.log'))
      cost_xy_list.append(float(sum(cost_seed_xy_list))/len(cost_seed_xy_list))
      runtime_xy_list.append(float(sum(runtime_seed_xy_list))/len(runtime_seed_xy_list))
      
    fig2 = plt.figure(figsize=(10, 5))
    ax2 = fig2.add_subplot(111)
    plt.plot(runtime_xy_list, cost_xy_list, '-d', label='xy_sa_min_cost')
    plt.plot(runtime_seq_list, cost_seq_list, '-d', label='seq_sa_min_cost')
    # plt.scatter(t_list, cost_list, marker="d", s=20, c='NONE', edgecolors='r', label='min_cost')
    ax2.legend(fontsize=20, loc='best')
    ax2.set_ylabel('Final Cost', fontsize=20)
    ax2.set_xlabel('Runtime / Sec.', fontsize=20)
    # ax2.set_yscale('log')
    ax2.set_xscale('log')
    # ax2.invert_xaxis()
    plt.show()


  

    
  def plot_cost_vs_trial_num(self, trial_num_list):
    temp_max  = 0
    temp_min  = 0
    cost_xy_list = []
    cost_seq_list = []
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    for i, trial_num in enumerate(trial_num_list): 
      g_cost   = self.return_target_list(14, 'output/output_xy_log_seed_0_trial_num_'+str(trial_num)+'.log')
      temp     = self.return_target_list(3,  'output/output_xy_log_seed_0_trial_num_'+str(trial_num)+'.log')
      cost_seed_xy_list = []
      cost_seed_seq_list = []
      for  seed in range(self.para_dict['seed_num']):
        cost_seed_xy_list.append (self.return_min_cost( 'output_trial_num_xy/output_xy_log_seed_'+str(seed)+'_trial_num_'+str(trial_num)+'.log'))
        cost_seed_seq_list.append(self.return_min_cost( 'output_trial_num_seq/output_xy_log_seed_'+str(seed)+'_trial_num_'+str(trial_num)+'.log'))
      print(cost_seed_xy_list)
      cost_xy_list.append(float(sum(cost_seed_xy_list))/len(cost_seed_xy_list))
      cost_seq_list.append(float(sum(cost_seed_seq_list))/len(cost_seed_seq_list))
      plt.plot(temp, g_cost, label='trial_num='+str(trial_num))
      if i==0:
        temp_max = max(temp)
        temp_min = min(temp)
      if temp_min > min(temp): temp_min = min(temp)
      if temp_max < max(temp): temp_max = max(temp)
      
    plt.plot([temp_min,temp_max], [1, 1], linestyle='--', color='grey', label='Feasible Solution')
    ax.legend(fontsize=10, loc='lower left')
    ax.set_ylabel('Cost Function', fontsize=20)
    ax.set_xlabel('Temperature', fontsize=20)
    ax.set_yscale('log')
    ax.set_xscale('log')
    ax.invert_xaxis()
    ax.grid(axis='x')
    #plt.ylim([0, max(y2)])
    plt.show()

    fig2 = plt.figure(figsize=(10, 5))
    ax2 = fig2.add_subplot(111)
    plt.plot(trial_num_list, cost_xy_list, '-d', label='xy_sa_min_cost')
    plt.plot(trial_num_list, cost_seq_list, '-d', label='seq_sa_min_cost')
    print(trial_num_list)
    print(cost_xy_list)
    # plt.scatter(t_list, cost_list, marker="d", s=20, c='NONE', edgecolors='r', label='min_cost')
    ax2.legend(fontsize=20, loc='upper right')
    ax2.set_ylabel('Final Cost', fontsize=20)
    ax2.set_xlabel('Trial Numbers per 10X', fontsize=20)
    # ax2.set_yscale('log')
    ax2.set_xscale('log')
    # ax2.invert_xaxis()
    plt.show()


  def plot_cost_vs_t_init(self, t_init_list):
    temp_max  = 0
    temp_min  = 0
    cost_xy_list = []
    cost_seq_list = []
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    for i, t_init in enumerate(t_init_list): 
      g_cost   = self.return_target_list(14, 'output_t_init_seq/output_xy_log_seed_0_t_init_'+str(t_init)+'.log')
      temp     = self.return_target_list(3,  'output_t_init_seq/output_xy_log_seed_0_t_init_'+str(t_init)+'.log')
      cost_xy_seed_list = []
      cost_seq_seed_list = []
      for  seed in range(self.para_dict['seed_num']):
        cost_xy_seed_list.append(self.return_min_cost( 'output_t_init_xy/output_xy_log_seed_'+str(seed)+'_t_init_'+str(t_init)+'.log'))
        cost_seq_seed_list.append(self.return_min_cost( 'output_t_init_seq/output_xy_log_seed_'+str(seed)+'_t_init_'+str(t_init)+'.log'))
      print('t_init='+str(t_init)+': ', cost_xy_seed_list)
      cost_xy_list.append(float(sum(cost_xy_seed_list))/len(cost_xy_seed_list))
      cost_seq_list.append(float(sum(cost_seq_seed_list))/len(cost_seq_seed_list))
      plt.plot(temp, g_cost, label='t_init='+str(t_init))
      if i==0:
        temp_max = max(temp)
        temp_min = min(temp)
      if temp_min > min(temp): temp_min = min(temp)
      if temp_max < max(temp): temp_max = max(temp)
      
    plt.plot([temp_min,temp_max], [1, 1], linestyle='--', color='grey', label='Feasible Solution')
    ax.legend(fontsize=10, loc='lower left')
    ax.set_ylabel('Cost Function', fontsize=20)
    ax.set_xlabel('Temperature', fontsize=20)
    ax.set_yscale('log')
    ax.set_xscale('log')
    ax.invert_xaxis()
    ax.grid(axis='x')
    #plt.ylim([0, max(y2)])
    plt.show()
    
    fig2 = plt.figure(figsize=(10, 5))
    ax2 = fig2.add_subplot(111)
    plt.plot(t_init_list, cost_xy_list, '-d', label='xy_sa_min_cost')
    plt.plot(t_init_list, cost_seq_list, '-d', label='seq_sa_min_cost')
    print(t_init_list)
    print(cost_xy_list)
    # plt.scatter(t_list, cost_list, marker="d", s=20, c='NONE', edgecolors='r', label='min_cost')
    ax2.legend(fontsize=20, loc='upper right')
    ax2.set_ylabel('Final Cost', fontsize=20)
    ax2.set_xlabel('Initial Temperature', fontsize=20)
    # ax2.set_yscale('log')
    ax2.set_xscale('log')
    # ax2.invert_xaxis()
    plt.show()






#  def get_cost_v_T(T_list, cost_list, T_init):
#    file_in = open('out_xy'+str(T_init)+'.log', 'r')
#    for idx, line in enumerate(file_in):
#      if line.startswith('Time:'):
#        line_list = line.split()
#        cost_list.append(float(line_list[11]))
#        T_list.append(float(line_list[3]))
#    file_in.close()

 


# if __name__ == '__main__':
#   file_in = open('out_xy.log', 'r')
#   
#   T1000        = []
#   T100        = []
#   T10        = []
#   T1        = []
#   T        = []
#   time     = []
#   cur_cost = []
#   g_cost   = []
#   g_cost1000   = []
#   g_cost100   = []
#   g_cost10   = []
#   g_cost1   = []
#   for idx, line in enumerate(file_in):
#     if line.startswith('Time:'):
#       line_list = line.split()
#       # print(line_list)
#       time.append(float(line_list[1]))
#       cur_cost.append(float(line_list[8]))
#       g_cost.append(float(line_list[11]))
#       T.append(float(line_list[3]))
#   file_in.close()
#    
# 
#   get_cost_v_T(T1000, g_cost1000, 1000)
#   get_cost_v_T(T100,  g_cost100,  100)
#   get_cost_v_T(T10,   g_cost10,   10)
#   get_cost_v_T(T1,    g_cost1,    1)
#  
# 
#   fig = plt.figure(figsize=(10, 10))
#   ax = fig.add_subplot(121)
#   plt.scatter(time, cur_cost, marker="d", s=20, c='NONE', edgecolors='r', label='current cost')
#   plt.plot(time, g_cost, label='global cost')
#   plt.plot([-1, max(time)], [1, 1], linestyle='--', color='grey', label='Feasible Solution')
#   #plt.ylim([0, max(y2)])



# if __name__ == '__main__':
#   file_in = open('out_xy.log', 'r')
#   
#   T1000        = []
#   T100        = []
#   T10        = []
#   T1        = []
#   T        = []
#   time     = []
#   cur_cost = []
#   g_cost   = []
#   g_cost1000   = []
#   g_cost100   = []
#   g_cost10   = []
#   g_cost1   = []
#   for idx, line in enumerate(file_in):
#     if line.startswith('Time:'):
#       line_list = line.split()
#       # print(line_list)
#       time.append(float(line_list[1]))
#       cur_cost.append(float(line_list[8]))
#       g_cost.append(float(line_list[11]))
#       T.append(float(line_list[3]))
#   file_in.close()
#    
# 
#   get_cost_v_T(T1000, g_cost1000, 1000)
#   get_cost_v_T(T100,  g_cost100,  100)
#   get_cost_v_T(T10,   g_cost10,   10)
#   get_cost_v_T(T1,    g_cost1,    1)
#  
# 
#   fig = plt.figure(figsize=(10, 10))
#   ax = fig.add_subplot(121)
#   plt.scatter(time, cur_cost, marker="d", s=20, c='NONE', edgecolors='r', label='current cost')
#   plt.plot(time, g_cost, label='global cost')
#   plt.plot([-1, max(time)], [1, 1], linestyle='--', color='grey', label='Feasible Solution')
#   #plt.ylim([0, max(y2)])


# def parse_runtime(SA, T):
#   file_in = open(SA+'/sweep_T'+str(T)+'.csv', 'r')
#   xy_sa = []
#   xy_r  = []
#   for idx, line in enumerate(file_in):
#     if idx != 0:
#       word_list = line.split(',')
#       xy_r.append( float(word_list[3].replace('%', ''))/100)
#       xy_sa.append(float(word_list[5].replace('\n', '')))
#   file_in.close()
#   return xy_sa, xy_r
# 
# def compare_sa(): 
#   fig = plt.figure(figsize=(10, 10))
#   ax = fig.add_subplot(111)
#  
#   T_list = [0.01, 0.1, 1, 10, 100, 1000] 
#   # T_list = [0.1, 1, 10, 100, 1000] 
#   # for T in T_list:
#   #  xy_sa, xy_r = parse_runtime('xy_sa', T)
#   #  plt.plot(xy_r, xy_sa, label='XY_SA_T'+str(T))
# 
#   for T in T_list:
#     xy_sa, xy_r = parse_runtime('seq_sa', T)
#     plt.plot(xy_r, xy_sa, label='SEQ_SA_T'+str(T))
# 
# 
#   # plt.xlim([-1, 10])
#   # plt.ylim([0, 20])
#   ax.legend(fontsize=20)
#   ax.set_ylabel('Time to Feasible Solutions / Sec.', fontsize=20)
#   ax.set_xlabel('BRAM Utilization', fontsize=20)
#   plt.show()
# 
# 
# def pic_t_sweep(prefix):
#   t_init = 1000.0
#   for i in range(7):
#     file_in = open(prefix+str(t_init)+'.log', 'r')
#     t_init = t_init/10
#     for line in file_in:
#       print(line)
#     file_in.close()
# 
# if __name__ == '__main__':
#   parser = argparse.ArgumentParser()
#   parser.add_argument('-t', '--run_t',  help="default: plot the fiture with differnt temperature", action='store_true')
#   parser.add_argument('-n', '--name', type=str, help="default: plot the fiture with differnt temperature", default='sb')
# 
#   option_dict = {}
#   args = parser.parse_args()
#   # benchmark_name = args.benchmark_name  
#   option_dict['run_t']        = args.run_t
# 
#   print (option_dict) 
# 
#   if option_dict['run_t']:
#     pic_t_sweep('out_xy_')






























  # compare_sa()
