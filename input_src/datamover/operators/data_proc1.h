

void data_proc1 (
		hls::stream<ap_uint<512> > & Input_1,
		hls::stream<ap_uint<512> > & Output_1);
#pragma map_target = HIPR
#pragma clb =8 ff = 1 bram =10 dsp =1.2
