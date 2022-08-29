// color the frame buffer
void coloringFB_bot_m(
		hls::stream<ap_uint<32> > & Input_1,
		hls::stream<ap_uint<128> > & Output_1);

#pragma clb =4 ff=1 bram =2.4 dsp =1.2
