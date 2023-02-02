
class _file:
  def __init__(self):
    pass

  def find_str(self, str_in, str_target):
    if(str_in.replace(str_target, '') == str_in): return False
    else                                        : return True
  
  def file2list(self, file_name):
    file_in=open(file_name,'r')
    line_list = []
    for line in file_in: line_list.append(line.replace('\n','').replace(' ',''))
    file_in.close()
    return line_list

  def file2rawlist(self, file_name):
    file_in=open(file_name,'r')
    line_list = []
    for line in file_in: line_list.append(line.replace('\n',''))
    file_in.close()
    return line_list
 
  def list2file(self, file_name, line_list, splitter='\t', isExe=False):
    file_out = open(file_name, 'w')
    if isExe: file_out.write('#!/bin/bash -e\n')
    for line in line_list:
      if(type(line)==list):
        line = self.str_list(line)
        file_out.write(splitter.join(line)+'\n')
      else:
        file_out.write(line+'\n')
    file_out.close()
    if isExe: os.system('chmod +x '+file_name)

  def dict2list(self, dict_in):
    out_list = []
    for key in sorted(dict_in.keys()):
      line_str = str(key)
      if type(dict_in[key]) is list:
        for word in dict_in[key]: line_str += ','+str(word) 
      else:
        line_str += ','+dict_in[key]
      out_list.append(line_str)
    return out_list

  def remove_space(self, in_list):
    out_list=[]
    for ele in in_list: out_list.append(ele.replace(' ',''))
    return out_list
    _
  def file2table(self, file_name, splitter=None):
    line_list = self.file2rawlist(file_name)
    for idx, line in enumerate(line_list):
      if splitter == None:
        line_list[idx] = self.remove_space(line.split())
      else:
        line_list[idx] = self.remove_space(line.split(splitter))
    return line_list 

  # copy list2 to list1 accroding to left_top and right_bottom coordinates
  def slice_cp(self, list1, list2, lt1, lt2, row_num, col_num):
    local_list = []
    for i in range(len(list1)):
      local_list.append(list1[i].copy())
    
    for row in range(row_num):
      for col in range(col_num):
        local_list[lt1[0]+row][lt1[1]+col] = list2[lt2[0]+row][lt2[1]+col]
    return local_list

  def return_slice(self, list1, row, col, row_num, col_num):
    out_list = []
    for i in range(row, row+row_num):
      for j in range(col, col+col_num):
        out_list.append(list1[i][j])
        
    return out_list
  
  def print_dict(self, in_dict):
    for key, value in sorted(in_dict.items()):
      print (str(key).ljust(30)+'->'+str(value))
    print('\n\n')
  
  def print_list(self, in_list):
    for idx, line in enumerate(in_list): print(('L'+str(idx)+':').rjust(5), line)
    print('\n\n')
  
  def int_list(self, lst):
    out_lst = []
    for ele in lst:
      out_lst.append(int(ele))
    return out_lst

  def float_list(self, lst):
    out_lst = []
    for ele in lst:
      out_lst.append(float(ele))
    return out_lst

  def str_list(self, lst):
    out_lst = []
    for ele in lst:
      out_lst.append(str(ele))
    return out_lst

  def avg_list(self, lst):
      return sum(lst) / len(lst)

  def replace_lines(self, filename, modification_dict):
    # change the string(key of modification_dict) to
    # target string (value of modification_dict)
    try:
      file_in =  open(filename, 'r')
      file_out = open(filename+'tmp', 'w')
      for line in file_in:
        find_target = False
        for key, value in modification_dict.items():
          if line.replace(key, '') != line:
            file_out.write(value+'\n')
            find_target = True
            break
        if find_target == False:
          file_out.write(line)
      file_out.close()
      file_in.close()
      os.system('mv '+filename+'tmp '+filename)
    except:
      print ("Modification for "+filename+" failed!")

  def gen_dfx(self, file_in, file_out):
    table_in  = f.file2table(file_in) 
    table_out = [] 
    for i in range(1, len(table_in)): table_out.append([table_in[i][0],table_in[i][1],table_in[i][3],table_in[i][4],table_in[i][5],table_in[i][7]])
    self.list2file(file_out, table_out)