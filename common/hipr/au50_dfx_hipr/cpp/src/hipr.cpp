#include "typedefs.hpp"
#include "hipr.hpp"
#include "cmdParser.hpp"


Graph::Graph(int V) // Constructor
{
	this->V = V;
	adj = new list<AdjListNode>[V];
}

Graph::~Graph() // Destructor
{
	delete [] adj;
}



hipr::hipr(cmdParser arg_parser){
	opt_solver         = arg_parser.opt_solver;
	initial_value_path = arg_parser.path_prefix + "/initial.txt";
	pragma_path       = arg_parser.path_prefix + "/pragma.txt";
	dfx_path          = arg_parser.path_prefix + "/dfx.txt";
	connect_path      = arg_parser.path_prefix + "/connect.txt";
	tile_path         = "./src/arch/"+arg_parser.device+".txt";
	invalid_tile_path = "./src/arch/"+arg_parser.device+"_invalid.txt";
	xdc_path          = arg_parser.out_file;
    xdc_inst          = arg_parser.inst_name;
    g_cnt             = 0;
    t_init            = arg_parser.t_init;
    t_min             = arg_parser.t_min;
    trial_num         = arg_parser.trial_num;
    trial_cnt         = trial_num/(-log(10)/log(AT_INIT));
	this->init_pragma();
	this->init_connect();
	this->init_tile();
	this->init_invalid();
	this->init_dfx();
	Region device_shape={0, 0, max_row, (int) tiles.size(), 0, 0};
	device_resource   = this->return_resource(device_shape);
	cout << "The device " << arg_parser.device << " has: " << endl;
	cout << device_resource.lut << " LUTs, " << endl;
	cout << device_resource.bram18 << " BRAM18s, " << endl;
	cout << device_resource.dsp2 << " DSP2s. " << endl;

	string out_log_path = arg_parser.out_log_file;
	const char *cstr = out_log_path.c_str();
	out_log_file.open (cstr);

	res_dict[CLB]    = "CLB";
	res_dict[BRAM18] = "BRAM18";
	res_dict[DSP2]   = "DSP2";
	res_dict[URAM]   = "URAM";
	res_dict[IOB]    = "IOB";
	res_dict[FORB]   = "FORB";
	res_dict[PCIE]   = "PCIE";
}


hipr::~hipr() // Destructor
{
	out_log_file.close();
}

void hipr::init_pragma(void){
	const char *cstr = pragma_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	int i=0;
	Pragma t;
	while(myfile >> t.name >> t.resource.lut >> t.resource.ff >> t.resource.bram18 >> t.resource.dsp2){
		t.i=i++;
		pragmas.push_back(t);
	}
	myfile.close();
}

void hipr::init_connect(void){
	const char *cstr = connect_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	Connect t;
	int i=0;
	while(myfile >> t.src >> t.dest >> t.width){
		t.i = i++;
		connects.push_back(t);
	}
	myfile.close();
}

void hipr::init_tile(void){
	const char *cstr = tile_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	int i=0;
	Tile t;
	string t_tile_l;
	double t_x_l;
	string t_tile_r;
	double t_x_r;

	while(myfile >> t_tile_l >> t_x_l >> t_tile_r >> t_x_r){
		if(t_tile_l == "CLB"   ) { t.tile_l = CLB;    }
		if(t_tile_l == "BRAM18") { t.tile_l = BRAM18; }
		if(t_tile_l == "DSP2"  ) { t.tile_l = DSP2;   }
		if(t_tile_l == "URAM"  ) { t.tile_l = URAM;   }
		if(t_tile_l == "IOB"   ) { t.tile_l = IOB;    }
		if(t_tile_l == "PCIE"  ) { t.tile_l = PCIE;   }
		if(t_tile_r == "CLB"   ) { t.tile_r = CLB;    }
		if(t_tile_r == "BRAM18") { t.tile_r = BRAM18; }
		if(t_tile_r == "DSP2"  ) { t.tile_r = DSP2;   }
		if(t_tile_r == "URAM"  ) { t.tile_r = URAM;   }
		if(t_tile_r == "IOB"   ) { t.tile_r = IOB;    }
		if(t_tile_r == "PCIE"  ) { t.tile_r = PCIE;   }
		t.x_l = t_x_l;
		t.x_r = t_x_r;
		t.i=i++;
		tiles.push_back(t);
	}
	myfile.close();
}

void hipr::init_dfx(void){
	const char *cstr = dfx_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	int i=0;
	Dfx t;

	while(myfile >> t.name >> t.isPR >> t.resource.lut >> t.resource.ff >> t.resource.bram18 >> t.resource.dsp2){
		t.resource.lut    *= pragmas[i].resource.lut;
		t.resource.ff     *= pragmas[i].resource.ff;
		t.resource.bram18 *= pragmas[i].resource.bram18;
		t.resource.dsp2   *= pragmas[i].resource.dsp2;
		t.i=i++;
		t.placement.x = rand()%tiles.size();
		t.placement.y = rand()%this->max_row;
		t.placement.w = 1;
		t.placement.h = 1;
#ifndef MILP
		//this->find_tile_range(t);
#endif
		dfxs.push_back(t);
		dfxs_dict.insert(pair<string, int>(t.name, t.i));
	}
	myfile.close();
}

void hipr::init_invalid(void){
	const char *cstr = invalid_tile_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	Region t;
	string t_name;
	// reand in the CLB BRAM36 BRAM18 DSP2 Y-offset for xdc generation
	myfile >> t_name >> CLB_OFFSET >> BRAM36_OFFSET >> BRAM18_OFFSET >> DSP2_OFFSET >> URAM288_OFFSET;
	if(t_name != "OFFSET_CLB_BRAM36_BRAM18_DSP2_URAM288"){
		cout << "The first line of " << invalid_tile_path << " should be \"OFFSET_CLB_BRAM36_BRAM18_DSP2_URAM288\"!" << endl;
		exit(FAILURE);
	}

	myfile >> t_name >> max_row;
	if(t_name != "MAX_ROW"){
		cout << "The second line of " << invalid_tile_path << " should be \"MAX_ROW\"!" << endl;
		exit(FAILURE);
	}



	// reand in the invalid tiles
	while(myfile >> t_name >> t.x >> t.y >> t.w >> t.h){
		if(t_name == "INVALID_TILE_x_y_w_h"){
			invalid_tiles.push_back(t);
		}else{
			cout << "The format from the second row of " << invalid_tile_path << " should be \"INVALID_TILE_X1_X2_Y1_Y2\"!" << endl;
			exit(FAILURE);
		}
	}
	myfile.close();
}

void hipr::load_init(void){
	const char *cstr = initial_value_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	int i_init=0;
	Region placement_in;;
	string dfx_name;
	// reand in the initial values for x, y, w, h
	string tmp[5];
	if(myfile >> tmp[0] >> tmp[1] >> tmp[2] >> tmp[3] >> tmp[4]){
		if(tmp[0] == "op" && tmp[1] == "x" && tmp[2] == "y" && tmp[3] == "w" && tmp[4] == "h"){
			while(myfile >> dfx_name >> placement_in.x >> placement_in.y >> placement_in.w >> placement_in.h){
				dfxs[i_init].placement = placement_in;
				i_init++;
			}
		}else{
			cout << "The first line in \"" << initial_value_path << "\" should be \"op x y w h\""<< endl;
			exit(0);
		}
	}else{
		cout << "Could not read" << initial_value_path << endl;
		exit(0);
	}
	myfile.close();
}

// If feasible area can be found return ture.
// The new region is included in prr struct.
bool hipr::find_tile_range(Res res_req, Region &prr, bool checkInvalid){
	Res stillNeed = res_req;
	int x=prr.x, y=prr.y, h=prr.h;
	prr.x = x; prr.y = y; prr.h = h; prr.w = 0;
	// cout << "stillNeed.lut0    = " << stillNeed.lut << endl;
	// cout << "stillNeed.bram180 = " << stillNeed.bram18 << endl;
	// cout << "stillNeed.dsp20   = " << stillNeed.dsp2 << endl;
	while(stillNeed.lut > 0 || stillNeed.bram18>0 || stillNeed.dsp2>0){
		prr.w++;
		int x_end = prr.w+x-1;
		// if x_end excesses the boarder of the chip, return false
		if(x_end >= (int) tiles.size()-4) { return false; }
		if(tiles[x_end].tile_l == CLB)    { stillNeed.lut    -= num_per_tile.lut*h; }
		if(tiles[x_end].tile_l == BRAM18) { stillNeed.bram18 -= num_per_tile.bram18*h; }
		if(tiles[x_end].tile_l == DSP2)   { stillNeed.dsp2   -= num_per_tile.dsp2*h; }
		if(tiles[x_end].tile_r == CLB)    { stillNeed.lut    -= num_per_tile.lut*h; }
		if(tiles[x_end].tile_r == BRAM18) { stillNeed.bram18 -= num_per_tile.bram18*h; }
		if(tiles[x_end].tile_r == DSP2)   { stillNeed.dsp2   -= num_per_tile.dsp2*h; }
		//cout << "stillNeed.lut    = " << stillNeed.lut << endl;
		//cout << "stillNeed.bram18 = " << stillNeed.bram18 << endl;
		//cout << "stillNeed.dsp2   = " << stillNeed.dsp2 << endl << endl;
		//cout << endl;
	}
	// Increase w by 2 to avoid put BRAM around the boarder
	prr.w += 3;
	// if w/h is too large, give up this placement
	if(prr.w>WH_RATIO*prr.h) { return false; }

	// if w/h is too small, increase w to a certain number
	if(HW_RATIO*prr.h>prr.w) {
		prr.w = HW_RATIO*prr.h;
		prr.w = prr.w+prr.x < (int) tiles.size()-1 ?  prr.w : tiles.size()-1-prr.x;
	}

	// No PR region can cross SLR
	if(prr.y <= 3 && prr.y+prr.h-1 >=4){ return false; }

	if(checkInvalid){
		// if all the resource requirements are met, return true
		for(auto ishape: invalid_tiles){
			// if overlap, return false immediately
			if(this->tiles_overlap(prr, ishape)){ return false; }
		}
	}

	// prr.w--;
	return true;
}


void hipr::place_operator(Dfx & op, bool isSeqSA){
	Res res_req = op.resource;
	Region prr=op.placement;
	for(prr.h=1; prr.h+prr.y-1 < this->max_row; prr.h++){
		if(this->find_tile_range(res_req, prr)){
			op.placement = prr;
			return;
		}
	}

	if(isSeqSA){
		prr.x = 0;
		prr.y = 0;
		for(prr.h=1; prr.h+prr.y-1 < this->max_row; prr.h++){
			if(this->find_tile_range(res_req, prr, false)){
				op.placement = prr;
				return;
			}
		}
	}else{
		for(int i=0; i<RETRY_NUM; i++){
			prr.x = rand()%tiles.size();
			prr.y = rand()%this->max_row;
			for(prr.h=1; prr.h+prr.y-1 < this->max_row; prr.h++){
				if(this->find_tile_range(res_req, prr, true)){
					op.placement = prr;
					return;
				}
			}
		}
	}
	cout << "Impossible to place " << op.name << " at (" << prr.x  << "," << prr.y << ")" << endl;
	cout << "Try to increase RETRY_NUM larger than " << RETRY_NUM << endl;
	exit(FAILURE);
}

