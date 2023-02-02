/*=============================================================================
Name         : cmdParse.hpp
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
Description  : This is a C++ source to parse the input command for Simulated
               Annealing algorithm.
Revision     :
               Revision 0.01 - File Created
               Revision 0.02 - Update file information
comment      :
=============================================================================*/

#include "typedefs.hpp"
#ifndef __CMDPARSER_H__
#define __CMDPARSER_H__

class cmdParser{
public:
	string opt_solver;
	bool _opt_solver=false;
	string path_prefix;
	bool _path_prefix=false;
	string app_name;
	bool _app_name=false;
	string device;
	bool _device=false;
	double t_init = 0;
	bool _t_init = false;
	int my_seed = 0;
	bool _my_seed = false;
	int trial_num = 0;
	bool _trial_num = false;
	double t_min = 0;
	bool _t_min = false;
	string out_file;
	bool _out_file = false;
	string inst_name;
	bool _inst_name = false;
	string out_log_file;
	bool _out_log_file = false;
	string position_file;
	bool _position_file = false;


	cmdParser(int argc, char **argv);
	string isSet(bool in);
	void print_par(void);

};


#endif
