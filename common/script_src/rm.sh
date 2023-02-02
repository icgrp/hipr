#!/usr/bin/env python
import os

dir_list=[
'F000_increment_320M',
'F000_increment_330M',
'F000_increment_340M',
'F000_increment_350M',
'F000_increment_360M',
'F000_increment_370M',
'F000_increment_380M',
'F000_increment_390M',
'F000_increment_400M',
'F001_overlay_bnn_200M',
'F001_overlay_digit_reg_200M',
'F001_overlay_digit_reg_200M_bk',
'F001_overlay_digit_reg_dyn_50p_100M',
'F001_overlay_digit_reg_dyn_50p_150M',
'F001_overlay_digit_reg_dyn_50p_200M',
'F001_overlay_digit_reg_dyn_50p_250M',
'F001_overlay_digit_reg_dyn_50p_300M',
'F001_overlay_digit_reg_dyn_50p_350M',
'F001_overlay_digit_reg_dyn_50p_400M',
'F001_overlay_digit_reg_dyn_60p_100M',
'F001_overlay_digit_reg_dyn_60p_150M',
'F001_overlay_digit_reg_dyn_60p_200M',
'F001_overlay_digit_reg_dyn_60p_250M',
'F001_overlay_digit_reg_dyn_60p_300M',
'F001_overlay_digit_reg_dyn_60p_350M',
'F001_overlay_digit_reg_dyn_60p_400M',
'F001_overlay_digit_reg_dyn_70p_100M',
'F001_overlay_digit_reg_dyn_70p_150M',
'F001_overlay_digit_reg_dyn_70p_200M',
'F001_overlay_digit_reg_dyn_70p_250M',
'F001_overlay_digit_reg_dyn_70p_300M',
'F001_overlay_digit_reg_dyn_70p_350M',
'F001_overlay_digit_reg_dyn_70p_400M',
'F001_overlay_digit_reg_dyn_80p_100M',
'F001_overlay_digit_reg_dyn_80p_150M',
'F001_overlay_digit_reg_dyn_80p_200M',
'F001_overlay_digit_reg_dyn_80p_250M',
'F001_overlay_digit_reg_dyn_80p_300M',
'F001_overlay_digit_reg_dyn_80p_350M',
'F001_overlay_digit_reg_dyn_80p_400M',
'F001_overlay_face_detect_200M',
'F001_overlay_face_detect_dyn_54p_100M',
'F001_overlay_face_detect_dyn_54p_150M',
'F001_overlay_face_detect_dyn_54p_200M',
'F001_overlay_face_detect_dyn_54p_250M',
'F001_overlay_face_detect_dyn_54p_300M',
'F001_overlay_face_detect_dyn_54p_350M',
'F001_overlay_face_detect_dyn_54p_400M',
'F001_overlay_face_detect_dyn_60p_100M',
'F001_overlay_face_detect_dyn_60p_150M',
'F001_overlay_face_detect_dyn_60p_200M',
'F001_overlay_face_detect_dyn_60p_250M',
'F001_overlay_face_detect_dyn_60p_300M',
'F001_overlay_face_detect_dyn_60p_350M',
'F001_overlay_face_detect_dyn_60p_400M',
'F001_overlay_face_detect_dyn_65p_100M',
'F001_overlay_face_detect_dyn_65p_150M',
'F001_overlay_face_detect_dyn_65p_200M',
'F001_overlay_face_detect_dyn_65p_250M',
'F001_overlay_face_detect_dyn_65p_300M',
'F001_overlay_face_detect_dyn_65p_350M',
'F001_overlay_face_detect_dyn_65p_400M',
'F001_overlay_face_detect_dyn_69p_100M',
'F001_overlay_face_detect_dyn_69p_150M',
'F001_overlay_face_detect_dyn_69p_200M',
'F001_overlay_face_detect_dyn_69p_250M',
'F001_overlay_face_detect_dyn_69p_300M',
'F001_overlay_face_detect_dyn_69p_350M',
'F001_overlay_face_detect_dyn_69p_400M',
'F001_overlay_face_detect_dyn_75p_100M',
'F001_overlay_face_detect_dyn_75p_150M',
'F001_overlay_face_detect_dyn_75p_200M',
'F001_overlay_face_detect_dyn_75p_250M',
'F001_overlay_face_detect_dyn_75p_350M',
'F001_overlay_face_detect_dyn_75p_400M',
'F001_overlay_face_detect_dyn_82p_100M',
'F001_overlay_face_detect_dyn_82p_150M',
'F001_overlay_face_detect_dyn_82p_200M',
'F001_overlay_face_detect_dyn_82p_250M',
'F001_overlay_face_detect_dyn_82p_350M',
'F001_overlay_face_detect_dyn_82p_400M',
'F001_overlay_face_detect_dyn_88p_100M',
'F001_overlay_face_detect_dyn_88p_150M',
'F001_overlay_face_detect_dyn_88p_200M',
'F001_overlay_face_detect_dyn_88p_250M',
'F001_overlay_face_detect_dyn_88p_350M',
'F001_overlay_face_detect_dyn_88p_400M',
'F001_overlay_face_detect_dyn_95p_100M',
'F001_overlay_face_detect_dyn_95p_150M',
'F001_overlay_face_detect_dyn_95p_200M',
'F001_overlay_face_detect_dyn_95p_250M',
'F001_overlay_face_detect_dyn_95p_350M',
'F001_overlay_face_detect_dyn_95p_400M',
'F001_overlay_face_detect_simple_200M',
'F001_overlay_optical_flow_200M',
'F001_overlay_rendering_200M',
'F001_overlay_spam_filter_200M',
'F002_hls_bnn_200M',
'F002_hls_digit_reg_200M',
'F002_hls_rendering_200M',
'F003_syn_bnn_200M',
'F003_syn_digit_reg_200M',
'F003_syn_rendering_200M',
'F004_impl_bnn_200M',
'F004_impl_digit_reg_200M',
'F004_impl_rendering_200M',
'F005_bits_bnn_200M',
'F005_bits_digit_reg_200M',
'F005_bits_rendering_200M']

file_out1=open('main.sh', 'w')
file_out1.write('#!/bin/bash -e\n')

for ele in dir_list:
  print(ele)
  file_out=open(ele+'.sh', 'w')
  file_out.write('#!/bin/bash -e\n')
  file_out.write('rm -rf '+ele+'\n')
  os.system('chmod +x '+ele+'.sh')
  file_out1.write('qsub -N rm'+ele+' -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd ./'+ele+'.sh\n')
  file_out.close()

file_out1.close()







