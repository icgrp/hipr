/* Copyright 2022, Gurobi Optimization, LLC */

/* This example formulates and solves the following simple MIP model:

     maximize    x +   y + 2 z
     subject to  x + 2 y + 3 z <= 4
                 x +   y       >= 1
                 x, y, z binary
*/

#include "typedefs.hpp"
#include "hipr.hpp"
#include "cmdParser.hpp"


int main(int argc, char **argv){

	// Define the input data path;
	cmdParser cmd_inst(argc, argv);
	srand(cmd_inst.my_seed);


	// Instantiate a pr class;
	hipr pr_inst(cmd_inst);
	// pr_inst.print_invalid();
	// pr_inst.print_connect();
	// pr_inst.print_pragma();
	// pr_inst.print_tile();
	bool RUN_SUCCESS = true;
	string method;


	if(cmd_inst.opt_solver == "MILP"){
		//RUN_SUCCESS = pr_inst.milp();
	}else if(cmd_inst.opt_solver == "XYSA"){
		RUN_SUCCESS = pr_inst.SimulatedAnnealing();
	}else if(cmd_inst.opt_solver == "SQSA"){
		RUN_SUCCESS = pr_inst.SimulatedAnnealing();
	}else if(cmd_inst.opt_solver == "BIPAR"){
		RUN_SUCCESS = pr_inst.SimulatedAnnealing();
	}else{
		RUN_SUCCESS = pr_inst.SimulatedAnnealing();
	}


	// pr_inst.milp();
	if(RUN_SUCCESS){
		cout << cmd_inst.opt_solver << " succeeded!" << endl;
	}else{
		cout << cmd_inst.opt_solver << " failed!" << endl;
	}

	cmd_inst.print_par();
	cout << "DONE!" << endl;
	return 0;
}
