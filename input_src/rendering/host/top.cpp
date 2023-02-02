/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
#include <iostream>
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "typedefs.h"

#ifdef MONO
  #include "../mono/rendering.h"

#else

  #include "../operators/read_in.h"
  #include "../operators/projection.h"
  #include "../operators/rasterization1.h"
  #include "../operators/rasterization2.h"
  #include "../operators/zculling.h"
  #include "../operators/coloringFB.h"
  
  
  const int DEFAULT_DEPTH = 128;
  
  
  void top (
  		  hls::stream<ap_uint<512> > & Input_1,
  		  hls::stream<ap_uint<512> > & Output_1
  		)
  
  //( bit32 input[3*NUM_3D_TRI], bit32 output[NUM_FB])
  {
  #pragma HLS INTERFACE ap_hs port=Input_1
  #pragma HLS INTERFACE ap_hs port=Output_1
  #pragma HLS DATAFLOW

  const int default_depth = 1024;
  hls::stream<ap_uint<128> > k1_in;
  hls::stream<ap_uint<64> > prj_out;
  hls::stream<ap_uint<32> > rast1_out;
  hls::stream<ap_uint<32> > rast2_out;
  hls::stream<ap_uint<32> > zcull_out;



    read_in(Input_1, k1_in);
    projection(k1_in, prj_out);
    rasterization1(prj_out, rast1_out);
    rasterization2(rast1_out, rast2_out);
    zculling(rast2_out, zcull_out);
    coloringFB(zcull_out, Output_1);
  
  }

#endif

extern "C" {
void increment(
    hls::stream<ap_axiu<64, 0, 0, 0> >& input1,
    hls::stream<ap_axiu<64, 0, 0, 0> >& output1,
    hls::stream<ap_axiu<512, 0, 0, 0> >& input2,
    hls::stream<ap_axiu<512, 0, 0, 0> >& output2,
    hls::stream<ap_axiu<32, 0, 0, 0> >& input3,
    hls::stream<ap_axiu<32, 0, 0, 0> >& output3
    ) {
// For free running kernel, user needs to specify ap_ctrl_none for return port.
// This will create the kernel without AXI lite interface. Kernel will always be
// in running states.
#pragma HLS interface ap_ctrl_none port = return
    ap_axiu<64,  0, 0, 0> v1;
    ap_axiu<512, 0, 0, 0> v2;
    ap_axiu<32,  0, 0, 0> v3;
#pragma dataflow
    hls::stream<ap_uint<512> > Input_1("Input_1_str");
    hls::stream<ap_uint<512> > Output_1("Output_str");


    for(int i=0; i<CONFIG_SIZE; i++) {
        v1 = input1.read();
        // std::cout << "cofig_i=" << i << std::endl;
        // v1.data = v1.data + 1;
        output1.write(v1);
    }

   

 
#ifdef MONO
    rendering(input2, output2);
#else
    for(int i=0; i<INPUT_SIZE; i++){
        v2 = input2.read();
        Input_1.write(v2.data);
        std::cout << "in: " << i << std::endl;
    }
    top(Input_1, Output_1);
    for(int i=0; i<OUTPUT_SIZE; i++){
        v2.data = Output_1.read();
        output2.write(v2);
        std::cout << "out: " << i << std::endl;
    }
#endif 

    for(int i=0; i<DEBUG_SIZE; i++) {
        v3 = input3.read();
        v3.data = v3.data + 1;
        output3.write(v3);
    }

}
}
