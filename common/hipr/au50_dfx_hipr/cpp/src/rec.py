#!/usr/bin/env python
import matplotlib
import matplotlib.pyplot as plt
ARCH_PATH = './src/arch/au50.txt'
MAX_ROW = 8

def show_floorplan(arch_file_name, max_row, m):
  fig = plt.figure(figsize=(10, 10))
  ax = fig.add_subplot(111)
  
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
        plt.text(xc-0.2, yc-0.2, key, fontsize=20, color='black')
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
        yc = float(m[key][3])+float(m[key][5])/2.0
        plt.text(xc-0.2, yc-0.2, key, fontsize=20, color='purple')
        #rect2 = matplotlib.patches.Circle((xc, yc), 1, color='red')
        #ax.add_patch(rect2)

  plt.xlim([-2, tile_max+2])
  plt.ylim([-0.3, max_row])
  plt.show()


if __name__ == '__main__':
  m={'a': [6, 6, 5, 8],
     'b': [9, 0, 6, 6],
     'c': [0, 4, 5, 4],
     'd': [11,6, 4, 10],
     'e': [0, 8, 6, 5],
     'f': [0, 0, 9, 4]}
  m2 = {}
  position_in = open('./position.txt', 'r')
  for idx, line in enumerate(position_in):
    if idx != 0:
      line_ele = line.replace('\n', '').split()
      m2[line_ele[0]] = line_ele

  show_floorplan(ARCH_PATH, MAX_ROW, m2)
  # show_pic(m2)


