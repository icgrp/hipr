
void read_in(hls::stream<ap_uint<512> >& Input_1,hls::stream<ap_uint<128> >& Output_1);
#pragma map_target = HIPR
#pragma clb =10 ff = 1 bram =2 dsp =1