double hipr::cost_function(bool debug){
	double cost = 0;
	vector <int> cost_table[max_row];

	// initialize the cost table to -1 as it can be used once.
	for (int row=0; row<max_row; row++){
		for (uint col=0; col<tiles.size(); col++){
			cost_table[row].push_back(-1);
		}
	}


	// increase the cost table for that file if it is used by some p-block
	for(uint i=0; i<dfxs.size(); i++){
		for(int row=dfxs[i].placement.y; row<dfxs[i].placement.y+dfxs[i].placement.h; row++){
			for(int col=dfxs[i].placement.x; col<dfxs[i].placement.x+dfxs[i].placement.w; col++){
				cost_table[row][col]++;
			}
		}
	}

	// test for overlapping
	int overlap = 0;
	for(int row=0; row<this->max_row; row++){
		for(uint col=0; col<tiles.size(); col++){
			if(cost_table[row][col] > 0){ overlap += cost_table[row][col]; }
		}
	}

	// cost for wire lengths
	double WLcost=0;
	for(unsigned int i=0; i<connects.size(); i++){
		int src_num=dfxs_dict[connects[i].src];
		int dest_num=dfxs_dict[connects[i].dest];
		double manhattan_x=0, manhattan_y=0, b = connects[i].width;
		double dcx1, dcx2, dcy1, dcy2;
		if(connects[i].src == "DMA"){
			dcx1 = DMA_DCX1;
			dcy1 = DMA_DCY1;
			dcx2 = (double)dfxs[dest_num].placement.x + ((double)dfxs[dest_num].placement.w)/2;
			dcy2 = (double)dfxs[dest_num].placement.y + ((double)dfxs[dest_num].placement.h)/2;
		}else if(connects[i].dest == "DMA"){
			dcx1 = (double)dfxs[src_num].placement.x + ((double)dfxs[src_num].placement.w)/2;
			dcy1 = (double)dfxs[src_num].placement.y + ((double)dfxs[src_num].placement.h)/2;
			dcx2 = DMA_DCX2;
			dcy2 = DMA_DCY2;
		}else{

			dcx1 = (double)dfxs[src_num].placement.x  + ((double)dfxs[src_num].placement.w)/2;
			dcy1 = (double)dfxs[src_num].placement.y  + ((double)dfxs[src_num].placement.h)/2;
			dcx2 = (double)dfxs[dest_num].placement.x + ((double)dfxs[dest_num].placement.w)/2;
			dcy2 = (double)dfxs[dest_num].placement.y + ((double)dfxs[dest_num].placement.h)/2;
		}
		manhattan_x = dcx1-dcx2 > 0 ? dcx1-dcx2 : dcx2-dcx1;
		manhattan_y = dcy1-dcy2 > 0 ? dcy1-dcy2 : dcy2-dcy1;
		WLcost += (manhattan_x*frameW+manhattan_y*frameH)*b;
	}
	WLcost = WLcost / connects.size() / (tiles.size()*frameW+max_row*frameH) / MAX_DATAWIDTH;



	// cost for wastage resources
	Res resource_wastage;
	for(uint n=0; n<dfxs.size(); n++){
		Res resource = this->return_resource(dfxs[n].placement);
		resource_wastage.lut    += resource.lut    - dfxs[n].resource.lut;
		resource_wastage.bram18 += resource.bram18 - dfxs[n].resource.bram18;
		resource_wastage.dsp2   += resource.dsp2   - dfxs[n].resource.dsp2;
	}
	double Rcost=0;
	Rcost= (resource_wastage.lut+resource_wastage.bram18+resource_wastage.dsp2)/(device_resource.lut+device_resource.bram18+device_resource.dsp2);



	// cost for peripheral length
	double Pcost=0;
	for(uint n=0; n<dfxs.size(); n++){
		double width  = dfxs[n].placement.w;
		double height = dfxs[n].placement.h;
		Pcost += (width*frameW+height*frameH)/(tiles.size()*frameW+max_row*frameH)/dfxs.size();
	}


	cost = overlap + q_WLcost*WLcost + q_Pcost*Pcost + q_Rcost*Rcost;// + this->return_total_dest()*W_DEST;
	return cost;
}

Res hipr::return_resource(Region shape){
	Res out;
	for(int x=shape.x; x<shape.x+shape.w; x++){
		if(tiles[x].tile_l == CLB)    { out.lut    += num_per_tile.lut*shape.h; }
		if(tiles[x].tile_l == BRAM18) { out.bram18 += num_per_tile.bram18*shape.h; }
		if(tiles[x].tile_l == DSP2)   { out.dsp2   += num_per_tile.dsp2*shape.h; }
		if(tiles[x].tile_r == CLB)    { out.lut    += num_per_tile.lut*shape.h; }
		if(tiles[x].tile_r == BRAM18) { out.bram18 += num_per_tile.bram18*shape.h; }
		if(tiles[x].tile_r == DSP2)   { out.dsp2   += num_per_tile.dsp2*shape.h; }
	}
	for(auto ishape:invalid_tiles){
		for(int row=ishape.y; row<ishape.y+ishape.h; row++){
			for(int col=ishape.x; col<ishape.x+ishape.w; col++){
				if(row>=shape.y && row<(shape.y+shape.h) && col>=shape.x && col<(shape.x+shape.w)){
					if(tiles[col].tile_l == CLB)    { out.lut    -= num_per_tile.lut; }
					if(tiles[col].tile_l == BRAM18) { out.bram18 -= num_per_tile.bram18; }
					if(tiles[col].tile_l == DSP2)   { out.dsp2   -= num_per_tile.dsp2; }
					if(tiles[col].tile_r == CLB)    { out.lut    -= num_per_tile.lut; }
					if(tiles[col].tile_r == BRAM18) { out.bram18 -= num_per_tile.bram18; }
					if(tiles[col].tile_r == DSP2)   { out.dsp2   -= num_per_tile.dsp2; }
				}
			}
		}
	}
	return out;
}

void hipr:: report_utilization(void){
	Res sum;
	Region shape;
	for (Dfx op: dfxs){
		shape.x = op.placement.x;
		shape.y = op.placement.y;
		shape.w = op.placement.w;
		shape.h = op.placement.h;
		Res op_res = this->return_resource(shape);
		sum.lut    += op_res.lut;
		sum.bram18 += op_res.bram18;
		sum.dsp2   += op_res.dsp2;
	}
	shape.x = 0;
	shape.y = 0;
	shape.w = tiles.size();
	shape.h = this->max_row;
	Res total = this->return_resource(shape);
	cout << "LUT  Utilization: " << sum.lut/total.lut << endl;
	cout << "BRAM Utilization: " << sum.bram18/total.bram18 << endl;
	cout << "DSP2 Utilization: " << sum.dsp2/total.dsp2 << endl;

}

bool hipr::tiles_overlap(Region shape1, Region shape2){
	Point l1, r1, l2, r2;
	l1.x = shape1.x;
	l1.y = shape1.y;
	r1.x = shape1.x + shape1.w - 1;
	r1.y = shape1.y + shape1.h - 1;
	l2.x = shape2.x;
	l2.y = shape2.y;
	r2.x = shape2.x + shape2.w - 1;
	r2.y = shape2.y + shape2.h - 1;

	int x_dist = min(r1.x, r2.x)
				- max(l1.x, l2.x)+1;
	int y_dist = (min(r1.y, r2.y)
				- max(l1.y, l2.y))+1;
	if( x_dist > 0 && y_dist > 0 ){ return true;  }
	else                          { return false; }
}

void hipr::write_position(){
	string position_path = "./position.txt";
	const char *cstr = position_path.c_str();
	ofstream myfile;
	myfile.open (cstr);

	myfile << "name is_placed x y w h" << endl;
	for(uint i=0; i<dfxs.size(); i++){
		myfile << dfxs[i].name << " ";
		myfile << 1 << " ";
		myfile << dfxs[i].placement.x << " ";
		myfile << dfxs[i].placement.y << " ";
		myfile << dfxs[i].placement.w << " ";
		myfile << dfxs[i].placement.h << endl;

	}

	for(uint i=0; i<h_slices.size(); i++){
		myfile << "hhhh" << to_string(i) << " ";
		myfile << 1 << " ";
		myfile << h_slices[i].shape.x << " ";
		myfile << h_slices[i].shape.y << " ";
		myfile << h_slices[i].shape.w << " ";
		myfile << h_slices[i].shape.h << endl;
	}


	myfile.close();
}

void hipr::seq_gen_graph(Graph &G_V, Graph &G_H, vector<position_seq> position_info){
	int init_weight = 0;
	// add edges according to the relative positions
	for(unsigned int i=0; i<dfxs.size(); i++){
		if (position_info[i].left.size() == 0) { this->seq_addEdge(G_H, 0, i+1, init_weight); }
		if (position_info[i].right.size() == 0){ this->seq_addEdge(G_H, i+1, dfxs.size()+1, init_weight); }
		for(unsigned j=0; j<position_info[i].right.size(); j++){this->seq_addEdge(G_H, i+1, position_info[i].right[j]+1, init_weight); }
		if (position_info[i].below.size() == 0){ this->seq_addEdge(G_V, 0, i+1, init_weight); }
		if (position_info[i].above.size() == 0){ this->seq_addEdge(G_V, i+1, dfxs.size()+1, init_weight); }
		for(unsigned j=0; j<position_info[i].above.size(); j++){this->seq_addEdge(G_V, i+1, position_info[i].above[j]+1, init_weight); }
	}
}

void hipr::seq_init_seq_pair(vector<int> &int_seq){
	bool need_idx = true;
	int func_idx;
	for(uint i=0; i<dfxs.size(); i++){
		// keep generating random number within function number until get no repetitive index
		need_idx = true;
		while(need_idx){
			func_idx = rand()%dfxs.size();
			// guarantee that no repetitive index is pushed into int_seq
			need_idx = false;
			for(unsigned j=0; j<int_seq.size(); j++){ if(func_idx == int_seq[j]){ need_idx = true; } }
		}
		int_seq.push_back(func_idx);
	}
}

void hipr::seq_parse_position(vector<position_seq> &position_info, vector<int> pos_seq, vector<int> neg_seq){
	for(unsigned int i=0; i<pos_seq.size(); i++){
		for(unsigned int j=0; j<i; j++){
			position_info[pos_seq[i]].pos_before.push_back(pos_seq[j]);
			position_info[neg_seq[i]].neg_before.push_back(neg_seq[j]);
		}
		for(unsigned int j=i+1; j<pos_seq.size(); j++){
			position_info[pos_seq[i]].pos_after.push_back(pos_seq[j]);
			position_info[neg_seq[i]].neg_after.push_back(neg_seq[j]);
		}
	}

	for(uint i=0; i<dfxs.size(); i++){
		for(unsigned int j=0; j<position_info[i].pos_after.size(); j++){
			for(unsigned int k=0; k<position_info[i].neg_after.size(); k++){
				if(position_info[i].pos_after[j] == position_info[i].neg_after[k]){
					position_info[i].right.push_back(position_info[i].pos_after[j]);
				}
			}
			for(unsigned int k=0; k<position_info[i].neg_before.size(); k++){
				if(position_info[i].pos_after[j] == position_info[i].neg_before[k]){
					position_info[i].below.push_back(position_info[i].pos_after[j]);
				}
			}

		}
		for(unsigned int j=0; j<position_info[i].pos_before.size(); j++){
			for(unsigned int k=0; k<position_info[i].neg_after.size(); k++){
				if(position_info[i].pos_before[j] == position_info[i].neg_after[k]){
					position_info[i].above.push_back(position_info[i].pos_before[j]);
				}
			}
		}
		for(unsigned int j=0; j<position_info[i].pos_before.size(); j++){
			for(unsigned int k=0; k<position_info[i].neg_before.size(); k++){
				if(position_info[i].pos_before[j] == position_info[i].neg_before[k]){
					position_info[i].left.push_back(position_info[i].pos_before[j]);
				}
			}
		}
	}
}

