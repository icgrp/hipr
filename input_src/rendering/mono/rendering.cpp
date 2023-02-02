
#include "../host/typedefs.h"

  #include "../mono/read_in.h"
  #include "../mono/projection.h"
  #include "../mono/rasterization1.h"
  #include "../mono/rasterization2.h"
  #include "../mono/zculling.h"
  #include "../mono/coloringFB.h"
  
  
  const int DEFAULT_DEPTH = 128;
  
 void rendering(hls::stream<ap_axiu<512, 0, 0, 0> >& Input_1, hls::stream<ap_axiu<512, 0, 0, 0> >& Output_1){
  const int default_depth = 1024;
  #pragma HLS DATAFLOW

  hls::stream<ap_uint<128> > k1_in;
  #pragma HLS STREAM variable=k1_in      depth=default_depth
  hls::stream<ap_uint<64> > prj_out;
  #pragma HLS STREAM variable=prj_out    depth=default_depth
  hls::stream<ap_uint<32> > rast1_out;
  #pragma HLS STREAM variable=rast1_out  depth=default_depth
  hls::stream<ap_uint<32> > rast2_out;
  #pragma HLS STREAM variable=rast2_out  depth=default_depth
  hls::stream<ap_uint<32> > zcull_out;
  #pragma HLS STREAM variable=zcull_out  depth=default_depth



    read_in(Input_1, k1_in);
    projection(k1_in, prj_out);
    rasterization1(prj_out, rast1_out);
    rasterization2(rast1_out, rast2_out);
    zculling(rast2_out, zcull_out);
    coloringFB(zcull_out, Output_1);
  
}



