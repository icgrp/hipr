#include "typedefs.hpp"
#include "hipr.hpp"

double e=1e-16,at=0.99999999,T=1;
int L = TRIAL_NUM;


hipr::hipr(string path_prefix, string device_name, string inst_name){
	pragma_path       = path_prefix + "/pragma.txt";
	dfx_path          = path_prefix + "/dfx.txt";
	connect_path      = path_prefix + "/connect.txt";
	tile_path         = "./src/arch/"+device_name+".txt";
	invalid_tile_path = "./src/arch/"+device_name+"_invalid.txt";
	xdc_path          = path_prefix + "/../../../xdc/sub.xdc";
	// xdc_path          = "/home/ylxiao/quark/Downloads/sub.xdc";
    xdc_inst          = inst_name;
	this->init_pragma();
	this->init_connect();
	this->init_tile();
	this->init_dfx();
	this->init_invalid();
}


void hipr::init_pragma(void){
	const char *cstr = pragma_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	int i=0;
	pragma t;
	while(myfile >> t.name >> t.lut >> t.ff >> t.bram18 >> t.dsp2){
		t.i=i++;
		pragmas.push_back(t);
	}
	myfile.close();
}

void hipr::init_connect(void){
	const char *cstr = connect_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	connect t;
	int i=0;
	while(myfile >> t.src >> t.dest){
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
	tile t;
	while(myfile >> t.tile_l >> t.x_l >> t.tile_r >> t.x_r){
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
	dfx t;
	while(myfile >> t.name >> t.type >> t.lut >> t.ff >> t.bram18 >> t.dsp2){
		t.lut    *= pragmas[i].lut;
		t.ff     *= pragmas[i].ff;
		t.bram18 *= pragmas[i].bram18;
		t.dsp2   *= pragmas[i].dsp2;
		t.i=i++;
		t.row = rand()%MAX_ROW;
		t.start = rand()%tiles.size();
		t.end = t.start;
		this->find_tile_range(t);
		dfxs.push_back(t);
	}
	myfile.close();
}

void hipr::init_invalid(void){
	const char *cstr = invalid_tile_path.c_str();
	ifstream myfile;
	myfile.open(cstr);
	invalid_tile t;

	// reand in the CLB BRAM36 BRAM18 DSP2 Y-offset for xdc generation
	myfile >> t.name >> CLB_OFFSET >> BRAM36_OFFSET >> BRAM18_OFFSET >> DSP2_OFFSET;
	if(t.name != "OFFSET_CLB_BRAM36_BRAM18_DSP2"){
		cout << "The first row of " << invalid_tile_path << " should be \"OFFSET_CLB_BRAM36_BRAM18_DSP2\"!" << endl;
		exit(FAILURE);
	}
	// reand in the invalid tiles
	while(myfile >> t.name >> t.col_start >> t.col_end >> t.row_start >> t.row_end){
		if(t.name == "INVALID_TILE"){
			invalid_tiles.push_back(t);
		}else{
			cout << "The format from the second row of " << invalid_tile_path << " should be \"INVALID_TILE\"!" << endl;
			exit(FAILURE);
		}
	}
	myfile.close();
}

res_range hipr::find_resource_range(string res_type, int start_tile, int end_tile){
	res_range out;
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
		res_range out;
		myfile << "\n\ncreate_pblock " << dfxs[i].name << endl;
		out = find_resource_range("CLB", dfxs[i].start, dfxs[i].end);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {SLICE_X";
			myfile << out.start << "Y" << CLB_OFFSET+dfxs[i].row*60 << ":SLICE_X" << out.end << "Y" << (this->dfxs[i].row+1)*60-1 << "}" << endl;
		}

		out = find_resource_range("EMPTY_CLB", dfxs[i].start, dfxs[i].end);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {SLICE_X";
			myfile << out.start << "Y" << CLB_OFFSET+dfxs[i].row*60 << ":SLICE_X" << out.end << "Y" << (this->CLB_OFFSET+dfxs[i].row+1)*60-1 << "}" << endl;
		}

		out = find_resource_range("DSP2", dfxs[i].start, dfxs[i].end);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {DSP48E2_X";
			myfile << out.start << "Y" << DSP2_OFFSET+dfxs[i].row*24 << ":DSP48E2_X" << out.end << "Y" << (this->dfxs[i].row+1)*24-1 << "}" << endl;
		}

		out = find_resource_range("EMPTY_DSP2", dfxs[i].start, dfxs[i].end);
		if (out.valid){
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {DSP48E2_X";
			myfile << out.start << "Y" << DSP2_OFFSET+dfxs[i].row*24 << ":DSP48E2_X" << out.end << "Y" << (this->DSP2_OFFSET+dfxs[i].row+1)*24-1 << "}" << endl;
		}


		out = find_resource_range("BRAM18", dfxs[i].start, dfxs[i].end);
		if (out.valid) {
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB18_X";
			myfile << out.start << "Y" << BRAM18_OFFSET+dfxs[i].row*24 << ":RAMB18_X" << out.end << "Y" << (this->dfxs[i].row+1)*24-1 << "}" << endl;
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB36_X";
			myfile << out.start << "Y" << BRAM36_OFFSET+dfxs[i].row*12 << ":RAMB36_X" << out.end << "Y" << (this->dfxs[i].row+1)*12-1 << "}" << endl;
		}

		out = find_resource_range("EMPTY_BRAM18", dfxs[i].start, dfxs[i].end);
		if (out.valid) {
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB18_X";
			myfile << out.start << "Y" << BRAM18_OFFSET+dfxs[i].row*24 << ":RAMB18_X" << out.end << "Y" << (this->dfxs[i].row+1)*24-1 << "}" << endl;
			myfile << "resize_pblock [get_pblocks " << dfxs[i].name << "] -add {RAMB36_X";
			myfile << out.start << "Y" << BRAM36_OFFSET+dfxs[i].row*12 << ":RAMB36_X" << out.end << "Y" << (this->dfxs[i].row+1)*12-1 << "}" << endl;
		}
		myfile << "set_property SNAPPING_MODE ON [get_pblocks " << dfxs[i].name << "]" << endl;
		myfile << "set_property IS_SOFT TRUE [get_pblocks " << dfxs[i].name << "]" << endl;
		myfile << "add_cells_to_pblock [get_pblocks " << dfxs[i].name;
		myfile << "] [get_cells -quiet [list " << this->xdc_inst << "/ydma_1/mono_inst/" << dfxs[i].name << "_inst]]" << endl;
	}
	myfile.close();
}



void hipr::print_pragma(void){
	cout << "\n\n============ pragma" << endl;
	for(uint i=0; i<pragmas.size(); i++){
		cout << pragmas[i].i << ": " ;
		cout << std::left << std::setw (20) << pragmas[i].name;
		cout << std::setw (4) << pragmas[i].lut;
		cout << std::setw (4) << pragmas[i].ff;
		cout << std::setw (4) << pragmas[i].bram18;
		cout << std::setw (4) << pragmas[i].dsp2 << endl;
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

void hipr::calc_resource(dfx dfx_in, int resource_num[3]){
	int clbs_num = 0, bram18s_num = 0, dsps_num = 0;

	for (int col = dfx_in.start; col <= dfx_in.end; col++){
		if (tiles[col].tile_l == "CLB"){ clbs_num += num_per_tile.lut; }
		if (tiles[col].tile_r == "CLB"){ clbs_num += num_per_tile.lut; }
		if (tiles[col].tile_l == "BRAM18"){ bram18s_num += num_per_tile.bram18; }
		if (tiles[col].tile_r == "BRAM18"){ bram18s_num += num_per_tile.bram18; }
		if (tiles[col].tile_l == "DSP2"){ dsps_num += num_per_tile.dsp2; }
		if (tiles[col].tile_r == "DSP2"){ dsps_num += num_per_tile.dsp2; }
	}
	resource_num[0] = clbs_num;
	resource_num[1] = bram18s_num;
	resource_num[2] = dsps_num;
}

void hipr::print_dfx(void){
	cout << "\n\n============ dfx" << endl;
	cout << "# : operator          Type   LUTs   FFs    BRAM18 DSP    row start end " << endl;
	int resource_num[3];
	for(uint i=0; i<dfxs.size(); i++){
		this->hipr::calc_resource(dfxs[i], resource_num);

		cout << std::setw (2) << dfxs[i].i;
		cout << ": " << std::left << std::setw (18);
		cout << dfxs[i].name;
		cout << std::setw (7) << dfxs[i].type;
		cout << std::setw (7) << dfxs[i].lut;
		cout << std::setw (7) << dfxs[i].ff;
		cout << std::setw (7) << dfxs[i].bram18;
		cout << std::setw (7) << dfxs[i].dsp2;
		cout << std::setw (4) << dfxs[i].row;
		cout << std::setw (6) << dfxs[i].start;
		cout << std::setw (4) << dfxs[i].end << endl;
		cout << std::setw (29) << " ";
		cout << std::setw (7) << resource_num[0];
		cout << std::setw (7) << " ";
		cout << std::setw (7) << resource_num[1];
		cout << std::setw (7) << resource_num[2] << endl;
	}
	cout << "============ end of dfx\n\n" << endl;
}

void hipr::print_utilization(void){
	cout << "\n\n============ utilization" << endl;
	int resource_num[3];
	int app_num_total[3] = {0, 0, 0};
	int resource_num_total[3] = {0, 0, 0};
	cout << "# : operator          LUTs   BRAM18 DSP" << endl;
	for(uint i=0; i<dfxs.size(); i++){
		this->hipr::calc_resource(dfxs[i], resource_num);
		cout << std::setw (2) << dfxs[i].i;
		cout << ": " << std::left << std::setw (18);
		cout << dfxs[i].name;
		cout << std::setprecision(2);
		cout << std::setw (7) << dfxs[i].lut / resource_num[0];
		cout << std::setw (7) << dfxs[i].bram18 / resource_num[1];
		cout << std::setw (7) << dfxs[i].dsp2 / resource_num[2] << endl;
		app_num_total[0] += dfxs[i].lut;
		app_num_total[1] += dfxs[i].bram18;
		app_num_total[2] += dfxs[i].dsp2;
		resource_num_total[0] += resource_num[0];
		resource_num_total[1] += resource_num[1];
		resource_num_total[2] += resource_num[2];
	}

	cout << std::setw (2) << "--";
	cout << ": " << std::left << std::setw (18);
	cout << "APP/Chip";
	cout << std::setprecision(2);
	cout << std::setw (7) << app_num_total[0] / 242864.0;
	cout << std::setw (7) << app_num_total[1] / 1680.0;
	cout << std::setw (7) << app_num_total[2] / 2160.0 << endl;

	cout << std::setw (2) << "--";
	cout << ": " << std::left << std::setw (18);
	cout << "PR/Chip";
	cout << std::setprecision(2);
	cout << std::setw (7) << resource_num_total[0] / 242864.0;
	cout << std::setw (7) << resource_num_total[1] / 1680.0;
	cout << std::setw (7) << resource_num_total[2] / 2160.0 << endl;

	cout << "============ end of utilization\n\n" << endl;
}


void hipr::print_invalid(void){
	cout << "\n\n============ invalid tiles" << endl;
	cout << "Resource  col_start  col_end  row_start  row_end " << endl;
	for(uint i=0; i<invalid_tiles.size(); i++){
		cout << std::left << std::setw (10);
		cout << invalid_tiles[i].name;
		cout << std::setw (11) << invalid_tiles[i].col_start;
		cout << std::setw (9) << invalid_tiles[i].col_end;
		cout << std::setw (11) << invalid_tiles[i].row_start;
		cout << std::setw (9) << invalid_tiles[i].row_end << endl;
	}


	cout << "============ invalid tiles\n\n" << endl;
}

void hipr::print_seq(void){
	for(uint i=0; i<dfxs.size(); i++){ cout << dfxs[i].i << " "; }
	cout << endl;
}

void hipr::floorplan(void){
	for(uint i=0; i<dfxs.size(); i++){
		this->find_tile_range(dfxs[i]);
	}
}

void hipr::find_tile_range(dfx & op, bool debug){
	dfx op_still_need;
	if(debug) cout << "\n======================" << op.name << endl;
	op_still_need = op;
	op_still_need.end = op_still_need.start;

	while(op_still_need.lut > 0 || op_still_need.bram18>0 || op_still_need.dsp2>0){
		if(debug) cout << op_still_need.end << endl;
		if(tiles[op_still_need.end].tile_l == "CLB")    { op_still_need.lut    -= num_per_tile.lut; }
		if(tiles[op_still_need.end].tile_l == "BRAM18") { op_still_need.bram18 -= num_per_tile.bram18; }
		if(tiles[op_still_need.end].tile_l == "DSP2")   { op_still_need.dsp2   -= num_per_tile.dsp2; }
		if(tiles[op_still_need.end].tile_r == "CLB")    { op_still_need.lut    -= num_per_tile.lut; }
		if(tiles[op_still_need.end].tile_r == "BRAM18") { op_still_need.bram18 -= num_per_tile.bram18; }
		if(tiles[op_still_need.end].tile_r == "DSP2")   { op_still_need.dsp2   -= num_per_tile.dsp2; }
		if(debug) cout << "clb=" << op_still_need.lut;
		if(debug) cout << "bram2=" << op_still_need.bram18;
		if(debug) cout << "dsp2=" << op_still_need.dsp2 << endl;
		if(debug) cout << "start_tile=" << op_still_need.start << " end_tile=" << op_still_need.end << endl;
		op_still_need.end++;
		// if resources are all used, go to the upper row
		if(op_still_need.end > ((int) tiles.size()-1)){
			op_still_need = op; // reset the resources needed
			op_still_need.row++;
			if(op_still_need.row == MAX_ROW){ op_still_need.row = 0; }
			op_still_need.start = 0;
			op_still_need.end = 0;
		}

	}

	op.start = op_still_need.start;
	op.end   = op_still_need.end;
	op.row   = op_still_need.row;
}

double hipr::return_total_dest(void){
	double x1, y1;
	double x2, y2;
	double out = 0;
	for(uint i=0; i<connects.size(); i++){
		for(uint j=0; j<dfxs.size(); j++){
			if(dfxs[j].name == connects[i].src || "DMA" == connects[i].src){
				x1 = dfxs[j].start;
				y1 = dfxs[j].row;

			}

			if(dfxs[j].name == connects[i].dest){
				x2 = dfxs[j].start;
				y2 = dfxs[j].row;
			}
			if("DMA" == connects[i].src){
				x1 = 0;
				y1 = 0;
				out += 3*(W_HORIZONTAL*((x1-x2)*(x1-x2))+W_VERTICAL*((y1-y2)*(y1-y2)));
			}else if("DMA" == connects[i].dest){
				x2 = 0;
				y2 = 0;
				out += 3*(W_HORIZONTAL*((x1-x2)*(x1-x2))+W_VERTICAL*((y1-y2)*(y1-y2)));
			}else{
				out += W_HORIZONTAL*((x1-x2)*(x1-x2))+W_VERTICAL*((y1-y2)*(y1-y2));
			}
		}

	}

	out = out/connects.size();
	return out;
}



double hipr::cost_function(bool debug){
	double cost = 0;
	vector <double> cost_table[7];

	// initialize the cost table to -1 as it can be used once.
	for (int row=0; row<7; row++){
		for (uint col=0; col<tiles.size(); col++){
			cost_table[row].push_back(-1);
		}
	}

	// increase the cost table for that file if it is used by some p-block
	for(uint i=0; i<dfxs.size(); i++){
		for(int j=dfxs[i].start; j<dfxs[i].end+2; j++){
			if (j != (int) tiles.size()) cost_table[dfxs[i].row][j]++;
		}
	}


	// test for overlapping
	int overlap = 0;
	for(int row=0; row<7; row++){
		for(uint col=0; col<tiles.size(); col++){
			//cout << cost_table[row][col] << ",";
			if(cost_table[row][col] > 0){
				overlap = overlap + cost_table[row][col];
			}
		}
		//cout << endl;
	}

	if(debug) cout << "overlap_area=" << overlap << endl;

	// test for unusable areas
	int invalid_area = 0;

	for(uint i=0; i<dfxs.size(); i++){
		for(unsigned int invld_idx = 0; invld_idx < invalid_tiles.size(); invld_idx++){
			if(dfxs[i].row <= invalid_tiles[invld_idx].row_end &&
			   dfxs[i].row >= invalid_tiles[invld_idx].row_start){
			   if((dfxs[i].start >= invalid_tiles[invld_idx].col_start &&
				  dfxs[i].start <= invalid_tiles[invld_idx].col_end) ||
				 (dfxs[i].end >= invalid_tiles[invld_idx].col_start &&
				  dfxs[i].end <= invalid_tiles[invld_idx].col_end) ||
				 (dfxs[i].end >= invalid_tiles[invld_idx].col_end &&
				  dfxs[i].start <= invalid_tiles[invld_idx].col_start) ){
				   invalid_area++;
			   }
			}
		}

	}


	cout << "invalid_area=" << invalid_area << endl;



	cost = overlap+invalid_area + this->return_total_dest()*W_DEST;
	if(debug) cout << "overlay = " << overlap << " invalid_area=" << invalid_area << endl;
	return cost;
}


void hipr::SimulatedAnnealing(bool debug){
	double accept = 0;
	double cost_min = 100000000;
	double cost;
	this->floorplan();
	cost_min = this->cost_function();
	if(debug) cout << "init_cost = " << this->cost_function() << endl;

	while(L--){
		if(L%1000==0) cout << "L = " << L << endl;
		int func_index1 = rand()%dfxs.size();

		dfx op_backup = dfxs[func_index1];

		dfxs[func_index1].row   = rand()%MAX_ROW;
		dfxs[func_index1].start = rand()%tiles.size();
		this->find_tile_range(dfxs[func_index1]);

		if(debug) cout << "L = " << L << ": ";
		this->floorplan();
		cost = this->cost_function();

		double df = cost - cost_min;
		double sj=rand()%10000;     //sj belogs to [0 1]
		sj/=10000;


		if( (df < 0)) { // if the cost is decreased, accept the swap
			cost_min = cost;
		} else if(exp(-df/T)>sj){ // accept the results based on some possibility
			cost_min = cost;
			accept++;
		} else {
			dfxs[func_index1] = op_backup;
		}
		if(debug) cout << "cost = " << cost;
		cout << " cost_min = " << cost_min << endl;

		if(debug) this->print_seq();
		if(cost_min < COST_REQ) break;
		T*=at;  // temperature decrease
		if(T<e) break;  //Exit when temperature is small enough
	}


	cout << " cost_min = " << cost_min << endl;
	cout << "accept ratio = " << accept/TRIAL_NUM << endl;
}







