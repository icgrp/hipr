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

extern "C" {
void mem_write3(hls::stream<ap_axiu<32, 0, 0, 0> >& stream, ap_uint<32> * mem, int size) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem5 latency = 50  num_write_outstanding= 128 num_read_outstanding=1
mem_write3:
    for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
        ap_axiu<32, 0, 0, 0> v = stream.read();
        mem[i] = v.data;
    }
}
}