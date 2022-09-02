void data_redir_m (
		hls::stream<ap_uint<128> > & Input_1,
		hls::stream<ap_uint<32> > & Output_1,
		hls::stream<ap_uint<32> > & Output_2
		);

#pragma map_target = HIPR 
#pragma clb =8 ff = 1 bram =6 dsp =1.2


