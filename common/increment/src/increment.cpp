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
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "typedefs.h"


void data_copy64(hls::stream<ap_uint<64> > & Input_1,
	 	         hls::stream<ap_uint<64> > & Output_1) {
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1
	ap_uint<64> in_tmp;
	in_tmp = Input_1.read();
	in_tmp = in_tmp + 1;
	Output_1.write(in_tmp);
}

void data_copy32(hls::stream<ap_uint<32> > & Input_1,
	 	         hls::stream<ap_uint<32> > & Output_1) {
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1
	ap_uint<32> in_tmp;
	in_tmp = Input_1.read();
	in_tmp = in_tmp + 1;
	Output_1.write(in_tmp);
}



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


    bit512 v2_buffer[8192];   // Local memory to store vector2
    #pragma HLS STREAM variable=v2_buffer depth=8192



    for(int i=0; i<CONFIG_SIZE; i++){
#pragma HLS PIPELINE II=1
        ap_axiu<64, 0, 0, 0> v1 = input1.read();
        v1.data = v1.data + 1;
        output1.write(v1);
    }


    ap_axiu<512, 0, 0, 0> v2_1;
    ap_axiu<512, 0, 0, 0> v2_2;

    for(int i=0; i<INPUT_SIZE/8192; i++){
#pragma HLS dataflow
      for(int j=0; j<8192; j++){
#pragma HLS PIPELINE II=1
        v2_1 = input2.read();
        v2_buffer[j] = v2_1.data;
      }
      for(int j=0; j<8192; j++){
#pragma HLS PIPELINE II=1
        v2_2.data = v2_buffer[j];
        output2.write(v2_2);
      }
    }



    for(int i=0; i<DEBUG_SIZE; i++) {
#pragma HLS PIPELINE II=1
        ap_axiu<32, 0, 0, 0> v3 = input3.read();
        v3.data = v3.data + 1;
        output3.write(v3);
    }
    
}
}
