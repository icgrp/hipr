/*=============================================================================
Name         : main.cpp
Author       : Yuanlong Xiao
Version      : 1.1
Copyright    : Your copyright notice
Description  : This is the main function source for HiPR project
Affiliation  : IC group, University of Pennsylvania
Contributor  : Yuanlong Xiao
Create Date  : 12/01/2022
Design Name  : main function on the top
Project Name : HIPR
Versions     : 1.02
Dependencies : C++
Description  : This is a python script to prepare the script for High Level
               Synthesis for HiPR project.
Revision     :
               Revision 0.01 - File Created
               Revision 0.02 - Update file information
comment      :

Use the Makefile to compile the source code. You will get a main.exe for
execution. To run the code, below are some parameters you can change for 
different optimization levels.
  -T_init        : default 100
                   specify the initial temperature for the simulated
                   annealing algorithm.
  -MY_SEED       : default 173
                   change the seed for random 
  -TRIAL_NUM     : default 1000
                   specify how many times for each temperature unit
  -T_MIN         : default  
                   The cooling temperature for simulated annealing to exit
  -P             : default "./src/app/"
                   The application path to floorplan
  -A             : default "digit_reg_dyn"
                   the application name to be floorplanned
  -DEVICE        : default AU50
                   The device name for the floorplan
  -O             : default "../xdc/sub.xdc"
                   the output file for the floorplan
  -OL            : default "out.log"
                   the file for the output log
  -INST          : default "
                   the instance name for the xdc file
  -OPT           : default "XYSA"
                   the optimizer for the floorplan. The optition can be
                   chosen from {XYSA, SQSA, MILP, BIPAR}

[1] Y. Xiao, A. Hota, D. Park, and A. DeHon. "HiPR: High-level partial 
    reconfiguration for fast incremental FPGA compilation". FPL2022
=============================================================================*/

#include "typedefs.hpp"
#include "hipr.hpp"
#include "cmdParser.hpp"


int main(int argc, char **argv){
	bool RUN_SUCCESS = true;

	// Define the input data path;
	cmdParser cmd_inst(argc, argv);
	srand(cmd_inst.my_seed);

	// Instantiate a pr class;
	hipr pr_inst(cmd_inst);

	// run the floorplan with specified optimization solver
	if(cmd_inst.opt_solver == "MILP")      {

#ifdef MILP
		RUN_SUCCESS = pr_inst.milp();
#endif
	;
	}
	else if(cmd_inst.opt_solver == "XYSA") { RUN_SUCCESS = pr_inst.SimulatedAnnealing(); }
	else if(cmd_inst.opt_solver == "SQSA") { RUN_SUCCESS = pr_inst.SimulatedAnnealing(); }
	else if(cmd_inst.opt_solver == "BIPAR"){ RUN_SUCCESS = pr_inst.SimulatedAnnealing(); }
	else                                   { RUN_SUCCESS = pr_inst.SimulatedAnnealing(); }


	if(RUN_SUCCESS){ cout << cmd_inst.opt_solver << " succeeded!" << endl; }
	else           { cout << cmd_inst.opt_solver << " failed!" << endl;    }

	pr_inst.dump_result(RUN_SUCCESS);
	cmd_inst.print_par();
	cout << "DONE!" << endl;
	return 0;
}
