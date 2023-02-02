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
void increment(
    hls::stream<ap_axiu<32, 0, 0, 0> >& input1,
    hls::stream<ap_axiu<32, 0, 0, 0> >& output1,
    hls::stream<ap_axiu<32, 0, 0, 0> >& input2,
    hls::stream<ap_axiu<32, 0, 0, 0> >& output2,
    hls::stream<ap_axiu<32, 0, 0, 0> >& input3,
    hls::stream<ap_axiu<32, 0, 0, 0> >& output3
    ) {
// For free running kernel, user needs to specify ap_ctrl_none for return port.
// This will create the kernel without AXI lite interface. Kernel will always be
// in running states.
#pragma HLS interface ap_ctrl_none port = return

    ap_axiu<32, 0, 0, 0> v1 = input1.read();
    v1.data = v1.data + 1;
    output1.write(v1);

    ap_axiu<32, 0, 0, 0> v2 = input2.read();
    v2.data = v2.data + 1;
    output2.write(v2);

    ap_axiu<32, 0, 0, 0> v3 = input3.read();
    v3.data = v3.data + 1;
    output3.write(v3);
}
}
