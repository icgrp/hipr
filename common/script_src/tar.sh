#!/bin/bash -e
# qsub -N digit_reg_dyn_50p_100 -q 70s -hold_jid digit_reg_dyn_49p_100 -l mem=40G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd ./qsub_src/run_digit_reg_dyn_50p_100M.sh

for i in $(seq 100 10 400)
do
  echo '#!/bin/bash -e'                   > run${i}.sh
  echo 'tar -zxvf ./F000_increment_'${i}'M.tar.gz' >> run${i}.sh 
  chmod +x                                  run${i}.sh
  qsub -N tar${i} -q 70s -l mem=20G -pe onenode 1  -o ./workspace/qsub -e ./workspace/qsub -cwd ./run${i}.sh
done
