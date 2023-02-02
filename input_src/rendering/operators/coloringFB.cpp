#include "../host/typedefs.h"

// color the frame buffer
void coloringFB(hls::stream<ap_uint<32> >& Input_1, hls::stream<ap_uint<512> >& Output_1)
{
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1
  #pragma HLS INLINE off
  Pixel pixel;
  bit32 in_tmp;
  bit32 size_pixels;
  bit8 frame_buffer[MAX_X][MAX_Y];
  bit512 out_FB;

  for (int counter = 0; counter < NUM_3D_TRI; counter++){
    size_pixels = Input_1.read();
  
    if ( counter == 0 )
    {
      // initilize the framebuffer for a new image
      COLORING_FB_INIT_ROW: for ( bit16 i = 0; i < MAX_X; i++)
      {
        #pragma HLS PIPELINE II=1
        COLORING_FB_INIT_COL: for ( bit16 j = 0; j < MAX_Y; j++)
          frame_buffer[i][j] = 0;
      }
    }
  
    // update the framebuffer
    COLORING_FB: for ( bit16 i = 0; i < size_pixels; i++)
    {
      #pragma HLS PIPELINE II=1
      in_tmp      = Input_1.read();
      pixel.x     = in_tmp(7, 0);
      pixel.y     = in_tmp(15, 8);
      pixel.color = in_tmp(23, 16);
      frame_buffer[ pixel.x ][ pixel.y ] = pixel.color;
    }
  }

  SEND:for (int i=0; i<MAX_X; i++){
    for(int j=0; j<MAX_Y; j+=64){
      for (int k=0; k<64; k++){
        #pragma HLS PIPELINE II=1
        out_FB( k*8+7,  k*8) = frame_buffer[i][j+k];
      }
     Output_1.write(out_FB);
    }
  }

}
