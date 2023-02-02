
CXX=g++
CFLAG=-m64 -g -O3 -DMILP
GUROBI_DIR=/opt/gurobi951/linux64

ifeq ($(shell hostname), icgrid42.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid43.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid44.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid45.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid46.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid47.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif
ifeq ($(shell hostname), icgrid48.seas.upenn.edu)
  GUROBI_DIR=/scratch/volatile/gurobi951/linux64
endif



INC=-I$(GUROBI_DIR)/include/
LIB=-L$(GUROBI_DIR)/lib/ -lgurobi_c++ -lgurobi95 -lm
src=./src
# app=b80
# app=yapp_inc
# app=digit_reg_p6
# app=digit_reg_dyn_70p
app=rendering
m=$(shell date)
s='position.txt'
# device=yboard_265k
device=au50
# opt=XYSA
opt=XYSA
n=100000
t=100

#./main -TRIAL_NUM 10000 -T_init 1 -MY_SEED 199 -T_MIN 1e-3 -P ./src/app/$(app) -DEVICE au50 -OPT XYSA
#./main -TRIAL_NUM 10000 -T_init 1 -MY_SEED 199 -T_MIN 1e-3 -P ./src/app/synthetic -DEVICE au50 -OPT XYSA
#./src/rec.py
all: main
	date
	rm -rf $(app).success
	rm -rf $(app).fail
	./main -A $(app) -OPT $(opt) -T_init $(t) -TRIAL_NUM $(n) -POS $(s) -DEVICE $(device) -OL $(app)_$(opt)_out.log
	date

run: main
	date
	./main
	./src/rec.py
	date

gen_dfx:
	./src/p_src/$@.py


show:
	./src/p_src/rec.py -i $(s) -d $(device)


sweep:
	./src/p_src/sweep.py

sweep_t:
	./src/p_src/sweep.py -t

sweep_n:
	./src/p_src/sweep.py -n

sweep_q:
	./src/p_src/sweep.py -q

sweep_m:
	./src/p_src/sweep.py -m

milp:
	./src/p_src/$@.py


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


try:
	echo $(host)

git:
	#cp -rf ./*.py ./src
	cp -rf ./Makefile ./src
	cd ./src && rm -rf ./main_sa ./main ./main_milp && git add . && git commit -m '$(m)' && git push origin main 

pull:
	cd ./src && git pull
	cp ./src/Makefile ./


clean:
	rm -rf ./mip1 ./*mps ./main_sa  ./*.log ./*.lp ./main_milp
	rm -rf output/*
	rm -rf ./position*.txt
	rm -rf ./*.success
	rm -rf ./*.fail
	rm -rf ./*.pdf

cleanall:
	rm -rf ./mip1 ./*mps ./main_sa ./main ./*.log ./*.lp ./main_milp
	rm -rf output/*
	rm -rf ./position*.txt
	rm -rf ./*.success
	rm -rf ./*.fail
	rm -rf ./*.pdf


clear:
	rm -rf ./mip1 ./*mps ./main_sa ./*.log ./*.lp ./main_milp
	rm -rf output/*


