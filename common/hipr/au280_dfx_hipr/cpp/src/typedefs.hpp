#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifdef MILP
  #include <gurobi_c++.h>
#endif

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <list>
#include <stack>
#include <map>
#include <math.h>
#include <limits>
#include <bits/stdc++.h>
typedef std::numeric_limits< double > dbl;

using namespace std;


  #define T_INIT           100                    // Initial temperature
  #define MY_SEED1         139                    // Random seed
  #define TRIAL_NUM1       100000                 // Default trial number per log-scale unit
  #define T_MIN1           1e-10                  // Cooling termination criteria
  #define PATH_PREFIX1     "./src/app/"           // default application dir
  #define APP_NAME1        "digit_reg_dyn"        // default application name
  #define DEVICE1          "au50"                 // default device name <au50, au51, zcu102, zcu103, ...>
  #define OUT_FILE1        "../xdc/sub.xdc"       // default xdc file path
  #define OUT_LOG_FILE1    "./out.log"            // default log file path
  #define INST_NAME1       "level0_i/ulp"         // default instance prefix for xdc generation. <"level0_i/ulp", "pfm_top_i/dynamic_region">
  #define OPT_SOLVER       "XYSA"                 // default optimizer <XYSA, SQSA, MILP>
  #define AT_INIT          0.9997                 // 999.97 for 10X sweep
  #define NINF             INT_MIN                // Initial temperature for Simulated Annealing
  #define SUCCESS          0                      // Optimizer can find a legal floorplan
  #define FAILURE          1                      // Optimizer cannot find a legal floorplan
  #define COST_REQ         0                      // The cost target. Set it to 0 to be the most aggressive trial
  #define frameW           1                      // width of a frame
  #define frameH           20                     // height of a frame
  #define MAX_DATAWIDTH    512                    // Maximum Data width used in the design. Usually, it is the datawdith between HOST and FABRIC
  #define WH_RATIO         60                     // Maximum W/H ratio allowed in Simulated Annealing search
  #define HW_RATIO         3                      // Maximum H/W ratio allowed in Simulated Annealing search
  #define RETRY_NUM        1000                   // If XYSA fails with the given (x,y), randomly regenerate new (x,y) RETRY_NUM times and try
  #define SHOW_LOG         "0"                    // 1: show all the log on the screen
  #define BIPAR_SPACE_SIZE 3                      //
  #define CLB_PER_TILE     480                    // The number of LUTs per tile
  #define FF_PER_TILE      960                    // The number of FFs per tile
  #define BRAM18_PER_TILE  24                     // The number of BRAM18s per tile
  #define DSP2_PER_TILE    24                     // The number of DSPs per tile
  #define DMA_DCX1         5                      // DMA source (Host to Fabric) x coordinate
  #define DMA_DCY1         0                      // DMA source (Host to Fabric) y coordinate
  #define DMA_DCX2         5                      // DMA destination (Fabric to Host) x coordinate
  #define DMA_DCY2         0                      // DMA destination (Fabric to Host) y coordinate
  #define MAX_TIME         2000                   // Maximum runtime for MILP
  #define POSITION_FILE    "position.txt"         // Default position file to python to show the flooplan
  #define q_WLcost         1                      // Weight for total wire length for cost function
  #define q_Pcost          0                      // Weight for total peripheral length for cost function
  #define q_Rcost          0                      // Weight for total resource wastage for cost function
  #define SAFE_GAP         3                      // The minimum columns gap between 2 PR Regions to place Relay_Station/Skid_buffer
  #define MIN_WIDTH        4                      // Minimum width for a page to facilitate routing
  #define MIPGAP           0                      // Set the Mixed Integer Programming Gap number for early termination
  #define MILP_MAX_TIME    86000                  // Set the Mixed Integer Programming maximum runtime. value 0 dienables this set
  #define PSUEDO1                                 // if PSUEDO, MILP will have more constraints. Used for debugging
  #define INIT1                                   // if INIT, all optimizers will have initial values
  #define STRICT_CLB



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
