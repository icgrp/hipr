#!/bin/bash -e

#my_dir=
#
#make prj_name=      freq=300M overlay || true

sqsub(){
  mkdir -p workspace/qsub
  qsub -N $1 -q 70s -hold_jid NONE -m abe -M qsub@qsub.com -l mem=100G -pe onenode 1  -o workspace/qsub -e workspace/qsub -cwd $2
}
#


#make prj_name=zcu102_1 overlay
mkdir -p qsub
qsub -N rendering$1 -q 70s -hold_jid NONE -m abe -M qsub@qsub.com -l mem=120G -pe onenode 1  -o qsub -e qsub -cwd build.sh
