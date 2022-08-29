
#include "cmdParser.hpp"

cmdParser::cmdParser(int argc, char **argv){
	if(argc%2==0){
		cout << "The correct in put arguments should be -XX XX!" << endl;
		exit(FAILURE);
	}
	if(argc>1){
		cout << "arguments=====================================" << endl;
		for(int i=1; i<argc; i+=2){
			if((string) argv[i] == "-T_init"){
				t_init = stod(argv[i+1]);
				_t_init = true;
				cout << argv[i] << ": " << t_init << endl;
			}else if((string) argv[i] == "-MY_SEED"){
				my_seed = stoi(argv[i+1]);
				_my_seed = true;
				cout << argv[i] << ": " << my_seed << endl;
			}else if((string) argv[i] == "-TRIAL_NUM"){
				trial_num = stoi(argv[i+1]);
				_trial_num = true;
				cout << argv[i] << ": " << trial_num << endl;
			}else if((string) argv[i] == "-T_MIN"){
				t_min = stod(argv[i+1]);
				_t_min = true;
				cout << argv[i] << ": " << t_min << endl;
			}else if((string) argv[i] == "-P"){
				path_prefix = argv[i+1];
				_path_prefix = true;
				cout << argv[i] << ": " << path_prefix << endl;
			}else if((string) argv[i] == "-DEVICE"){
				device = argv[i+1];
				_device = true;
				cout << argv[i] << ": " << device << endl;
			}else if((string) argv[i] == "-O"){
				out_file = argv[i+1];
				_out_file = true;
				cout << argv[i] << ": " << out_file << endl;
			}else if((string) argv[i] == "-OL"){
				out_log_file = argv[i+1];
				_out_log_file = true;
				cout << argv[i] << ": " << out_log_file << endl;
			}else if((string) argv[i] == "-INST"){
				inst_name = argv[i+1];
				_inst_name = true;
				cout << argv[i] << ": " << out_log_file << endl;
			}else if((string) argv[i] == "-OPT"){
				opt_solver = argv[i+1];
				_opt_solver = true;
				cout << argv[i] << ": " << out_log_file << endl;
			}else{
				cout << "Invalid input argument " << argv[i] << "!" << endl;
				exit(FAILURE);
			}
		}
		cout << "End of arguments==============================" << endl;
	}
	if(!_t_init)       { t_init      = T_INIT1; }
	if(!_my_seed)      { my_seed     = MY_SEED1; }
	if(!_trial_num)    { trial_num   = TRIAL_NUM1; }
	if(!_t_min)        { t_min       = T_MIN1; }
	if(!_path_prefix)  { path_prefix = PATH_PREFIX1; }
	if(!_device)       { device      = DEVICE1; }
	if(!_out_file)     { out_file    = OUT_FILE1; }
	if(!_out_log_file) { out_log_file= OUT_LOG_FILE1; }
	if(!_inst_name)    { inst_name   = INST_NAME1; }
	if(!_opt_solver)   { opt_solver    = OPT_SOLVER; }
}

string cmdParser::isSet(bool in){
	string outStr = in ? "Specified" : "Default";
	return outStr;
}

void cmdParser::print_par(void){
	cout << std::right;
	cout << setw(20) << t_init       << "(" << setw(9) << this->isSet(_t_init)       << "): The initial temperature" << endl;
	cout << setw(20) << opt_solver   << "(" << setw(9) << this->isSet(_opt_solver)   << "): The floorplan solver" << endl;
	cout << setw(20) << my_seed      << "(" << setw(9) << this->isSet(_my_seed)      << "): The random seed" << endl;
	cout << setw(20) << trial_num    << "(" << setw(9) << this->isSet(_trial_num)    << "): The iteration number per 10X of T" << endl;
	cout << setw(20) << t_min        << "(" << setw(9) << this->isSet(_t_min)        << "): The cooling temperature" << endl;
	cout << setw(20) << device       << "(" << setw(9) << this->isSet(_device)       << "): The target device" << endl;
	cout << setw(20) << path_prefix  << "(" << setw(9) << this->isSet(_path_prefix)  << "): The application path" << endl;
	cout << setw(20) << out_log_file << "(" << setw(9) << this->isSet(_out_log_file) << "): The output log file" << endl;
	cout << setw(20) << out_file     << "(" << setw(9) << this->isSet(_out_file)     << "): The output XDC file" << endl;

};