void hipr::seq_addEdge(Graph &G, int u, int v, int weight)
{
	AdjListNode node(v, weight);
	G.adj[u].push_back(node); // Add v to u's list
}

// A recursive function used by longestPath. See below
// link for details
// https:// www.geeksforgeeks.org/topological-sorting/
void hipr::seq_topologicalSortUtil(Graph &G, int v, bool visited[],
								stack<int>& Stack)
{
	// Mark the current node as visited
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	list<AdjListNode>::iterator i;
	for (i = G.adj[v].begin(); i != G.adj[v].end(); ++i) {
		AdjListNode node = *i;
		if (!visited[node.getV()])
			seq_topologicalSortUtil(G, node.getV(), visited, Stack);
	}

	// Push current vertex to stack which stores topological
	// sort
	Stack.push(v);
}

void hipr::seq_y_adjust(Dfx &op){
	for(Region t:invalid_tiles){
		Point l1, r1, l2, r2;
		l1.x = t.x;
		l1.y = t.y;
		r1.x = t.x+t.w-1;
		r1.y = t.y+t.h-1;
		l2.x = op.placement.x;
		l2.y = op.placement.y;
		r2.x = op.placement.x+op.placement.w-1;
		r2.y = op.placement.y+op.placement.h-1;
		int x_dist = min(r1.x, r2.x)
					- max(l1.x, l2.x)+1;
		int y_dist = (min(r1.y, r2.y)
					- max(l1.y, l2.y))+1;
		if( x_dist > 0 && y_dist > 0 ){
			if(r1.y + op.placement.h <= max_row) {
				op.placement.y = r1.y+1;
			}else{
				op.placement.y = 0;
			}
		}
	}
	for(Region t:invalid_tiles){
		Point l1, r1, l2, r2;
		l1.x = t.x;
		l1.y = t.y;
		r1.x = t.x+t.w-1;
		r1.y = t.y+t.h-1;
		l2.x = op.placement.x;
		l2.y = op.placement.y;
		r2.x = op.placement.x+op.placement.w-1;
		r2.y = op.placement.y+op.placement.h-1;
		int x_dist = min(r1.x, r2.x)
					- max(l1.x, l2.x)+1;
		int y_dist = (min(r1.y, r2.y)
					- max(l1.y, l2.y))+1;
		if( x_dist > 0 && y_dist > 0 ){
			if(r1.y + op.placement.h <= max_row) {
				op.placement.y = r1.y+1;
			}else{
				op.placement.y = 0;
			}
		}
	}
}

// The function to find longest distances from a given vertex.
// It uses recursive topologicalSortUtil() to get topological
// sorting.
void hipr::seq_longestPath(Graph &G, int s, string dir)
{
	stack<int> Stack;
	int dist[G.V];

	// Mark all the vertices as not visited
	bool* visited = new bool[G.V];
	for (int i = 0; i < G.V; i++)
		visited[i] = false;

	// Call the recursive helper function to store Topological
	// Sort starting from all vertices one by one
	for (int i = 0; i < G.V; i++)
		if (visited[i] == false)
			seq_topologicalSortUtil(G, i, visited, Stack);

	// Initialize distances to all vertices as infinite and
	// distance to source as 0
	for (int i = 0; i < G.V; i++)
		dist[i] = NINF;
	dist[s] = 0;
	// Process vertices in topological order
	while (Stack.empty() == false) {
		// Get the next vertex from topological order
		int u = Stack.top();
		Stack.pop();
		int new_weight=0;
		if (u!=0 && u!=(int)dfxs.size()+1){
			if( dir=="H"){
				// for horizontal graph, update the start column with dist[u]
				// then, find_tile_range() will update the end column automatically
				int node_num = u-1;
				dfxs[node_num].placement.x = dist[u] < (int) this->tiles.size() ? dist[u] : 0;
				this->place_operator(dfxs[node_num], true);
				new_weight = dfxs[node_num].placement.w;
			}else{
				// for vertical graph, fist backup the height
				// then, update the row number according to dist[u]
				int node_num = u-1;
				dfxs[node_num].placement.y = dist[u]+dfxs[node_num].placement.h > max_row ? 0 : dist[u];
				int pre_y = dfxs[node_num].placement.y;
				this->seq_y_adjust(dfxs[node_num]);
				new_weight = dfxs[node_num].placement.h+dfxs[node_num].placement.y-pre_y;
			}
		}
		// Update distances of all adjacent vertices
		list<AdjListNode>::iterator i;
		if (dist[u] != NINF) {
			for (i = G.adj[u].begin(); i != G.adj[u].end(); ++i){
				if (u != 0){ i->setWeight(new_weight); }
				if (dist[i->getV()] < dist[u] + i->getWeight())
					dist[i->getV()] = dist[u] + i->getWeight();
			}
		}
	}

	delete [] visited;
}

void hipr::seq_floorplan(vector<int> pos_seq, vector<int> neg_seq){
	// extract the relative positions among different DFXs
	vector<position_seq> position_info(dfxs.size());
	this->seq_parse_position(position_info, pos_seq, neg_seq);


	// Construct vertical and horizontal graph
	Graph G_H(dfxs.size()+2);
	Graph G_V(dfxs.size()+2);
	this->seq_gen_graph(G_V, G_H, position_info);


	// determine the x and y coordinates according to the longest path algorithm
	// (https://www.geeksforgeeks.org/find-longest-path-directed-acyclic-graph/)
	this->seq_longestPath(G_H, 0, "H");
	this->seq_longestPath(G_V, 0, "V");
}

unsigned int hipr::seq_return_vector_idx(vector<int> vector_in, int target){
	for(unsigned i=0; i<vector_in.size(); i++){
		if(target == vector_in[i]) return i;
	}
	return 0;
}

void hipr::seq_swap(int swap_case, int func_idx1, int func_idx2,
					vector<int> &pos_seq, vector<int> &neg_seq){
	unsigned int seq_idx1=0, seq_idx2=0;
	if(swap_case == 0){
		seq_idx1 = this->seq_return_vector_idx(pos_seq, func_idx1);
		seq_idx2 = this->seq_return_vector_idx(pos_seq, func_idx2);
		unsigned int tmp = pos_seq[seq_idx1];
		pos_seq[seq_idx1] = pos_seq[seq_idx2];
		pos_seq[seq_idx2] = tmp;
	}else if(swap_case == 1){
		seq_idx1 = this->seq_return_vector_idx(neg_seq, func_idx1);
		seq_idx2 = this->seq_return_vector_idx(neg_seq, func_idx2);
		unsigned int tmp = neg_seq[seq_idx1];
		neg_seq[seq_idx1] = neg_seq[seq_idx2];
		neg_seq[seq_idx2] = tmp;
	}else{
		seq_idx1 = this->seq_return_vector_idx(pos_seq, func_idx1);
		seq_idx2 = this->seq_return_vector_idx(pos_seq, func_idx2);
		unsigned int tmp = pos_seq[seq_idx1];
		pos_seq[seq_idx1] = pos_seq[seq_idx2];
		pos_seq[seq_idx2] = tmp;
		seq_idx1 = this->seq_return_vector_idx(neg_seq, func_idx1);
		seq_idx2 = this->seq_return_vector_idx(neg_seq, func_idx2);
		tmp = neg_seq[seq_idx1];
		neg_seq[seq_idx1] = neg_seq[seq_idx2];
		neg_seq[seq_idx2] = tmp;
	}
}

void hipr::seq_print_out(vector<int> vector_in, string name){
	cout << name << ": " << setw(3) << vector_in[0];
	for(unsigned int i=1; i<vector_in.size(); i++){ cout << ", " << setw(3) << vector_in[i]; }
	cout << endl;
}




