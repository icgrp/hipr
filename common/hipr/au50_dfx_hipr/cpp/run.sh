#!/bin/bash -e

CXX=g++
CFLAG='-m64 -g -O3'
GUROBI_DIR=/opt/gurobi951/linux64
INC=-I${GUROBI_DIR}/include/
LIB=-L${GUROBI_DIR}/lib/' -lgurobi_c++ -lgurobi95 -lm'
src=./src
m=

# g++ ./src/host.cpp ./src/hipr.cpp -o host  
${CXX} ${CFLAG} ${INC} -o host ./${src}/*.cpp ./${src}/*.hpp #${LIB}   


start=$(date +%s.%N) 

./host -P ./src/app/$1 

# dur=$(echo "$(date +%s.%N) - $start" | bc)
dur=0
printf "run: %.3f seconds" $dur > runtime_${1}.log
printf ""







