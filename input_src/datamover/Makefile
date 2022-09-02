# Makefile for Digit_reg of Rosetta benchmarks
#
# Author: Yuanlong Xiao (ylxiao@seas.upenn.edu)
#
# Targets:
#   all   - Builds hardware and software in SDSoC.

ifndef XILINX_VIVADO
  $(error Environment variable XILINX_VIVADO is required and should point to Vitis install area)
endif



OPERATORS_OBJ=$(addsuffix .o, $(notdir $(basename $(wildcard operators/*.cpp))))
# HOST_OBJ=$(addsuffix .o, $(notdir $(basename $(wildcard host/*.cpp))))
HOST_OBJ=main.o top.o



INCLUDE=-I $(XILINX_VIVADO)/include 
OPT_LEVEL=-O3
CFLAGS=$(INCLUDE) $(OPT_LEVEL)
CXX=g++
VPATH=src


all: main
	./main

main:$(OPERATORS_OBJ) $(HOST_OBJ)
	$(CXX) $(CFLAGS) -o main $^ 

$(HOST_OBJ):%.o:./host/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^


$(OPERATORS_OBJ):%.o:./operators/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^

run: main
	./main

install:
	echo hello

print: 
	echo $(XILINX_VIVADO)
	ls ./src

tar:
	tar -czvf ./src.tar.gz ./src/ 


try:
	echo $(notdir $(wildcard ./src)) 



clean:
	rm -rf ./*.o main


















