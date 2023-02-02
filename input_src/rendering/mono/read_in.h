
void read_in(hls::stream<ap_axiu<512, 0, 0, 0> >& Input_1,hls::stream<ap_uint<128> >& Output_1);
#pragma map_target = HIPR
#pragma clb =1 ff = 1 bram =1 dsp =1

