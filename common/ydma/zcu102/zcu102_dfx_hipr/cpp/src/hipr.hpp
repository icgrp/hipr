#include "typedefs.hpp"

class hipr{
public:
	string          pragma_path;
	string          dfx_path;
	string          connect_path;
	string          tile_path;
	string          invalid_tile_path;
	string          xdc_path;
    string          xdc_inst;
	vector<connect> connects;
	vector<pragma>  pragmas;
	vector<tile>    tiles;
	vector<invalid_tile> invalid_tiles;
	vector<dfx>     dfxs;
	res_per_tile    num_per_tile = {432, 22, 22};
	int             CLB_OFFSET;
	int             BRAM36_OFFSET;
	int             BRAM18_OFFSET;
	int             DSP2_OFFSET;

	hipr(string path_prefix, string device_name, string inst_name);

	void init_pragma(void);
	void init_connect(void);
	void init_tile(void);
	void init_dfx(void);
	void init_invalid(void);
	void gen_xdc(void);

	void print_connect(void);
	void print_pragma(void);
	void print_tile(void);
	void print_dfx(void);
	void print_utilization(void);
	void print_invalid(void);

	void calc_resource(dfx dfx_in, int resource_num[3]);
	double return_total_dest(void);
	double cost_function(bool debug=false);
	void find_tile_range(dfx & op, bool debug=false);
	res_range find_resource_range(string res_type, int start_tile, int end_tile);
	void floorplan(void);
	void SimulatedAnnealing(bool debug=false);
	void print_seq(void);
};


