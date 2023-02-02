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
void mem_read1(ap_uint<64> * mem, hls::stream<ap_axiu<64, 0, 0, 0> >& stream, int size) {
mem_read1:
    for (int i = 0; i < size; i++) {
        ap_uint<64> a = mem[i];
        ap_axiu<64, 0, 0, 0> v;
        v.data = a;
        stream.write(v);
    }
}
}
