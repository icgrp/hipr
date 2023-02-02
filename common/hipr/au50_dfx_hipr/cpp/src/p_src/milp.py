#!/usr/bin/env python
import os
from _file import _file
import matplotlib
import matplotlib.pyplot as plt

out_dir = './output/'

PAGE_NUM = 20
MAX_ROW  = 3
# MAX_COL_NUM = 61
app_dir  = './src/app/'  # application dir
arch_dir = './src/arch/' # FPGA device dir

class sweep_milp(_file):
  def __init__(self, pages):
    self.p = pages;

  def gen_connect(self):
    for i in range(1, self.p+1): 
      print(i)

  def clear(self):
    for i in range(1, self.p+1): os.system('rm -rf '+app_dir+'/digit_reg_p'+str(i))

  def gen_connect(self, page_num):
    out_list = ['DMA	    update_knn1 512', 'update_knn'+str(page_num)+' DMA 512 ']
    for i in range(1, page_num): out_list.append('update_knn'+str(i)+' update_knn'+str(i+1)+' 32')
    self.list2file(app_dir+'/digit_reg_p'+str(page_num)+'/connect.txt', out_list)

  def gen_dfx(self, page_num):
    out_list = []
    for i in range(1, page_num+1): out_list.append('update_knn'+str(i)+'	HIPR	1587	1837	16	0')
    self.list2file(app_dir+'/digit_reg_p'+str(page_num)+'/dfx.txt', out_list)

  def gen_initial(self, page_num):
    out_list = ['op x y w h']
    for i in range(1, page_num+1): out_list.append('update_knn'+str(i)+' 0 0 1 1')
    self.list2file(app_dir+'/digit_reg_p'+str(page_num)+'/initial.txt', out_list)

  def gen_pragma(self, page_num):
    out_list = []
    for i in range(1, page_num+1): out_list.append('update_knn'+str(i)+'                  	1	1	1	1')
    self.list2file(app_dir+'/digit_reg_p'+str(page_num)+'/pragma.txt', out_list)

  def gen_app(self):
    for i in range(1, self.p+1):
      os.system('rm -rf '+app_dir+'/digit_reg_p'+str(i))
      os.system('cp -rf '+app_dir+'/digit_reg_p '+app_dir+'/digit_reg_p'+str(i))
      self.gen_connect(i)
      self.gen_dfx(i)
      self.gen_initial(i)
      self.gen_pragma(i)

  def gen_arch(self):
    line_list=self.file2rawlist(arch_dir+'/au50.txt')
    for i in range(5, len(line_list), 10):
      os.system('rm -rf '+arch_dir+'/yboard_r'+str(i)+'.txt')
      self.list2file(arch_dir+'/yboard_r'+str(i)+'.txt', line_list[0:i])
      self.list2file(arch_dir+'/yboard_r'+str(i)+'_invalid.txt', ['OFFSET_CLB_BRAM36_BRAM18_DSP2_URAM288 0 0 0 -6 0', 'MAX_ROW '+str(MAX_ROW), 'INVALID_TILE_x_y_w_h 0 0 1 1'])
  
  def sweep_run(self):
    line_list=self.file2rawlist(arch_dir+'/au50.txt')
    os.system('make main')
    max_page_num = self.p
    max_r_num = len(line_list)
    # max_r_num = MAX_COL_NUM
    for j in range(1, max_page_num+1, 1):
      table_row = []
      for i in range(5, max_r_num+1, 10):
        os.system('rm -rf milp_digit_reg_p'+str(j)+'_r'+str(i)+'_out.log')
        os.system('./main -A digit_reg_p'+str(j)+' -OPT MILP -TRIAL_NUM 100000 -POS position_digit_reg_p'+str(j)+'_yboard_r'+str(i)+'.txt -DEVICE yboard_r'+str(i)+' -OL digit_reg_p'+str(j)+'_r'+str(i)+'_out.log')
  
  def plot_runtime(self):
    line_list=self.file2rawlist(arch_dir+'/au50.txt')
    arch_row_list = []
    max_page_num = self.p
    max_r_num = len(line_list)
    # max_r_num = MAX_COL_NUM
    for i in range(5, max_r_num+1, 10): arch_row_list.append(i)
    runtime_table = [arch_row_list]
    true_table = [arch_row_list]
    for j in range(1, max_page_num+1, 1):
      table_row = []
      true_row = []
      for i in range(5, max_r_num+1, 10):
        log_list = self.file2rawlist('milp_digit_reg_p'+str(j)+'_r'+str(i)+'_out.log')
        run_success = True
        for line in log_list:
          # store whether this run is successful
          if(self.find_str(line, 'Solution count 0')): true_row.append(False)

          # store the runtime
          if(self.find_str(line, 'simplex iterations')):
            words_list = line.split()
            if words_list[7] =='0.00': 
              table_row.append(0.001)
            else:
              table_row.append(float(words_list[7]))
        true_row.append(run_success)

      true_table.append(true_row)
      runtime_table.append(table_row)
    
    
    self.list2file(out_dir+'/runtime.csv', runtime_table)
    self.list2file(out_dir+'/true.csv', true_table)
    # runtime_table.append([0, 2.58, 8.02, 51.22, 103.93, 211.08, 395.48, 603.94, 1072.61, 2173, 0, 0, 0, 0])
    # true_table.append(   [False, True, True, True, True, True, True, True, True, False, False, False, False])
    
    
    f, ax = plt.subplots(1, 2, figsize=(10, 5))
    plt.rcParams.update({'font.size': 10})
    legend_list = []
    row_list = runtime_table[0]
    for row in range(1, len(runtime_table)):
      legend_list.append("page="+str(row))
      x_list = []
      y_list = []
      for col in range(len(runtime_table[0])):
        
        if true_table[row][col]:
          x_list.append(int(row_list[col]))
          y_list.append(float(runtime_table[row][col]))
      
      ax[0].plot(x_list, y_list)
      ax[1].plot(x_list, y_list)
    
    ax[0].legend(legend_list)
    ax[1].legend(legend_list)
    # ax[0].set_ylim([-5, 100])
    # ax[1].set_xlim([1e-12, 1e5])
    # ax[1].invert_xaxis()
    ax[1].set_yscale("log")
    # ax[1].set_yscale("log")
    ax[0].set_xlabel('Device Column Number')
    ax[1].set_xlabel('Device Column Number')
    ax[0].set_ylabel('Time to Optimality /  Seconds')
    ax[1].set_ylabel('Log-scale Time to Optimality /  Seconds')
    # ax[0].set_title(str(size)+' Samples per Digit')
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    # plt.show()
    plt.savefig(out_dir+"/milp.pdf", bbox_inches='tight')


