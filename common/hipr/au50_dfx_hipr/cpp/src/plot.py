#!/usr/bin/env python
import matplotlib
import matplotlib.pyplot as plt


#if __name__ == '__main__':
#  seq_sa = [0.0727, 0.0545, 0.0875, 0.0673, 0.0706, 0.0706, 0.0527, 0.0501, 0.0762, 0.0596, 0.46, 9.4]
#  seq_r  = [0.8791208791, 0.8901098901, 0.9010989011, 0.9120879121, 0.9230769231, 0.9340659341, 0.9450549451, 0.956043956, 0.967032967, 0.978021978, 0.989010989, 1]
#  xy_sa  = [0.3511, 0.2395, 0.2062, 0.2232, 5.7465, 30.855, 9.893, 20.416, 10.3825, 246.9, 623.25]
#  xy_r   = [0.8791208791, 0.8901098901, 0.9010989011, 0.9120879121, 0.9230769231, 0.9340659341, 0.9450549451, 0.956043956, 0.967032967, 0.978021978, 0.989010989]
#
#  fig = plt.figure(figsize=(10, 10))
#  ax = fig.add_subplot(111)
#  plt.plot(seq_r, seq_sa, label='seq_SA')
#  plt.plot(xy_r, xy_sa, label='XY_SA')
#  ax.legend(fontsize=20)
#  ax.set_ylabel('Execution Time / Sec.', fontsize=20)
#  ax.set_xlabel('BRAM Utilization', fontsize=20)
#  plt.show()


#  file_out.close()



def parse_runtime(SA, T):
  file_in = open(SA+'/sweep_T'+str(T)+'.csv', 'r')
  xy_sa = []
  xy_r  = []
  for idx, line in enumerate(file_in):
    if idx != 0:
      word_list = line.split(',')
      xy_r.append( float(word_list[3].replace('%', ''))/100)
      xy_sa.append(float(word_list[5].replace('\n', '')))
  file_in.close()
  return xy_sa, xy_r

def compare_sa(): 
  fig = plt.figure(figsize=(10, 10))
  ax = fig.add_subplot(111)
 
  T_list = [0.01, 0.1, 1, 10, 100, 1000] 
  # T_list = [0.1, 1, 10, 100, 1000] 
  # for T in T_list:
  #  xy_sa, xy_r = parse_runtime('xy_sa', T)
  #  plt.plot(xy_r, xy_sa, label='XY_SA_T'+str(T))

  for T in T_list:
    xy_sa, xy_r = parse_runtime('seq_sa', T)
    plt.plot(xy_r, xy_sa, label='SEQ_SA_T'+str(T))


  # plt.xlim([-1, 10])
  # plt.ylim([0, 20])
  ax.legend(fontsize=20)
  ax.set_ylabel('Time to Feasible Solutions / Sec.', fontsize=20)
  ax.set_xlabel('BRAM Utilization', fontsize=20)
  plt.show()


if __name__ == '__main__':
  compare_sa()
