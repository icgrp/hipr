void data_transfer (
		hls::stream<ap_uint<512> > & Input_1,
		hls::stream<ap_uint<128> > & Output_1
		);

#pragma map_target = HIPR page_num = 16 inst_mem_size = 32768
#pragma clb =8 ff = 1 bram =2.4 dsp =1.2

