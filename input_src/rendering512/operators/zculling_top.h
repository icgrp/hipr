
// filter hidden pixels
void zculling_top (
		hls::stream<ap_uint<32> > & Input_1,
		hls::stream<ap_uint<32> > & Input_2,
		hls::stream<ap_uint<32> > & Output_1
	  );
#pragma map_target = HIPR  page_num = 17 inst_mem_size = 65536
#pragma clb =4 ff=1 bram =2.4 dsp =1.2

