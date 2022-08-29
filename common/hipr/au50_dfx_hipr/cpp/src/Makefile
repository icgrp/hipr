
CXX=g++
CFLAG=-m64 -g -O3
GUROBI_DIR=/opt/gurobi951/linux64
INC=-I$(GUROBI_DIR)/include/
LIB=-L$(GUROBI_DIR)/lib/ -lgurobi_c++ -lgurobi95 -lm
src=./src
app=bnn512
m=
all: main
	date
	./main -P ./src/app/$(app) 
	#./main -TRIAL_NUM 10000 -T_init 1 -MY_SEED 199 -T_MIN 1e-3 -P ./src/app/$(app) -DEVICE au50 -OPT XYSA
	#./main -TRIAL_NUM 10000 -T_init 1 -MY_SEED 199 -T_MIN 1e-3 -P ./src/app/synthetic -DEVICE au50 -OPT XYSA
	#./src/rec.py
	date


run: main
	date
	./main
	./src/rec.py
	date

show:
	./src/rec.py


sweep:
	./src/$@.py

plot:
	./$@.py


# g++ -m64 -g -o mip1 ./$(src)/mip1.cpp -I/opt/gurobi951/linux64/include/ -L/opt/gurobi951/linux64/lib/ -lgurobi_c++ -lgurobi95 -lm
main: ./$(src)/*.cpp ./$(src)/*.hpp
	$(CXX) $(CFLAG) $(INC) -o $@ $^  $(LIB)   


sa: main_sa
	date
	./main_sa
	date

main_sa: ./$(src)/*.cpp ./$(src)/*.hpp
	$(CXX) $(CFLAG) $(INC) -o $@ $^  $(LIB) 

milp: main_milp
	date
	./main_milp
	date

main_milp: ./$(src)/*.cpp ./$(src)/*.hpp
	$(CXX) -DMILP $(CFLAG) $(INC) -o $@ $^  $(LIB) 


git:
	#cp -rf ./*.py ./src
	cp -rf ./Makefile ./src
	cd ./src && rm -rf ./main_sa ./main ./main_milp && git add . && git commit -m '$(m)' && git push origin main 

clean:
	rm -rf ./mip1 ./*mps ./main_sa ./main ./*.log ./*.lp ./main_milp
	rm -rf output/*
	rm -rf ./position.txt

clear:
	rm -rf ./mip1 ./*mps ./main_sa ./*.log ./*.lp ./main_milp
	rm -rf output/*


