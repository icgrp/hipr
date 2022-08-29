#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
//#include "gurobi_c++.h"
#include <limits.h>
#include <list>
#include <stack>
#include <map>
#include <math.h>
#include <limits>
#include <bits/stdc++.h>
typedef std::numeric_limits< double > dbl;

using namespace std;
#define OPT_SOLVER "XYSA"
//#define OPT_SOLVER "SQSA"


#define TRIAL_NUM1 10000
#define PSUEDO
#define INIT1
#define STRICT_CLB


#define NINF INT_MIN

// #define T_min   1e-16      // Cooling termination criteria
#define T_INIT1  100        // Initial temperature
#define AT_INIT 0.9997     // 999.97 for 10X sweep
//#define AT_INIT 0.9977     // 999.97 for 10X sweep
#define T_MIN1   1e-3      // Cooling termination criteria

#define MY_SEED1 139        // Random seed

#define  DEVICE1 "au50"
//#define  DEVICE "zcu102"
//#define  DEVICE1 "zcu102"


#define SUCCESS   0
#define FAILURE   1



#define q_WLcost 1
#define q_Pcost 0
#define q_Rcost 0
#define COST_REQ   0




#define frameW 1	// width of a frame
#define frameH 12	// height of a frame

#define MAX_DATAWIDTH 512
#define WH_RATIO 60
#define HW_RATIO 3
#define RETRY_NUM 1000

#define SHOW_LOG "0"



#define BIPAR_SPACE_SIZE 3


#define PATH_PREFIX1 "./src/app/synthetic"
// #define PATH_PREFIX "./src/app/rendering512"
// #define PATH_PREFIX "./src/app/spam_filter512"
// #define PATH_PREFIX "./src/app/face_detection512"
// #define PATH_PREFIX "./src/app/bnn512"
// #define PATH_PREFIX "./src/app/digit_reg512"
// #define PATH_PREFIX "./src/app/optical_flow512"

// #define PATH_PREFIX "./src/app/au50_5_90"
// #define PATH_PREFIX "./src/app/au50_5_85"
// #define PATH_PREFIX "./src/app/au50_5_80"
// #define PATH_PREFIX "./src/app/au50_5_75"
// #define PATH_PREFIX "./src/app/au50_5_70"

//#define  DEVICE "zcu102"
//#define INST_NAME1 "pfm_top_i/dynamic_region"
#define INST_NAME1 "level0_i/ulp"
//#define OUT_FILE "/home/ylxiao/ws_211_hipr/hipr_fpl2022/workspace/F001_overlay_rendering512/ydma/zcu102/zcu102_dfx_hipr/xdc/sub.xdc"
#define OUT_FILE1 "../xdc/sub.xdc"
#define OUT_LOG_FILE1 "./out.log"


// #define CLB_PER_TILE 1
// #define BRAM18_PER_TILE 1
// #define DSP2_PER_TILE 1
// #define CLB_PER_TILE 432
#define CLB_PER_TILE    480
#define FF_PER_TILE     960
#define BRAM18_PER_TILE 24
#define DSP2_PER_TILE   24

#define DMA_DCX1 5
#define DMA_DCY1 3.5
#define DMA_DCX2 5
#define DMA_DCY2 3.5



#define MAX_TIME 2000

#define W_VERTICAL   1000
#define W_HORIZONTAL 1
#define W_DEST       0.000002
//#define COST_REQ      0.06



typedef unsigned int uint;

enum Res_type {CLB, BRAM18, DSP2, URAM, IOB, FORB, PCIE}; // specify the type of resources

typedef struct{ // Represent a point
	int x, y;
}Point;

typedef struct{ // Represent a physical region
    int x;               // The left bottom x axis
	int y;               // The left bottom y axis
    int h;               // The height of the region
    int w;               // The width of the region
    double dis=0;        // The distance to an anchor point
    double r_wastage=0;  // The resource wastage
}Region;

typedef struct{ // Define the link between different operators
	string src;   // The source operator's name
	string dest;  // The destination operator's name
	double width; // The wire width
	int i;
}Connect;

typedef struct{ // Resource representation for FPGAs
	double lut=0, ff=0, bram18=0, dsp2=0;
} Res;

typedef struct{ // Pragma reprensation
    string name;
    Res resource;
    int i;  //
} Pragma;

typedef struct{ // Tile description for FPGAs
	Res_type tile_l;   // The left half of a tile type
	Res_type tile_r;   // The right half of a tile type
    double x_l;      // The number for different resource type
    double x_r;      // The number for different resource type
    int i;  //
} Tile;

typedef struct { // The dynamic function exchange description
    string name;
    string isPR;
    Res resource;
    Region placement;
    map<string, Region> placements;
    int i;  //
}Dfx;

typedef struct { // Find the left-most and right-most tiles of specific resource type
	bool valid=false;
    int start, end;
}Res_range;








typedef struct{ // for MILP partition generation
	Res_type resource_name;
	bool valid=true;
	bool used=false;
} Grid;

typedef struct{ // rectangle
	Res_type name;
	int r1,r2,c1,c2;
} Partition;

typedef struct{ // resource requirements
         int n1, n2;
         double b;
}Link;

typedef struct{ // comparative position
	vector<int> pos_before, pos_after,neg_before, neg_after;
	vector<int> right, left, above, below;
} position_seq;

typedef struct{ // comparative position
	int src, dest;
	double weight;
} weight_edge;

typedef struct{ // vertice for bipartition
	string name;
	Res resource;
} vertice;


typedef struct{ // psuedo_type
	Region shape;
	Res resource;
	vector<Dfx> dfxs;
} bin_partition;




#endif
