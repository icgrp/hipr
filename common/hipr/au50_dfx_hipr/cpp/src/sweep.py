#!/usr/bin/env python
import os
APP_PATH = './src/app/digit_reg512/dfx.txt'
LOG_PATH = './out.log'
op = ["update_knn1", "update_knn10", "update_knn11", "update_knn12", "update_knn13", "update_knn14", "update_knn15", "update_knn16", "update_knn17", "update_knn18", "update_knn19", "update_knn2", "update_knn20", "update_knn3", "update_knn4", "update_knn5", "update_knn6", "update_knn7", "update_knn8", "update_knn9"]
BAR = 15

def show_floorplan(APP_PATH, bar):
  out_list = []
  for idx, name in enumerate(op):
    if idx < bar:
      out_list.append(name+" hipr      3624    5552    80      0")
    else:
      out_list.append(name+" hipr      3624    5552    10      0")
  
  file_out = open(APP_PATH, 'w')
  for line in out_list: file_out.write(line+'\n')
  file_out.close()

def parse_results(log_path):
  sa_result = False
  out_dict = {}
  in_list = [] 
  file_in = open(log_path, 'r')
  for line in file_in:
    in_list.append(line)
  file_in.close()
  
  for idx, line in enumerate(in_list):
    if line.startswith('SA took'):
      word_list = line.split()
      time = word_list[2]
      out_dict['time'] = time

    if line.startswith('at = '):
      word_list = line.split()
      at = word_list[2]
      out_dict['at'] = at

    if line.startswith('T_init ='):
      word_list = line.split()
      T = word_list[2]
      out_dict['T'] = T

    if line.startswith('LUT '):
      word_list = line.split()
      lut_u = word_list[2]
      out_dict['lut_u'] = lut_u

    if line.startswith('BRAM '):
      word_list = line.split()
      bram_u = word_list[2]
      out_dict['bram_u'] = bram_u

    if line.startswith('DSP2 '):
      word_list = line.split()
      dsp_u = word_list[2]
      out_dict['dsp_u'] = dsp_u

    if line.replace('succeeded', '') != line:
      sa_result= True
  
  return sa_result, out_dict
 
def sweep_T(T): 
  out_file = open('sweep_T'+str(T)+'.csv', 'w')
  out_file.write('T,at,lut,bram18,dsp,time\n')
  out_file.close()
  for bar in range(BAR):
    os.system('cp sweep_T'+str(T)+'.csv sweep_now.csv')
    out_file = open('sweep_T'+str(T)+'.csv', 'a')
    show_floorplan(APP_PATH, bar)
    os.system('make > out.log')
    success, out_dict = parse_results(LOG_PATH)
    if success:
      print("case"+str(bar)+" T="+str(T)+" OK! "+str(int(round(float(out_dict['bram_u']), 2)*100))   +'%, '+out_dict['time']+'s')
      out_file.write(out_dict['T']+',')
      out_file.write(out_dict['at']+',')
      out_file.write(str(int(round(float(out_dict['lut_u']), 2)*100))   +'%,')
      out_file.write(str(int(round(float(out_dict['bram_u']), 2)*100))   +'%,')
      out_file.write(str(int(round(float(out_dict['dsp_u']), 2)*100))   +'%,')
      out_file.write(out_dict['time']+'\n')
      out_file.close()
    else:
      print("case"+str(bar)+" failed!")
      out_file.close()
      exit()


if __name__ == '__main__':
  T_list = [0.01, 0.1, 1, 10, 100, 1000] 
  for T in T_list:
    line_list = []
    file_in = open('./src/typedefs.hpp', 'r')
    for line in file_in:
      if line.startswith('#define T_init'):
        line_list.append('#define T_init  '+str(T)+'         // Initial temperature\n')
      else:
        line_list.append(line)
    file_in.close()

    file_out = open('./src/typedefs.hpp', 'w')
    for line in line_list:
      file_out.write(line)
    file_out.close()

    sweep_T(T) 









  # show_pic(m2)