// initialized the resource arrays of the whole devide
//void hipr::milp_init_resource_array(vector<Grid> resource_array[]){
//	// init array values
//	for(int r=0; r<max_row; r++){
//		for(unsigned int j=0; j<tiles.size(); j++){
//			Grid t;
//			if(tiles[j].tile_l != CLB)      { t.resource_name = tiles[j].tile_l; }
//			else if(tiles[j].tile_r != CLB ){ t.resource_name = tiles[j].tile_r; }
//			else                            { t.resource_name = CLB; }
//			resource_array[r].push_back(t);
//		}
//	}
//
//	// Set the forbidden areas
//	for(unsigned i=0; i<invalid_tiles.size(); i++){
//		for(int r=invalid_tiles[i].y; r<invalid_tiles[i].y+invalid_tiles[i].h; r++){
//			for(int c=invalid_tiles[i].x; c<invalid_tiles[i].x+invalid_tiles[i].w; c++){
//				resource_array[r][c].valid = false;
//				resource_array[r][c].resource_name = FORB;
//			}
//		}
//	}
//}
//
//
//bool hipr::milp_find_rectangle(vector<Grid> resource_array[], Partition &rect){
//	Res_type res_name=resource_array[(rect).r1][(rect).c1].resource_name;
//	rect.name = res_name;
//	if(resource_array[rect.r1][rect.c1].used) {return false; }
//
//	// find the maximum row with the same type
//	for(int r=rect.r1; r<max_row; r++){
//		if(res_name != resource_array[r][rect.c1].resource_name || resource_array[r][rect.c1].used){
//			rect.r2 = r-1;
//			break;
//		}else{ rect.r2 = r; }
//	}
//
//	// find the maximum column with the same type
//	for(int r=rect.r1; r<=rect.r2; r++){
//		for(uint c= rect.c1; c<tiles.size(); c++){
//			if(res_name != resource_array[r][c].resource_name || resource_array[r][rect.c1].used){
//				rect.c2 = c-1;
//				break;
//			}else{ rect.c2 = c; }
//		}
//	}
//	return true;
//}
//
//// divide the device into partitions with the same resources
//void hipr::milp_find_partition(vector<Grid> resource_array[], vector<Partition> &part){
//	Partition cur_rect;
//	cur_rect.r1 = 0; cur_rect.r2 = 0; cur_rect.c1 = 0; cur_rect.c2 = 0;
//	for(uint c=0; c<tiles.size(); c++){
//		for(int r=0; r<max_row; r++){
//			cur_rect.r1 = r;
//			cur_rect.c1 = c;
//			if(this->milp_find_rectangle(resource_array, cur_rect)){
//				for(int i=cur_rect.r1; i<=cur_rect.r2; i++){
//					for(int j=cur_rect.c1; j<=cur_rect.c2; j++){
//						resource_array[i][j].used = true;
//					}
//				}
//				part.push_back(cur_rect);
//			}
//		}
//	}
//
//
//	string position_path = "/home/ylxiao/eclipse-workspace/hipr_20220426/position.txt";
//	const char *cstr = position_path.c_str();
//	ofstream myfile;
//	myfile.open (cstr);
//
//	myfile << "name is_placed x y w h" << endl;
//	for(uint i=0; i<part.size(); i++){
//		myfile << res_dict[part[i].name]+"_"+to_string(i) << " ";
//		myfile << 1 << " ";
//		myfile << part[i].c1 << " ";
//		myfile << part[i].r1 << " ";
//		myfile << part[i].c2-part[i].c1+1 << " ";
//		myfile << part[i].r2-part[i].r1+1 << endl;
//
//	}
//	myfile.close();
//}
//
//
//bool hipr::milp(bool debug){
//	vector<Grid> resource_array[max_row];
//	this->milp_init_resource_array(resource_array);
//
//	vector<Partition> part;
//	this->milp_find_partition(resource_array, part);
//
//
//	vector<unsigned int> forb_part_idx; // forbidden areas
//	for(unsigned int p=0; p<part.size(); p++){
//		if(part[p].name == FORB){ forb_part_idx.push_back(p); }
//	}
//
//
//	bool rp[part.size()][max_row];
//	for (unsigned int p=0; p<part.size(); p++){
//		for(int r=0; r<max_row; r++){ rp[p][r] = 0; }
//	}
//
//	for (unsigned int p=0; p<part.size(); p++){
//		for(int r=part[p].r1; r<=part[p].r2; r++){ rp[p][r] = 1; }
//	}
//
//	int x1[part.size()], x2[part.size()];
//	for(unsigned int p=0; p<part.size(); p++){
//		x1[p] = part[p].c1;
//		x2[p] = part[p].c2;
//	}
//
//
//	Res d[part.size()];
//	for(unsigned int p=0; p<part.size(); p++){
//		if(part[p].name == CLB)   { d[p].lut = num_per_tile.lut*2; }
//		if(part[p].name == BRAM18){ d[p].bram18 = num_per_tile.bram18;  d[p].lut = num_per_tile.lut; }
//		if(part[p].name == DSP2)  { d[p].dsp2 = num_per_tile.dsp2;      d[p].lut = num_per_tile.lut; }
//		if(part[p].name == URAM)  { d[p].lut = num_per_tile.lut; }
//		if(part[p].name == IOB)   { d[p].lut = num_per_tile.lut; }
//		if(part[p].name == PCIE)  { d[p].lut = num_per_tile.lut; }
//	}
//
//	try {
//
//		// Create an environment
//		GRBEnv env = GRBEnv(true);
//		env.set("LogFile", "mip1.log");
//		env.start();
//
//		// Create an empty model
//		GRBModel model = GRBModel(env);
//
//		// Create variables
//		GRBVar a[dfxs.size()][max_row];
//		for(uint i=0; i<dfxs.size(); i++){
//			for(int j=0; j<max_row; j++){
//				ostringstream name;
//				name << "an" << i << "r" << j;
//				a[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//			}
//		}
//
//		GRBVar x[dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			ostringstream name;
//			name << "xn" << i;
//			x[i] = model.addVar(0.0, tiles.size()-1, 0.0, GRB_INTEGER, name.str());
//		}
//
//
//		GRBVar w[dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			ostringstream name;
//			name << "wn" <<i;
//			w[i] = model.addVar(1.0, tiles.size(), 0.0, GRB_INTEGER, name.str());
//		}
//
//
//		GRBVar yl[dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			ostringstream name;
//			name << "yln" << i;
//			yl[i] = model.addVar(0.0, max_row-1, 0.0, GRB_INTEGER, name.str());
//		}
//
//
//		GRBVar yh[dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			ostringstream name;
//			name << "yhn" << i;
//			yh[i] = model.addVar(0.0, max_row-1, 0.0, GRB_INTEGER, name.str());
//		}
//
//
//		GRBVar h[dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			ostringstream name;
//			name << "hn" << i;
//			h[i] = model.addVar(1.0, max_row, 0.0, GRB_INTEGER, name.str());
//		}
//
//
//		GRBVar l[dfxs.size()][part.size()][max_row];
//		for(uint n=0; n<dfxs.size(); n++){
//			for(unsigned int p=0; p<part.size(); p++){
//				for(int r=0; r<max_row; r++){
//					ostringstream name;
//					name << "ln" << n << "p" << p << "r" << r;
//					l[n][p][r] = model.addVar(0.0, tiles.size(), 0.0, GRB_INTEGER, name.str());
//				}
//			}
//		}
//
//
//		GRBVar k[dfxs.size()][part.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			for(unsigned int j=0; j<part.size(); j++){
//				ostringstream name;
//				name << "kn" << i << "p" << j;
//				k[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//			}
//		}
//
//
//		GRBVar g[dfxs.size()][dfxs.size()];
//		for(uint i=0; i<dfxs.size(); i++){
//			for(uint j=0; j<dfxs.size(); j++){
//				ostringstream name;
//				name << "gn" << i << "n" << j;
//				g[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//			}
//		}
//
//		//============================================================================================
//		GRBLinExpr lhs;
//		// add constraints
//		this->load_init();
//
//#ifdef PSUEDO
//		for(uint n=0; n<dfxs.size(); n++){
//			model.addConstr(x[n]  == dfxs[n].placement.x);
//			model.addConstr(yl[n] == dfxs[n].placement.y);
//			model.addConstr(w[n]  == dfxs[n].placement.w);
//			model.addConstr(yh[n] == dfxs[n].placement.y+dfxs[n].placement.h-1);
//		}
//#endif
//
//
//#ifdef INIT
//		for(uint n=0; n<dfxs.size(); n++){
//			x[n].set(GRB_DoubleAttr_Start, dfxs[n].placement.x);
//			w[n].set(GRB_DoubleAttr_Start, dfxs[n].placement.w);
//			yl[n].set(GRB_DoubleAttr_Start, dfxs[n].placement.y);
//			yh[n].set(GRB_DoubleAttr_Start, dfxs[n].placement.y+dfxs[n].placement.h-1);
//		}
//#endif
//
//		// Row contiguity constraints
//		// a[n][r2] >= a[n][r1]+a[n][r3]-1
//		for (uint n=0; n<dfxs.size(); n++) {
//			for(int r1=0; r1<max_row; r1++){
//				for(int r2=r1+1; r2<max_row; r2++){
//					for(int r3=r2+1; r3<max_row; r3++){
//						lhs = a[n][r2] - a[n][r1] - a[n][r3] + 1;
//						model.addConstr(lhs >= 0.0);
//					}
//				}
//			}
//		}
//
//		// on the left constraints
//		for(uint n=0; n<dfxs.size(); n++){
//			for(unsigned int p=0; p<part.size(); p++){
//				ostringstream cname;
//				cname << "KCONSn" << n << "p" << p;
//				lhs = x[n]+w[n]-x1[p]*k[n][p]-1;
//				model.addConstr(lhs >= 0.0, cname.str());
//			}
//		}
//
//		// intersect constraints
//		for(uint n=0; n<dfxs.size(); n++){
//			for(unsigned int p=0; p<part.size(); p++){
//				ostringstream cname;
//				cname << "SECCONSn" << n << "p" << p;
//				lhs = x[n]-x2[p]-(1-k[n][p])*(tiles.size()-1-x2[p]);
//				model.addConstr(lhs <= 0.0, cname.str());
//			}
//		}
//
//		// width constraints
//		for(uint n_idx=0; n_idx<dfxs.size(); n_idx++){
//			ostringstream cname;
//			cname << "widthCONSn" << n_idx;
//			lhs = x[n_idx]+w[n_idx];
//			model.addConstr(lhs <= tiles.size(), cname.str());
//		}
//
//		// height constraints
//		for(uint n=0; n<dfxs.size(); n++){
//			ostringstream cname;
//			cname << "heightCONSn" << n;
//			lhs = 0;
//			for(int r=0; r<max_row; r++){
//				lhs += a[n][r];
//			}
//			model.addConstr(lhs == h[n], cname.str());
//		}
//
//		// yln <= R && yln <= r (occupied)
//		for(uint n=0; n<dfxs.size(); n++){
//			for(int r=0; r<max_row; r++){
//				ostringstream cname;
//				cname << "ylyhCONSn" << n << "r" << r;
//				model.addConstr(yl[n] <=(max_row-1) - a[n][r]*((max_row-1)-r), cname.str());
//			}
//		}
//
//		// yh
//		for(uint n=0; n<dfxs.size(); n++){
//			for(int r=0; r<max_row; r++){
//				ostringstream cname;
//				cname << "yhCONSn" << n << "r" << r;
//				model.addConstr(yh[n] >= a[n][r]*r, cname.str());
//			}
//		}
//
//		// yh[n]-yl[n]+1==h[n]
//		for(uint n=0; n<dfxs.size(); n++){ model.addConstr(yh[n]-yl[n]+1==h[n]); }
//
//		// the projection cannot be greater than the width of Ps
//		for(uint n=0; n<dfxs.size(); n++){
//			for(uint p=0; p<part.size(); p++){
//				for(int r=0; r<max_row; r++){
//					if(rp[p][r] == 1){
//						model.addConstr(l[n][p][r] <= a[n][r]*(x2[p]-x1[p]+1));
//						model.addConstr(l[n][p][r] <= k[n][p]*(x2[p]-x1[p]+1));
//						model.addConstr(l[n][p][r] <= w[n]);
//						model.addConstr(l[n][p][r] <= x[n]+w[n]-k[n][p]*x1[p]);
//						model.addConstr(l[n][p][r] <= x2[p]-x[n]+1+(1-k[n][p])*(tiles.size()-1-x2[p]));
//					}
//				}
//			}
//		}
//
//
//		// projection overlay should be greater than w[n]
//		for(uint n=0; n<dfxs.size(); n++){
//			for(int r=0; r<max_row; r++){
//				lhs = 0;
//				for(unsigned int p=0; p<part.size(); p++){
//					if(rp[p][r] == 1){
//						lhs += l[n][p][r];
//					}
//				}
//				model.addConstr(lhs >= w[n]-(1-a[n][r])*tiles.size());
//			}
//		}
//
//		// For forbidden areas
//		for(uint n=0; n<dfxs.size(); n++){
//			for(int r=0; r<max_row; r++){
//				for(unsigned int i=0; i<forb_part_idx.size(); i++){
//					if(rp[forb_part_idx[i]][r] == 1){ model.addConstr(l[n][forb_part_idx[i]][r] == 0.0); }
//				}
//			}
//		}
//
//		for(uint n1=0; n1<dfxs.size(); n1++){
//			for(uint n2=n1+1; n2<dfxs.size(); n2++){
//				model.addConstr(x[n1] + w[n1] <= x[n2] + g[n1][n2]*tiles.size());
//			}
//		}
//
//		for(uint n=0; n<dfxs.size(); n++){
//			GRBLinExpr lhs_lut    = 0;
//			GRBLinExpr lhs_bram18 = 0;
//			GRBLinExpr lhs_dsp2   = 0;
//			for(unsigned int p=0; p<part.size(); p++){
//				for(int r=0; r<max_row; r++){
//					if(rp[p][r] == 1){
//						lhs_lut    += l[n][p][r]*d[p].lut;
//						lhs_bram18 += l[n][p][r]*d[p].bram18;
//						lhs_dsp2   += l[n][p][r]*d[p].dsp2;
//					}
//
//				}
//			}
//			model.addConstr(lhs_lut    >= dfxs[n].resource.lut);
//			model.addConstr(lhs_bram18 >= dfxs[n].resource.bram18);
//			model.addConstr(lhs_dsp2   >= dfxs[n].resource.dsp2);
//		}
//
//		for(int r=0; r<max_row; r++){
//			for(uint n1=0; n1<dfxs.size(); n1++){
//				for(uint n2=n1+1; n2<dfxs.size(); n2++){
//					model.addConstr(x[n1] >= x[n2]+w[n2]-(3-g[n1][n2]-a[n1][r]-a[n2][r])*tiles.size());
//					//model.addConstr(x[n1] >= 1+x[n2]+w[n2]-(3-g[n1][n2]-a[n1][r]-a[n2][r])*(tiles.size()+1));
//				}
//			}
//		}
//
//
//		// cost for wire lengths
//		GRBLinExpr dcx[dfxs.size()], dcy[dfxs.size()];
//		GRBVar manhattan_x[connects.size()], manhattan_y[connects.size()];
//		for(uint i=0; i<connects.size(); i++){
//			manhattan_x[i] = model.addVar(0.0, tiles.size(), 0.0, GRB_CONTINUOUS);
//			manhattan_y[i] = model.addVar(0.0, max_row, 0.0, GRB_CONTINUOUS);
//		}
//		GRBLinExpr WLcost = 0;
//
//		for(uint i=0; i<dfxs.size(); i++){
//			dcx[i] = x[i]  + w[i]/2;
//			dcy[i] = (yh[i]+yl[i]+1)/2;
//		}
//
//		for(unsigned int i=0; i<connects.size(); i++){
//			int src_num=dfxs_dict[connects[i].src];
//			int dest_num=dfxs_dict[connects[i].dest];
//			double b = connects[i].width;
//			GRBLinExpr dcx1=0, dcy1=0, dcx2=0, dcy2=0;
//			if(connects[i].src == "DMA"){
//				dcx1 = DMA_DCX1;
//				dcy1 = DMA_DCY1;
//				dcx2 = dcx[dest_num];
//				dcy2 = dcy[dest_num];
//			}else if(connects[i].dest == "DMA"){
//				dcx1 = dcx[src_num];
//				dcy1 = dcy[src_num];
//				dcx2 = DMA_DCX2;
//				dcy2 = DMA_DCY2;
//			}else{
//				dcx1 = dcx[src_num];
//				dcy1 = dcy[src_num];
//				dcx2 = dcx[dest_num];
//				dcy2 = dcy[dest_num];
//			}
//			model.addConstr(manhattan_x[i] >= dcx1-dcx2);
//			model.addConstr(manhattan_x[i] >= dcx2-dcx1);
//			model.addConstr(manhattan_y[i] >= dcy1-dcy2);
//			model.addConstr(manhattan_y[i] >= dcy2-dcy1);
//			WLcost += (manhattan_x[i]*frameW+manhattan_y[i]*frameH)*b;
//		}
//		WLcost = WLcost / connects.size() / (tiles.size()*frameW+max_row*frameH) / MAX_DATAWIDTH;
//
//		// Cost function for peripheral length
//		GRBLinExpr Pcost = 0;
//		for(uint n=0; n<dfxs.size(); n++){
//			Pcost += (w[n]*frameW+h[n]*frameH)/(tiles.size()*frameW+max_row*frameH)/dfxs.size();
//		}
//
//		// Cost function for the resource wastage
//		GRBLinExpr Rcost = 0;
//		GRBLinExpr lhs_lut    = 0;
//		GRBLinExpr lhs_bram18 = 0;
//		GRBLinExpr lhs_dsp2   = 0;
//		for(uint n=0; n<dfxs.size(); n++){
//			for (uint p=0; p<part.size(); p++){
//				for(int r=0; r<max_row; r++){
//					if(rp[p][r] == 1){
//						lhs_lut    += l[n][p][r]*d[p].lut;
//						lhs_bram18 += l[n][p][r]*d[p].bram18;
//						lhs_dsp2   += l[n][p][r]*d[p].dsp2;
//					}
//				}
//			}
//			lhs_lut    -= dfxs[n].resource.lut;
//			lhs_bram18 -= dfxs[n].resource.bram18;
//			lhs_dsp2   -= dfxs[n].resource.dsp2;
//		}
//
//		Rcost = (lhs_lut+lhs_bram18+lhs_dsp2)/(device_resource.lut+device_resource.bram18+device_resource.dsp2);
//
//		GRBLinExpr final_obj = 0;
//
//		final_obj = q_WLcost*WLcost + q_Pcost*Pcost +q_Rcost*Rcost;
//		//final_obj = Pcost;
//		// final_obj = Rcost;
//		model.setObjective(final_obj, GRB_MINIMIZE);
//
//
//
//
//
//		// Optimize model
//		//model.set(GRB_DoubleParam_TimeLimit, MAX_TIME);
//		//model.set(GRB_DoubleParam_BestObjStop, 0.10166);
//		//model.set(GRB_DoubleParam_MIPGap, 0.65);
//
//		cout << "begin opt" << endl;
//		model.optimize();
//		cout << "finish opt" << endl;
//
//		int optimstatus = model.get(GRB_IntAttr_Status);
//		if (optimstatus != GRB_OPTIMAL){
//			return false;
//		}
//		cout << "======================================" << endl;
//		cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
//		cout << std::left;
//
//		cout << "\n==================" << endl;
//		cout << "n   x   y   w   h" << endl;
//		for (uint n=0; n<dfxs.size(); n++){
//			cout << std::setw (4) << n;
//			cout << std::setw (4) << (int) x[n].get(GRB_DoubleAttr_X);
//			dfxs[n].placement.x = (int) x[n].get(GRB_DoubleAttr_X);
//			cout << std::setw (4) << (int) yl[n].get(GRB_DoubleAttr_X);
//			dfxs[n].placement.y = (int) yl[n].get(GRB_DoubleAttr_X);
//			cout << std::setw (4) << (int) w[n].get(GRB_DoubleAttr_X);
//			dfxs[n].placement.w = (int) w[n].get(GRB_DoubleAttr_X);
//			cout << std::setw (4) << (int) yh [n].get(GRB_DoubleAttr_X) - (int) yl[n].get(GRB_DoubleAttr_X) + 1 << endl;
//			dfxs[n].placement.h = (int) yh[n].get(GRB_DoubleAttr_X) - (int) yl[n].get(GRB_DoubleAttr_X) + 1;
//		}
//
//		this->print_dfxs();
//		this->write_position();
//		return true;
//
//	} catch(GRBException e) {
//	cout << "Error code = " << e.getErrorCode() << endl;
//	cout << e.getMessage() << endl;
//	} catch(...) {
//	cout << "Exception during optimization" << endl;
//	}
//
//	return false;
//}





























































