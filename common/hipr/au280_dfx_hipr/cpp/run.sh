#!/bin/bash -e

CXX=g++
CFLAG='-m64 -g -O3'

GUROBI_DIR=/opt/gurobi951/linux64

INC=-I${GUROBI_DIR}/include/
LIB=-L${GUROBI_DIR}/lib/' -lgurobi_c++ -lgurobi95 -lm'
src=./src
m=

${CXX} ${CFLAG} ${INC} -o host ./${src}/*.cpp ./${src}/*.hpp #${LIB}   


start=$(date +%s) 

./host -A $1 

# dur=$(echo "$(date +%s.%N) - $start" | bc)
end=$(date +%s)    
dur=0
runtime=$((${end} - ${start}))
echo "runtime: $runtime" > $1.runtime







