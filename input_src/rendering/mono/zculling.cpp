#include "../host/typedefs.h"


// filter hidden pixels
void zculling (hls::stream<ap_uint<32> >& Input_1,
               hls::stream<ap_uint<32> >& Output_1)
{
  #pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1
  #pragma HLS INLINE off
  bit16 size;
  bit32 out_tmp;
  Pixel pixels[500];
  CandidatePixel fragment;
  bit8 z_buffer[MAX_X][MAX_Y];
  bit32 in_tmp;

  for (int counter = 0; counter < NUM_3D_TRI; counter++){
    // pixel counter
    bit32 pixel_cntr = 0;
    
    size = Input_1.read();
    // initilize the z-buffer in rendering first triangle for an image
    
    if (counter == 0)
    {
      ZCULLING_INIT_ROW: for ( bit16 i = 0; i < MAX_X; i++)
      {
        #pragma HLS PIPELINE II=1
        ZCULLING_INIT_COL: for ( bit16 j = 0; j < MAX_Y; j++)
        {
          z_buffer[i][j] = 255;
        }
      }
    }
    // update z-buffer and pixels
    ZCULLING: for ( bit16 n = 0; n < size; n++ )
    {
      in_tmp = Input_1.read();
      fragment.x     = in_tmp(7,   0);
      fragment.y     = in_tmp(15,  8);
      fragment.z     = in_tmp(23, 16);
      fragment.color = in_tmp(31, 24);
      //printf("%d_%d_%d_%d\n", (unsigned int) fragment.x, (unsigned int) fragment.y, (unsigned int) fragment.z, (unsigned int) fragment.color);
      #pragma HLS PIPELINE II=1
      if( fragment.z < z_buffer[fragment.y][fragment.x] )
      {
        pixels[pixel_cntr].x     = fragment.x;
        pixels[pixel_cntr].y     = fragment.y;
        pixels[pixel_cntr].color = fragment.color;
        pixel_cntr++;
        z_buffer[fragment.y][fragment.x] = fragment.z;
      }
    }
    
    Output_1.write(pixel_cntr);
    for(int o_num=0; o_num<pixel_cntr; o_num++){
      out_tmp(7,   0) = pixels[o_num].x;
      out_tmp(15,  8) = pixels[o_num].y;
      out_tmp(23, 16) = pixels[o_num].color;
      Output_1.write(out_tmp);
    }
  
  }

}

