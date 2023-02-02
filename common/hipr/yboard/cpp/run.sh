#!/bin/bash -e

CXX=g++
CFLAG='-m64 -g -O3'

GUROBI_DIR=/scratch/volatile/gurobi951/linux64

INC=-I${GUROBI_DIR}/include/
LIB=-L${GUROBI_DIR}/lib/' -lgurobi_c++ -lgurobi95 -lm'
src=./src
board=
m=

${CXX} ${CFLAG} ${INC} -o host ./${src}/*.cpp ./${src}/*.hpp #${LIB}   


start=$(date +%s.%N) 

echo ./host -A $1 -DEVICE ${board} 
./host -A $1 -DEVICE ${board} 

# dur=$(echo "$(date +%s.%N) - $start" | bc)
dur=0
printf "run: %.3f seconds" $dur > runtime_${1}.log
printf ""