bool ascendDist(Region shape1, Region shape2){
	return shape1.dis < shape2.dis;
}

bool ascendRes(Region shape1, Region shape2){
	return shape1.r_wastage < shape2.r_wastage;
}


void hipr::print_pragma(void){
	cout << "\n\n============ pragma" << endl;
	for(uint i=0; i<pragmas.size(); i++){
		cout << pragmas[i].i << ": " ;
		cout << std::left << std::setw (20) << pragmas[i].name;
		cout << std::setw (4) << pragmas[i].resource.lut;
		cout << std::setw (4) << pragmas[i].resource.ff;
		cout << std::setw (4) << pragmas[i].resource.bram18;
		cout << std::setw (4) << pragmas[i].resource.dsp2 << endl;
	}
	cout << "============ end of pragma\n\n" << endl;
}

void hipr::print_connect(void){
	cout << "\n\n============ connect" << endl;
	for(uint i=0; i<connects.size(); i++){
		cout << connects[i].i << ": " << std::left << std::setw (18);
		cout << connects[i].src << " => " << connects[i].dest << endl;
	}
	cout << "============ end of connect\n\n" << endl;
}

void hipr::print_tile(void){
	cout << "\n\n============ tile" << endl;
	for(uint i=0; i<tiles.size(); i++){
		cout << tiles[i].i << ": ";
		cout << tiles[i].tile_l << " " << tiles[i].x_l;
		cout << " " << tiles[i].tile_r << " " << tiles[i].x_r << endl;
	}
	cout << "============ end of tile\n\n" << endl;

}

void hipr::print_dfxs(void){
	cout << "\n\n============ dfx" << endl;
	cout << "# : operator          Type   LUTs   FFs    BRAM18 DSP    x   y   w   h " << endl;
	for(uint i=0; i<dfxs.size(); i++){
		Res resource = this->hipr::return_resource(dfxs[i].placement);
		cout << std::setw (2) << dfxs[i].i;
		cout << ": " << std::left << std::setw (18);
		cout << dfxs[i].name;
		cout << std::setw (7) << dfxs[i].isPR;
		cout << std::setw (7) << dfxs[i].resource.lut;
		cout << std::setw (7) << dfxs[i].resource.ff;
		cout << std::setw (7) << dfxs[i].resource.bram18;
		cout << std::setw (7) << dfxs[i].resource.dsp2;
		cout << std::setw (4) << dfxs[i].placement.x;
		cout << std::setw (4) << dfxs[i].placement.y;
		cout << std::setw (4) << dfxs[i].placement.w;
		cout << std::setw (4) << dfxs[i].placement.h << endl;
		cout << std::setw (29) << " ";
		cout << std::setw (7) << resource.lut;
		cout << std::setw (7) << " ";
		cout << std::setw (7) << resource.bram18;
		cout << std::setw (7) << resource.dsp2 << endl;
	}
	cout << "----------------------------------------------------------------------" << endl;
	for(uint i=0; i<dfxs.size(); i++){
		Res resource = this->hipr::return_resource(dfxs[i].placement);
		cout << std::left << std::setw (22);
		cout << dfxs[i].name;
		cout << std::setw (7) << dfxs[i].isPR;
		cout << std::setw (7) << resource.lut;
		cout << std::setw (7) << resource.ff;
		cout << std::setw (7) << resource.bram18;
		cout << std::setw (7) << resource.dsp2 << endl;
	}
	cout << "============ end of dfx\n\n" << endl;
}

void hipr::print_utilization(void){
	cout << "\n\n============ utilization" << endl;
	int app_num_total[3] = {0, 0, 0};
	Res resource_total;
	cout << "# : operator          LUTs   BRAM18 DSP" << endl;
	for(uint i=0; i<dfxs.size(); i++){
		Res resource = this->hipr::return_resource(dfxs[i].placement);
		cout << std::setw (2) << dfxs[i].i;
		cout << ": " << std::left << std::setw (18);
		cout << dfxs[i].name;
		cout << std::setprecision(2);
		cout << std::setw (7) << dfxs[i].resource.lut    / resource.lut;
		cout << std::setw (7) << dfxs[i].resource.bram18 / resource.bram18;
		cout << std::setw (7) << dfxs[i].resource.dsp2   / resource.dsp2 << endl;
		app_num_total[0] += dfxs[i].resource.lut;
		app_num_total[1] += dfxs[i].resource.bram18;
		app_num_total[2] += dfxs[i].resource.dsp2;
		resource_total.lut += resource.lut;
		resource_total.bram18 += resource.bram18;
		resource_total.dsp2 += resource.dsp2;
	}

	cout << std::setw (2) << "--";
	cout << ": " << std::left << std::setw (18);
	cout << "APP/Chip";
	cout << std::setprecision(2);
	cout << std::setw (7) << (double) app_num_total[0] / device_resource.lut;
	cout << std::setw (7) << (double) app_num_total[1] / device_resource.bram18;
	cout << std::setw (7) << (double) app_num_total[2] / device_resource.dsp2 << endl;

	cout << std::setw (2) << "--";
	cout << ": " << std::left << std::setw (18);
	cout << "PR/Chip";
	cout << std::setprecision(2);
	cout << std::setw (7) << (double) resource_total.lut / device_resource.lut;
	cout << std::setw (7) << (double) resource_total.bram18 / device_resource.bram18;
	cout << std::setw (7) << (double) resource_total.dsp2 / device_resource.dsp2 << endl;

	cout << "============ end of utilization\n\n" << endl;
}

