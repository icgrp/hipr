
// filter hidden pixels
void zculling_top (
		hls::stream<ap_uint<32> > & Input_1,
		hls::stream<ap_uint<32> > & Input_2,
		hls::stream<ap_uint<32> > & Output_1
	  );
#pragma map_target = HIPR 
#pragma clb =4 ff=1 bram =6 dsp =1.2

