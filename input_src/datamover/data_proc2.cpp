#include "../host/typedefs.h"

void data_proc2 (
		hls::stream<ap_uint<512> > & Input_1,
		hls::stream<ap_uint<512> > & Output_1
		)
{
#pragma HLS INTERFACE axis register both port=Input_1
#pragma HLS INTERFACE axis register both port=Output_1

	ap_uint<512> tmp;

	for(int i=0; i<MAX_X; i++){
		tmp = Input_1.read();
		tmp = tmp*2;
		Output_1.write(tmp);
	}
}