void hipr::print_invalid(void){
	cout << "\n\n============ invalid tiles" << endl;
	cout << "Resource                   col_start  col_end  row_start  row_end " << endl;
	for(uint i=0; i<invalid_tiles.size(); i++){
		cout << std::left << std::setw (27);
		cout << "invalid_tiles[" << i << "]: ";
		cout << std::setw (11) << invalid_tiles[i].x;
		cout << std::setw (9) << invalid_tiles[i].y;
		cout << std::setw (11) << invalid_tiles[i].w;
		cout << std::setw (9) << invalid_tiles[i].h << endl;
	}
	cout << "============ invalid tiles\n\n" << endl;
}




void hipr::write_position(vector<string> names, vector<Region> shapes){
	string position_path = "./position.txt";
	const char *cstr = position_path.c_str();
	ofstream myfile;
	myfile.open (cstr);

	if(names.size() != shapes.size()){ cout << "names and shapes discrepency!" << endl; exit(0); }
	myfile << "name is_placed x y w h" << endl;
	for(uint i=0; i<names.size(); i++){
		myfile << names[i] << " ";
		myfile << 1 << " ";
		myfile << shapes[i].x << " ";
		myfile << shapes[i].y << " ";
		myfile << shapes[i].w << " ";
		myfile << shapes[i].h << endl;
	}
	myfile.close();
}

void hipr::write_position(Dfx op){
	string position_path = "./position.txt";
	const char *cstr = position_path.c_str();
	ofstream myfile;
	myfile.open (cstr);
	myfile << "name is_placed x y w h" << endl;
	for(uint i=0; i<op.placements.size(); i++){
		myfile << i << " ";
		myfile << 1 << " ";
		//myfile << op.placements[i].x << " ";
		//myfile << op.placements[i].y << " ";
		//myfile << op.placements[i].w << " ";
		//myfile << op.placements[i].h << endl;
	}
	myfile.close();
}

void hipr::write_position(Dfx op, int place_num){
	string position_path = "./position.txt";
	const char *cstr = position_path.c_str();
	ofstream myfile;
	myfile.open (cstr);
	myfile << "name is_placed x y w h" << endl;
	myfile << place_num << " ";
	myfile << 1 << " ";
	myfile.close();
}


int hipr::return_overlap_tiles(Region shape1, Region shape2){
	Point l1, r1, l2, r2;
	l1.x = shape1.x;
	l1.y = shape1.y;
	r1.x = shape1.x + shape1.w - 1;
	r1.y = shape1.y + shape1.h - 1;
	l2.x = shape2.x;
	l2.y = shape2.y;
	r2.x = shape2.x + shape2.w - 1;
	r2.y = shape2.y + shape2.h - 1;

	int x_dist = min(r1.x, r2.x)
				- max(l1.x, l2.x)+1;
	int y_dist = (min(r1.y, r2.y)
				- max(l1.y, l2.y))+1;
	int areaI = 0;
	if( x_dist >0 && y_dist >0 ){ areaI = x_dist * y_dist; }

	return areaI;
}



Res_range hipr::find_resource_range(Res_type res_type, int start_tile, int end_tile){
	Res_range out;
	for(int i=start_tile; i<end_tile+1; i++){
		if(tiles[i].tile_l == res_type) { out.start = tiles[i].x_l; out.valid = true; break; }
		if(tiles[i].tile_r == res_type) { out.start = tiles[i].x_r; out.valid = true; break; }
	}

	for(int i=start_tile; i<end_tile+1; i++){
		if(tiles[i].tile_l == res_type) { out.end = tiles[i].x_l; }
		if(tiles[i].tile_r == res_type) { out.end = tiles[i].x_r; }
	}
	return out;
}

void hipr::gen_xdc(void){
	const char *cstr = xdc_path.c_str();
	ofstream myfile;
	myfile.open (cstr);

	for(uint i=0; i<dfxs.size(); i++){
		Res_range out;
		//dfxs[i].row_end = dfxs[i].row;
		myfile << "\n\ncreate_pblock " << dfxs[i].name << endl;
		out = find_resource_range(CLB, dfxs[i].placement.x,  dfxs[i].placement.x+ dfxs[i].placement.w-1);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {SLICE_X";
			myfile << out.start << "Y" << CLB_OFFSET+dfxs[i].placement.y*60+5 << ":SLICE_X" << out.end << "Y" << (dfxs[i].placement.y+dfxs[i].placement.h)*60-1 << "}" << endl;
		}

		out = find_resource_range(DSP2, dfxs[i].placement.x, dfxs[i].placement.x+dfxs[i].placement.w-1);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {DSP48E2_X";
			int left_botton_Y = DSP2_OFFSET+dfxs[i].placement.y*24+2 < 0 ? 0 : DSP2_OFFSET+dfxs[i].placement.y*24+2;
			myfile << out.start << "Y" << left_botton_Y << ":DSP48E2_X" << out.end << "Y" << this->DSP2_OFFSET+(dfxs[i].placement.y+dfxs[i].placement.h)*24-1 << "}" << endl;
		}

		out = find_resource_range(BRAM18, dfxs[i].placement.x, dfxs[i].placement.x+dfxs[i].placement.w-1);
		if (out.valid) {
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB18_X";
			int left_botton_Y;
			left_botton_Y = BRAM18_OFFSET+dfxs[i].placement.y*24+2 < 0 ? 0 : BRAM18_OFFSET+dfxs[i].placement.y*24+2;
			myfile << out.start << "Y" << left_botton_Y << ":RAMB18_X" << out.end << "Y" << (dfxs[i].placement.y+dfxs[i].placement.h)*24-1 << "}" << endl;
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB36_X";
			left_botton_Y = BRAM36_OFFSET+dfxs[i].placement.y*12+1 < 0 ? 0 : BRAM36_OFFSET+dfxs[i].placement.y*12+1;
			myfile << out.start << "Y" << left_botton_Y << ":RAMB36_X" << out.end << "Y" << (dfxs[i].placement.y+dfxs[i].placement.h)*12-1 << "}" << endl;
		}

		out = find_resource_range(URAM, dfxs[i].placement.x, dfxs[i].placement.x+dfxs[i].placement.w-1);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {URAM288_X";
			int left_botton_Y = URAM288_OFFSET+dfxs[i].placement.y*16+4 < 0 ? 0 : URAM288_OFFSET+dfxs[i].placement.y*16+4;
			myfile << out.start << "Y" << left_botton_Y << ":URAM288_X" << out.end << "Y" << this->URAM288_OFFSET+(dfxs[i].placement.y+dfxs[i].placement.h)*16-1 << "}" << endl;
		}

		myfile << "set_property SNAPPING_MODE ON [get_pblocks " << dfxs[i].name << "]" << endl;
		myfile << "set_property IS_SOFT TRUE [get_pblocks " << dfxs[i].name << "]" << endl;
		myfile << "add_cells_to_pblock [get_pblocks " << dfxs[i].name;
		myfile << "] [get_cells -quiet [list " << this->xdc_inst << "/ydma_1/mono_inst/" << dfxs[i].name << "_inst]] -quiet" << endl;
	}
	myfile.close();
}




void hipr::find_all_placements(){

	for(uint i=0; i<dfxs.size(); i++){
		dfxs[i].placements.clear();
		Res res_req;
		Region prr;
		res_req = dfxs[i].resource;
		for(uint x=0; x<tiles.size(); x++){
			for(int y=0; y<this->max_row; y++){
				for(int h=1; h+y-1<this->max_row; h++){
					prr.x=x; prr.y=y; prr.h=h;
					if(this->find_tile_range(res_req, prr)){
						// dfxs[i].placements.insert(pair<string,Region>(to_string(prr.x)+"_"+to_string(prr.y), prr));
					}
				}
			}
		}
	}
}


