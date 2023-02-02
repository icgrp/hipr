#!/bin/bash -e

#start=`date +%s`
#end=`date +%s`

make clean 
start=$(date +%s.%N)
make main -j32
dur=$(echo "$(date +%s.%N) - $start" | bc)
echo "Compile Time: " $dur

