#!/bin/bash -e

g++ -std=c++11  ./src/host.cpp ./src/hipr.cpp -o host  

start=$(date +%s.%N) 

./host $1

dur=$(echo "$(date +%s.%N) - $start" | bc)
printf "run: %.3f seconds" $dur > runtime_${1}.log
printf ""