#ifdef BIPAR
bool hipr::bipar_nlp_partition (res ub,
							bin_partition parent_partition,
							bin_partition &p0,
							bin_partition &p1){

	uint node_num = parent_partition.dfxs.size();

	// only push the dfxs in this partition to the dict
	map<string, int> local_dfxs_dict;
	int idx = 1;
	for(dfx p:parent_partition.dfxs){ local_dfxs_dict.insert(pair<string, int>(p.name, idx++)); }

	int nets[node_num][node_num]={0};
	// initilize the values for 2-D arrays
	for(uint i=0; i<node_num; i++){ for(uint j=0; j<node_num; j++){ nets[i][j] = 0; } }
	for(connect p:connects){
		// only consider the interconnections within this partition
		if(local_dfxs_dict[p.src] && local_dfxs_dict[p.dest]){
			nets[local_dfxs_dict[p.src]-1][local_dfxs_dict[p.dest]-1] = p.width;
			nets[local_dfxs_dict[p.dest]-1][local_dfxs_dict[p.src]-1] = p.width;
		}
	}

	try {


		// Create an environment
		GRBEnv env = GRBEnv(true);
		env.set("LogFile", "bipart.log");
		env.start();

		// Create an empty model
		GRBModel model = GRBModel(env);

		// Create variables
		GRBVar m[node_num];
		for(uint i=0; i<node_num; i++){
			ostringstream name;
			name << "m" << i;
			m[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
		}

		//============================================================================================
		// add constraints
		GRBLinExpr lhs, lhs0, lhs1;

		// add luts constraints for partition 0
		lhs0 = 0;
		for(uint i=0; i<node_num; i++){ lhs0 += parent_partition.dfxs[i].lut*(1-m[i]); }
		model.addConstr(lhs0 <= p0.resource.lut);

		// add lusts constraints for partition 1
		lhs1 = 0;
		for(uint i=0; i<node_num; i++){ lhs1 += parent_partition.dfxs[i].lut*m[i]; }
		model.addConstr(lhs1 <= p1.resource.lut);
		// unbalanced ratio constraints
		model.addConstr(lhs1*(0.5-ub.lut) <= lhs0*(0.5+ub.lut));
		model.addConstr(lhs0*(0.5-ub.lut) <= lhs1*(0.5+ub.lut));


		// add bram18 constraints for partition 0
		lhs0 = 0;
		for(uint i=0; i<node_num; i++){ lhs0 += parent_partition.dfxs[i].bram18*(1-m[i]); }
		model.addConstr(lhs0 <= p0.resource.bram18);

		// add constraints for partition 1
		lhs1 = 0;
		for(uint i=0; i<node_num; i++){ lhs1 += parent_partition.dfxs[i].bram18*m[i]; }
		model.addConstr(lhs1 <= p1.resource.bram18);
		// unbalanced ratio constraints
		model.addConstr(lhs1*(0.5-ub.bram18) <= lhs0*(0.5+ub.bram18));
		model.addConstr(lhs0*(0.5-ub.bram18) <= lhs1*(0.5+ub.bram18));

		// add constraints for partition 0
		lhs0 = 0;
		for(uint i=0; i<node_num; i++){ lhs0 += parent_partition.dfxs[i].dsp2*(1-m[i]); }
		model.addConstr(lhs0 <= p0.resource.dsp2);

		// add constraints for partition 1
		lhs1 = 0;
		for(uint i=0; i<node_num; i++){ lhs1 += parent_partition.dfxs[i].dsp2*m[i]; }
		model.addConstr(lhs1 <= p1.resource.dsp2);
		// unbalanced ratio constraints
		model.addConstr(lhs1*(0.5-ub.dsp2) <= lhs0*(0.5+ub.dsp2));
		model.addConstr(lhs0*(0.5-ub.dsp2) <= lhs1*(0.5+ub.dsp2));


		// add constraints for the dfx numbers between 2 partitions
		lhs0 = 0;
		for(uint i=0; i<node_num; i++){ lhs0 += (1-m[i]); }
		lhs1 = 0;
		for(uint i=0; i<node_num; i++){ lhs1 += m[i]; }
		model.addConstr(lhs1-lhs0<=1.0);
		model.addConstr(lhs0-lhs1<=1.0);

		GRBQuadExpr total_nets = 0;

		for(unsigned int i=0; i<node_num; i++){
			for(unsigned int j=0; j<node_num; j++){
				total_nets += nets[i][j]*(m[i]+m[j]-2*m[i]*m[j]);
			}
		}


		model.setObjective(total_nets, GRB_MINIMIZE);

		// Optimize model
		model.set(GRB_DoubleParam_TimeLimit, MAX_TIME);
		model.set("LogToConsole", SHOW_LOG);
		//model.set(GRB_DoubleParam_BestObjStop, 0.10166);
		//model.set(GRB_DoubleParam_MIPGap, 0.65);

		// cout << "begin opt" << endl;
		model.optimize();
		// cout << "finish opt" << endl;

		int optimstatus = model.get(GRB_IntAttr_Status);
		if (optimstatus != GRB_OPTIMAL){
			return false;
		}

		for (uint i=0; i<node_num; i++){
			if(m[i].get(GRB_DoubleAttr_X) == 0){
				p0.dfxs.push_back(parent_partition.dfxs[i]);
			}else{
				p1.dfxs.push_back(parent_partition.dfxs[i]);
			}
		}

		// cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

		model.write("bipar.lp");
		model.write("bipar.mps");
		return true;
	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}
	return false;
}



void hipr::bipar_update_dist_res(dfx &op, tile_range shape){
	Point anchor;
	anchor.x = shape.x + shape.w/2;
	anchor.y = shape.y + shape.h/2;
	for(uint i=0; i<op.placement.size(); i++){
		double c_x = op.placement[i].x +  op.placement[i].w/2;
		double c_y = op.placement[i].y +  op.placement[i].h/2;
		double d_x = c_x>anchor.x ? c_x-anchor.x : anchor.x-c_x;
		double d_y = c_y>anchor.y ? c_y-anchor.y : anchor.y-c_y;
		op.placement[i].dis = d_x + d_y;
		res occupy_res = return_resource(op.placement[i]);
		op.placement[i].r_wastage = occupy_res.lut+occupy_res.bram18+occupy_res.dsp2-op.lut-op.bram18-op.dsp2;
	}
}

void hipr::bipar_recur_partition(bin_partition &parent_partition,
		                         int start,
								 int cut_line,
								 int end,
								 string cut_type,
								 int level){
	// create 2 siblings
	bin_partition partition0, partition1;
	bin_partition new_parent_partition=parent_partition;
	new_parent_partition.dfxs.clear();

	// stop division if no dfxs are assigned to this partition
	if(parent_partition.dfxs.size() == 0){ return; }

	// update the feasible placements if only one dfx is in the partition
	if(parent_partition.dfxs.size()==1){

		dfxs[dfxs_dict[parent_partition.dfxs[0].name]].placement.clear();

		// update the distance to the partition anchor for placements
		this->bipar_update_dist_res(parent_partition.dfxs[0], parent_partition.shape);
		dfxs[dfxs_dict[parent_partition.dfxs[0].name]].placement = parent_partition.dfxs[0].placement;

		// back up the partitions for debugging
		if(cut_type == "V"){ v_slices.push_back(parent_partition); }
		if(cut_type == "H"){ h_slices.push_back(parent_partition); }
		return;
	}

	// if this is the last level of the partition, update the placements
	if(level == 0){
		if(cut_type == "V"){
			v_slices.push_back(parent_partition);
		}else{
			h_slices.push_back(parent_partition);
			for(uint i=0; i<parent_partition.dfxs.size(); i++){
				dfxs[dfxs_dict[parent_partition.dfxs[i].name]].placement.clear();
				this->bipar_update_dist_res(parent_partition.dfxs[i], parent_partition.shape);
				dfxs[dfxs_dict[parent_partition.dfxs[i].name]].placement = parent_partition.dfxs[i].placement;
			}
		}
		return;
	}

	if(cut_type == "H"){ // If the cut is a horizontal line
		partition0.shape.x = parent_partition.shape.x;
		partition0.shape.y = parent_partition.shape.y;
		partition0.shape.w = parent_partition.shape.w;
		partition0.shape.h = cut_line - parent_partition.shape.y + 1;

		partition1.shape.x = parent_partition.shape.x;
		partition1.shape.y = cut_line + 1;
		partition1.shape.w = parent_partition.shape.w;
		partition1.shape.h = parent_partition.shape.h - partition0.shape.h;
	}else{ // If the cut is a vertical line
		partition0.shape.x = parent_partition.shape.x;
		partition0.shape.y = parent_partition.shape.y;
		partition0.shape.w = cut_line - parent_partition.shape.x + 1;
		partition0.shape.h = parent_partition.shape.h;

		partition1.shape.x = cut_line + 1;
		partition1.shape.y = parent_partition.shape.y;
		partition1.shape.w = parent_partition.shape.w - partition0.shape.w;
		partition1.shape.h = parent_partition.shape.h;
	}
	partition0.resource = this->return_resource(partition0.shape);
	partition1.resource = this->return_resource(partition1.shape);

	double area_constraint;
	area_constraint = (start == 0 && end == maxW) ? 1 : 0.9;


	// find the the unplaceable operators
	for(dfx func:parent_partition.dfxs){
		bool isFeasible = false;
		for(tile_range placement:func.placement){
			double area_placement = placement.w * placement.h;
			double area_ratio0 = return_overlap_tiles(placement, partition0.shape) / area_placement;
			if(area_ratio0 >= area_constraint){ isFeasible = true; break; }
		}
		for(tile_range placement:func.placement){
			double area_placement = placement.w * placement.h;
			double area_ratio1 = return_overlap_tiles(placement, partition1.shape) / area_placement;
			if(area_ratio1 >= area_constraint){ isFeasible = true; break; }
		}

		// if could not place the dfx in this partition, remove the it from the subsequent partition
		if(isFeasible == false){
			this->bipar_update_dist_res(func, parent_partition.shape);
			dfxs[dfxs_dict[func.name]].placement = func.placement;
		}else{
			new_parent_partition.dfxs.push_back(func);
		}
	}


	// Try to balance the resource distribution between p0 and p1.
	// When u=0.5, NPL partitioner does not consider the res distribution at all
	bool result = false;
	for(float i=0.1; i<0.6; i=i+0.1){
		res ub={i, i, i};
		bin_partition p0=partition0;
		bin_partition p1=partition1;
		result = this->bipar_nlp_partition (ub, new_parent_partition, p0, p1);
		cout << endl;
		if(result){
			partition0 = p0;
			partition1 = p1;
			break;
		}
	}

	// return if could not partition
	if(!result){
		cout << "Could not partition (" << new_parent_partition.shape.x << ",";
		cout <<  new_parent_partition.shape.y << ",";
		cout << new_parent_partition.shape.w << ",";
		cout << new_parent_partition.shape.h << ")" << endl;
		if(cut_type == "V"){
			v_slices.push_back(parent_partition);
		}else{
			h_slices.push_back(parent_partition);
			for(uint i=0; i<parent_partition.dfxs.size(); i++){
				dfxs[dfxs_dict[parent_partition.dfxs[i].name]].placement.clear();
				this->bipar_update_dist_res(parent_partition.dfxs[i], parent_partition.shape);
				dfxs[dfxs_dict[parent_partition.dfxs[i].name]].placement = parent_partition.dfxs[i].placement;
			}
		}
		return;
	}

	// only reserve the no-overlapping areas for partition0
	for(uint i=0; i<partition0.dfxs.size(); i++){
		vector<tile_range> new_list_placement;
		for(tile_range placement:partition0.dfxs[i].placement){
			double area_placement = placement.w * placement.h;
			double area_ratio0 = return_overlap_tiles(placement, partition0.shape) / area_placement;
			if(area_ratio0 == 1){ new_list_placement.push_back(placement); }
		}
		partition0.dfxs[i].placement = new_list_placement;
	}

	// only reserve the no-overlapping areas for partition1
	for(uint i=0; i<partition1.dfxs.size(); i++){
		vector<tile_range> new_list_placement;
		for(tile_range placement:partition1.dfxs[i].placement){
			double area_placement = placement.w * placement.h;
			double area_ratio1 = return_overlap_tiles(placement, partition1.shape) / area_placement;
			if(area_ratio1 == 1){ new_list_placement.push_back(placement); }
		}
		partition1.dfxs[i].placement = new_list_placement;
	}


	hipr::bipar_recur_partition(partition0, start, (start+cut_line)/2, cut_line, cut_type, level-1);
	hipr::bipar_recur_partition(partition1, cut_line+1, (cut_line+1+end)/2, end, cut_type, level-1);

}


void hipr::bipr_set_dse_idx(vector<vector<int>> &dfx_idxes, int digit[N], uint n){
	if(n==0) {dfx_idxes.clear(); }
	if(n==N) { return; }
	for(uint i=0; i<dfxs[n].placement.size(); i++){
		digit[n] =i;
		if(n==N-1){
			vector<int> tmp;
			for(uint j=0; j<N; j++){
				tmp.push_back(digit[j]);
			}
			dfx_idxes.push_back(tmp);
		}
		this->bipr_set_dse_idx(dfx_idxes, digit, n+1);
	}
}


bool hipr::bipar_find_feasible(vector< vector<int> > dfx_idxes){
	for(uint i=0; i<dfx_idxes.size(); i++){
		int tile_cnt[MAX_ROW][maxW];
		bool overlap = false;
		for(int j=0; j<MAX_ROW; j++){ for(int k=0; k<maxW; k++){ tile_cnt[j][k] = 0; }}
		for(uint j=0; j<N; j++){
			int start = dfxs[j].placement[dfx_idxes[i][j]].x;
			int end = dfxs[j].placement[dfx_idxes[i][j]].x+dfxs[j].placement[dfx_idxes[i][j]].w-1;
			int row = dfxs[j].placement[dfx_idxes[i][j]].y;
			int row_end = dfxs[j].placement[dfx_idxes[i][j]].y+dfxs[j].placement[dfx_idxes[i][j]].h-1;
			for(int k=row; k<=row_end; k++){
				for(int l=start; l<=end; l++){
					tile_cnt[k][l]++;
					if(tile_cnt[k][l] == 2){
						overlap = true;
						break;
					}
				}
				if(overlap) break;
			}
			if(overlap) break;
		}

		if(!overlap){
			for(uint j=0; j<dfxs.size(); j++){
				dfxs[j].start = dfxs[j].placement[dfx_idxes[i][j]].x;
				dfxs[j].row   = dfxs[j].placement[dfx_idxes[i][j]].y;
				dfxs[j].end = dfxs[j].placement[dfx_idxes[i][j]].x + dfxs[j].placement[dfx_idxes[i][j]].w - 1;
				dfxs[j].row_end = dfxs[j].placement[dfx_idxes[i][j]].y + dfxs[j].placement[dfx_idxes[i][j]].h - 1;
			}
			cout << "No. " << i << "/" << dfx_idxes.size() << " solution works!" << endl;
			return true;
			break;
		}
	}

	cout << "No feasible solutions out of " << dfx_idxes.size() << endl;
	return false;
}
void hipr::bipar_reduce_dse(uint space_size){
	for(uint i=0; i<dfxs.size(); i++){
		vector<tile_range> place_dist=dfxs[i].placement;
		vector<tile_range> place_res =dfxs[i].placement;
		uint len_max;
		sort(place_dist.begin(), place_dist.end(), ascendDist);
		sort(place_res.begin(), place_res.end(), ascendRes);

		dfxs[i].placement.clear();
		len_max = place_dist.size() > space_size ? space_size : place_dist.size();
		for(uint j=0; j<len_max; j++){dfxs[i].placement.push_back(place_dist[j]); }
		len_max = place_dist.size() > space_size ? space_size : place_res.size();
		for(uint j=0; j<len_max; j++){dfxs[i].placement.push_back(place_res[j]); }
	}

}

void hipr::bipar_print_slices(vector<bin_partition> slices){
	for(auto p: slices){
		cout << "partition("<< p.shape.x << ",";
		cout << p.shape.y << ",";
		cout << p.shape.w << ",";
		cout << p.shape.h << ",";
		cout << p.resource.lut << ",";
		cout << p.resource.bram18 << ",";
		cout << p.resource.dsp2 << "): ";
		for(auto func:p.dfxs){ cout << func.name << ","; }
		cout << endl;
	}
}

bool hipr::bipar(bool debug){
	bool isFeasible = false;
	// find all the feasible placements for all the dfxs
	this->bipar_find_placements();

	// prepare the parent partitions, which contains all the dfxs
	bin_partition parent_partition;
	parent_partition.dfxs = this->dfxs;
	parent_partition.shape.x = 0;
	parent_partition.shape.y = 0;
	parent_partition.shape.w = maxW;
	parent_partition.shape.h = MAX_ROW;
	parent_partition.resource = this->return_resource(parent_partition.shape);
	cout << "we have " << parent_partition.resource.lut << " luts, ";
	cout << parent_partition.resource.bram18 << " bram18s, ";
	cout << parent_partition.resource.dsp2 << " dsp2s" << endl << endl;



	int level = 2;  // divide the chips vertically into 4 slices
	this->bipar_recur_partition(parent_partition, 0, 30, maxW-1, "V", level);

	for(uint i=0; i<v_slices.size(); i++){
		// level = ceil(log(v_slices[i].dfxs.size())/log(2));
		level = 2; // divide the vertical slices into 4 parts
		this->bipar_recur_partition(v_slices[i], 0, 3, MAX_ROW-1, "H", level);
		cout << endl;
	}

	// reduce the design space exploration
	uint space_size = BIPAR_SPACE_SIZE;
	this->bipar_reduce_dse(space_size);
	int digit[N];
	vector< vector<int> > dfx_idxes;
	this->bipr_set_dse_idx(dfx_idxes, digit, 0);
	cout << "Design Space is: " << dfx_idxes.size() << endl;
	isFeasible=this->bipar_find_feasible(dfx_idxes);

	this->write_position();
	return isFeasible;

}

#endif




bool hipr::SimulatedAnnealing(bool debug){
	double T = t_init;
	int L = 0;
	int L_per_T = 0;
	double at = 0;      // Decreasing ratio (alpha) for temperature
	double accept = 0;
	double cost_min = 100000000;
	double cost_min_backup = cost_min+1;
	double cost_last =  cost_min_backup;
	vector<Dfx> dfxs_backup;
	double cost;
	mTimer stimer;
	int freeze_cnt = 0;
	int L_cnt[(int)log10(t_init/t_min)+1] = {0};
	int iter_max = (int)log10(t_init/t_min)*this->trial_num+this->trial_num*4;
	// Sequence pair for floorplan representation
	vector<int> pos_seq;
	vector<int> neg_seq;
	int func_idx1;
	int func_idx2;
	int swap_case;
	Dfx op_backup;

#ifdef INIT
	// load the initial floorplan
	this->load_init();
#endif


	if(opt_solver == "SQSA"){
		// initialize the sequence pair
		seq_init_seq_pair(pos_seq);
		seq_init_seq_pair(neg_seq);

		// translate sequence pair to floorplan
		this->seq_floorplan(pos_seq, neg_seq);
		this->seq_print_out(pos_seq, "pos_seq");
		this->seq_print_out(neg_seq, "neg_seq");
	}

	this->print_dfxs();
	cost_min = this->cost_function();
	cout << "init_cost = " << cost_min << endl;

	if(opt_solver == "XYSA"){
		for(uint i=0; i<dfxs.size(); i++){ this->place_operator(dfxs[i]); }
	}

	// this->write_position();
	cost_min = this->cost_function();
	if(cost_min_backup > cost_min){
		cost_last =  cost_min_backup;
		cost_min_backup = cost_min;
		dfxs_backup = this->dfxs;
		cout << "init_cost = " << cost_min << endl;
	}

	stimer.start();

	// TODO: accelerate the tile search by finding all the placements first
	// this->find_all_placements();

	// cout << endl;
	// for(auto f:dfxs){
	// 	 map<string, Region>::iterator iter;
	//	for(iter=f.placements.begin(); iter != f.placements.end(); iter++){
	//		cout << iter->first << " : (" << iter->second.x << ",";
	//		cout << iter->second.y << ",";
	//		cout << iter->second.w << ",";
	//		cout << iter->second.h << ")" << endl;
	//	}
	// }


	while((cost_min >= COST_REQ)){
	// while((L--) && (stimer.return_dur() < 600)){
		if(opt_solver == "SQSA"){
			func_idx1 = rand()%dfxs.size();
			func_idx2 = rand()%dfxs.size();
			swap_case = rand()%3;

			func_idx2 = func_idx2!=func_idx1 ? func_idx2 : (func_idx2+1)%dfxs.size();
			this->seq_swap(swap_case, func_idx1, func_idx2, pos_seq, neg_seq);
			this->seq_floorplan(pos_seq, neg_seq);
		}

		if(opt_solver == "XYSA"){
			func_idx1 = rand()%dfxs.size();
			op_backup = dfxs[func_idx1];
			dfxs[func_idx1].placement.x = rand()%tiles.size();
			dfxs[func_idx1].placement.y = rand()%this->max_row;
			this->place_operator(dfxs[func_idx1]);
		}

		// this->print_dfxs();
		cost = this->cost_function();

		double df = cost - cost_min;
		double sj=rand()%10000;     //sj belogs to [0 1]
		sj/=10000;

		cost_last = cost_min_backup;

		if( (df < 0)) { // if the cost is decreased, accept the swap
			cost_min = cost;
			if(cost_min_backup > cost){
				cost_min_backup = cost;
				dfxs_backup = this->dfxs;
			}
		} else if(exp(-df/T)>sj){ // accept the results based on some possibility
			cost_min = cost;
			accept++;
		} else {
			if(opt_solver == "SQSA"){
				this->seq_swap(swap_case, func_idx1, func_idx2, pos_seq, neg_seq);
			}
			if(opt_solver == "XYSA"){
				dfxs[func_idx1] = op_backup;
			}
		}


		//  || cost_min_backup < cost_last
		if(L%(iter_max/100)==0 || L < 1000)
		{
			stimer.stop();
			cout << "Time: " << stimer.return_dur();
			cout << " T: " << T;
			cout << " L = " << L;
			cout << " inter_max = " << iter_max;
			cout << " Already_tried " << L*100.0/iter_max << "%";
			cout << " cost_min = " << cost_min;
			cout << " cout_last = " << cost_min_backup;
			cout << " cost_min_backup = " << cost_min_backup << endl;
			stimer.stop();
			out_log_file << "Time: " << stimer.return_dur();
			out_log_file << " T: " << T;
			out_log_file << " Already_tried " << L*100.0/iter_max << "%";
			out_log_file << " cost_min = " << cost_min;
			out_log_file << " cout_last = " << cost_min_backup;
			out_log_file << " cost_min_backup = " << cost_min_backup << endl;
		}else if(cost_min_backup < cost_last){
			stimer.stop();
			cout << "ExTime: " << stimer.return_dur();
			cout << " T: " << T;
			cout << " L = " << L;
			cout << " inter_max = " << iter_max;
			cout << " Already_tried " << L*100.0/iter_max << "%";
			cout << " cost_min = " << cost_min;
			cout << " cout_last = " << cost_min_backup;
			cout << " cost_min_backup = " << cost_min_backup << endl;
			stimer.stop();
			out_log_file << "ExTime: " << stimer.return_dur();
			out_log_file << " T: " << T;
			out_log_file << " Already_tried " << L*100.0/iter_max << "%";
			out_log_file << " cost_min = " << cost_min;
			out_log_file << " cout_last = " << cost_min_backup;
			out_log_file << " cost_min_backup = " << cost_min_backup << endl;
		}
		L++;

		L_per_T++;

		if(T>this->t_min){
			if(L_per_T == this->trial_cnt){
				at = AT_INIT;
				//if(T>1)        { at = 0.9999;   }
				//else if(T>0.1) { at = 0.99999;  }
				//else if(T>0.01){ at = 0.9999999; }
				//else           { at = 0.99999999;}
				T*=at;  // temperature decrease
				L_per_T = 0;
			}
		}else{
			if(freeze_cnt++ == 4*trial_num){
				break; //Exit when temperature is small enough
			}
		}


		for(int i=0; i<(int)log10(t_init/t_min)+1; i++){
			if(T < t_init/pow(10, i) && T > t_init/pow(10, i+1)){
				L_cnt[i]++;
			}
		}

	}

	cout << "(int)log10(t_init/t_min)=" << (int)log10(t_init/t_min)+1 << endl;
	for(int i=0; i<(int)log10(t_init/t_min)+1; i++){
		cout << setw(10) <<  L_cnt[i] << ": T = " << t_init/pow(10, i) << "-" << t_init/pow(10, i+1) << endl;
	}

	this->dfxs = dfxs_backup;
	cost_min = this->cost_function();

	stimer.stop();
	cout << "Time: " << stimer.return_dur();
	cout << " T: " << T;
	cout << " Already_tried " << L*100.0/iter_max << "%";
	cout << " cost_min = " << cost_min << endl;

	cout << "SA took " << stimer.return_dur() << " sec." << endl;
	cout << "cost_min = " << cost_min << endl;
	cout << "cost_min_backup = " << cost_min_backup << endl;
	cout << "accept ratio = " << accept/this->trial_num << endl;
	cout << "e_termination = " << this->t_min << endl;
	cout.precision(13);
	cout << "at = " << at << endl;
	cout << "T_init = " << this->t_init << endl;
	cout << "L_max " << this->trial_num << endl;
	cout << "Finished with " << L << " trials!" << endl;

	out_log_file << "SA took " << stimer.return_dur() << " sec." << endl;
	out_log_file << "cost_min = " << cost_min << endl;
	out_log_file << "cost_min_backup = " << cost_min_backup << endl;
	out_log_file << "accept ratio = " << accept/this->trial_num << endl;
	out_log_file << "e_termination = " << this->t_min << endl;
	out_log_file.precision(13);
	out_log_file << "at = " << at << endl;
	out_log_file << "T_init = " << this->t_init << endl;
	out_log_file << "L_max " << this->trial_num << endl;
	out_log_file << "Finished with " << L << " trials!" << endl;

	this->gen_xdc();
	this->write_position();
	this->report_utilization();


	this->print_dfxs();


	cout << "trial_cnt=" << trial_cnt << endl;
	cout << tiles.size() << endl;
	return cost_min < 1 ? true : false;
}


