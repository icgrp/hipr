#include "typedefs.hpp"
#include "cmdParser.hpp"

class mTimer {
	double start_clk=0;
	double end_clk=0;

public:
	mTimer(void)
	{
		start_clk = 0;
		end_clk = 0;
	}
	void start(){ start_clk = clock(); }
	void stop() { end_clk = clock(); }
	double return_dur(){ return (end_clk-start_clk)/CLOCKS_PER_SEC; }
};

class AdjListNode {
	int v;
	int weight;

public:
	AdjListNode(int _v, int _w)
	{
		v = _v;
		weight = _w;
	}
	void setWeight(int w){ weight = w; }
	int getV() { return v; }
	int getWeight() { return weight; }
};

// Class to represent a graph using adjacency list
// representation
class Graph {
public:
	int V; // No. of vertices'
	// Pointer to an array containing adjacency lists
	list<AdjListNode>* adj;
	Graph(int V); // Constructor
	~Graph(); // Destructor
};


class hipr{
public:
	string opt_solver;
	string initial_value_path;
	string          pragma_path;
	string          dfx_path;
	string          connect_path;
	string          tile_path;
	string          invalid_tile_path;
	string          xdc_path;
    string          xdc_inst;
	vector<Connect> connects;
	vector<Pragma>  pragmas;
	vector<Tile>    tiles;
	vector<Region>  invalid_tiles;
	vector<Dfx>     dfxs;
	Res             num_per_tile = {CLB_PER_TILE, FF_PER_TILE, BRAM18_PER_TILE, DSP2_PER_TILE};
	Res             device_resource;
	int             CLB_OFFSET;
	int             BRAM36_OFFSET;
	int             BRAM18_OFFSET;
	int             DSP2_OFFSET;
	int             URAM288_OFFSET;
	int 			g_cnt;   // global counter for debugging
	int             max_row;
	ofstream        out_log_file;
	double          t_init;
	double          t_min;
	int             trial_num;
	int             trial_cnt;
	vector<bin_partition> v_slices;
	vector<bin_partition> h_slices;
	map<string, int> dfxs_dict;  // Quickly return the index of a DFXs
	map<Res_type, string> res_dict; // resource dictionary between Res_type and string





	hipr(cmdParser agr_parser);
	~hipr();


	void init_pragma(void);
	void init_connect(void);
	void init_tile(void);
	void init_dfx(void);
	void init_invalid(void);

	bool find_tile_range(Res res_req, Region &prr, bool checkIvalid=true);
	void place_operator(Dfx & op, bool isSeqSA=false);
	void find_all_placements();
	double cost_function(bool debug=false);
	Res return_resource(Region shape);
	void report_utilization(void);
	bool tiles_overlap(Region shape1, Region shape2);
	void write_position(void);





	unsigned int seq_return_vector_idx(vector<int> vector_in, int target);
	void seq_print_out(vector<int> vector_in, string name="no_name");
	void seq_init_seq_pair(vector<int> &int_seq);
	void seq_parse_position(vector<position_seq> &position_info, vector<int> pos_seq, vector<int> neg_seq);
	void seq_gen_graph(Graph &G_V, Graph &G_H, vector<position_seq> position_info);
	void seq_addEdge(Graph &G, int u, int v, int weight);
	void seq_topologicalSortUtil(Graph &G, int v, bool visited[], stack<int>& Stack);
	void seq_y_adjust(Dfx &op);
	void seq_longestPath(Graph &G, int s, string dir="H");
	void seq_floorplan(vector<int> pos_seq, vector<int> neg_seq);
	void seq_swap(int swap_case, int func_idx1, int func_idx2, vector<int> &pos_seq, vector<int> &neg_seq);



	//void milp_init_resource_array(vector<Grid> resource_array[]);
	//void milp_find_partition(vector<Grid> resource_array[], vector<Partition> &part);
	//bool milp_find_rectangle(vector<Grid> resource_array[], Partition &rect);
	//void milp_gen_xdc(vector<Dfx> milp_dfxs);
	//bool milp(bool debug=false);






































	void gen_xdc(void);

	void print_connect(void);
	void print_pragma(void);
	void print_tile(void);
	void print_dfxs(void);
	void print_utilization(void);
	void print_invalid(void);






	void load_init(void);

	double return_total_dest(void);



	Res_range find_resource_range(Res_type res_type, int start_tile, int end_tile);

	bool SimulatedAnnealing(bool debug=false);
	void write_position(Dfx op);
	void write_position(Dfx op, int place_num);
	void write_position(vector<string> names, vector<Region> shapes);

	int return_overlap_tiles(Region shape1, Region shape2);







#ifdef BIPAR
	bool bipar_find_tile_range(res res_req, int x, int y, int h, tile_range &prr, bool debug=false);
	void bipar_find_placements();
	bool bipar_find_feasible(vector< vector<int> > dfx_idxes);
	void bipr_set_dse_idx(vector<vector<int>> &dfx_idxes, int digit[N], uint n);
	void bipar_update_dist_res(dfx &op, tile_range shape);
	bool bipar_nlp_partition (res ub, bin_partition parent_partition, bin_partition &p0, bin_partition &p1);
	void bipar_recur_partition(bin_partition &parent_partition, int start, int cut_line, int end, string cut_type, int level);
	void bipar_reduce_dse(uint space_size);
	void bipar_print_slices(vector<bin_partition> slices);
	bool bipar(bool debug=false);
#endif

};



