#include "../host/typedefs.h"

void read_in(hls::stream<ap_uint<512> >& Input_1,hls::stream<ap_uint<128> >& Output_1){
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1

  bit512 in_tmp;
  bit128 out_tmp;
  for (bit16 i = 0; i < NUM_3D_TRI/4; i++){
    in_tmp = Input_1.read();
    for(int j=0; j<4; j++){
      #pragma HLS PIPELINE II=1
      out_tmp = in_tmp(128*j+127, 128*j);
      Output_1.write(out_tmp);
    }
  }
}
      