class milp_rosetta(_file):

  def plot_cost_vs_time(self, app):
    f, ax = plt.subplots(3, 2, figsize=(10, 15))
    plt.rcParams.update({'font.size': 10})
    
    xysa_time, xysa_temp, xysa_cost = self.parse_xysa_log(app+'_XYSA_out.log')
    milp_time, milp_cost            = self.parse_milp_log('milp_'+app+'_MILP_out.log')
    xysa_time.append(milp_time[-1])
    xysa_cost.append(xysa_cost[-1])
    
    ax[0][0].plot(xysa_time, xysa_cost)
    ax[0][0].plot(milp_time, milp_cost)
    
    ax[0][0].legend(["XYSA", "MILP"])
    # ax[0].set_ylim([-5, 100])
    # ax[1].set_xlim([1e-12, 1e5])
    # ax[1].invert_xaxis()
    ax[0][0].set_yscale("log")
    ax[0][0].set_xscale("log")
    ax[0][0].set_xlabel('Runtime')
    ax[0][0].set_ylabel('Cost Function')
    # ax[1].set_ylabel('Log-scale Time to Optimality /  Seconds')
    ax[0][0].set_title(app)
    # ax[0].set_xticks([2, 4, 8, 16, 32])

    # plt.show()
    plt.savefig(out_dir+"/xysa_vs_milp.pdf", bbox_inches='tight')




# mainly used for digit recognition sweep
if __name__ == "__main1__":
  os.system('mkdir -p '+out_dir)
  c = sweep_milp(PAGE_NUM)
  c.clear()
  c.gen_app()
  # c.gen_arch()
  # c.sweep_run()
  # c.plot_runtime()

# compare XYSA and MILP in real benchmarks
if __name__ == "__main__":
  os.system('mkdir -p '+out_dir)
  m = milp_rosetta()
  m.plot_cost_vs_time('rendering')


