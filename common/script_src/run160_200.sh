#!/bin/bash -e

#my_dir=
#
#make prj_name=      freq=300M overlay || true
#sqsub(){
#  mkdir -p workspace/qsub
#  qsub -N $1 -q 70s -hold_jid NONE -m abe -M qsub@qsub.com -l mem=100G -pe onenode 1  -o workspace/qsub -e workspace/qsub -cwd ./common/script_src/run_$1.sh
#}
#
#sqsub rendering512
#sqsub digit_reg512
#sqsub spam_filter512
#sqsub optical_flow512
#sqsub face_detection512
#sqsub bnn512


make freq_start=160 freq_end=200 freq_diff=10  install


