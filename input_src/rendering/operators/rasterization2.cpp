#include "../host/typedefs.h"

// Given a pixel, determine whether it is inside the triangle
// by Pineda algorithm
// if so, return true
// else, return false
bit1 pixel_in_triangle( bit8 x, bit8 y, Triangle_2D triangle_2d )
{

  int pi0, pi1, pi2;

  pi0 = (x - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0) - (y - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);
  pi1 = (x - triangle_2d.x1) * (triangle_2d.y2 - triangle_2d.y1) - (y - triangle_2d.y1) * (triangle_2d.x2 - triangle_2d.x1);
  pi2 = (x - triangle_2d.x2) * (triangle_2d.y0 - triangle_2d.y2) - (y - triangle_2d.y2) * (triangle_2d.x0 - triangle_2d.x2);

  return (pi0 >= 0 && pi1 >= 0 && pi2 >= 0);
}

// find pixels in the triangles from the bounding box
void rasterization2 ( hls::stream<ap_uint<32> >& Input_1, hls::stream<ap_uint<32> >& Output_1) {
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1

  #pragma HLS INLINE off
  bit32 out_tmp;
  CandidatePixel fragment2[500]; 
  bit32 flag;
  bit8 max_min[5];
  bit16 max_index;
  Triangle_2D triangle_2d;
  bit32 in_tmp;


  TRIANGLES: for (bit16 angle_num = 0; angle_num < NUM_3D_TRI; angle_num++){
    flag = Input_1.read();
    
    // clockwise the vertices of input 2d triangle
    if ( flag==1 ) { Output_1.write(0); continue; }
  
    in_tmp = Input_1.read();
    triangle_2d.x0 = in_tmp(7,   0);
    triangle_2d.y0 = in_tmp(15,  8);
    triangle_2d.x1 = in_tmp(23, 16);
    triangle_2d.y1 = in_tmp(31, 24);
    in_tmp = Input_1.read();
    triangle_2d.x2 = in_tmp(7,   0);
    triangle_2d.y2 = in_tmp(15,  8);
    triangle_2d.z  = in_tmp(23, 16);
  
    in_tmp = Input_1.read();
    max_min[0](7, 0) = in_tmp(7,   0);
    max_min[1](7, 0) = in_tmp(15,  8);
    max_min[2](7, 0) = in_tmp(23, 16);
    max_min[3](7, 0) = in_tmp(31, 24);
  
    in_tmp = Input_1.read();
    max_min[4](7, 0)   = in_tmp(7,   0) ;
    max_index(15, 0)   = in_tmp(23,  8);
  
  
  
    bit8 color = 100;
    bit16 i = 0;
  
    RAST2: for ( bit16 k = 0; k < max_index; k++ )
    {
      #pragma HLS PIPELINE II=1
      bit8 x = max_min[0] + k%max_min[4];
      bit8 y = max_min[2] + k/max_min[4];
  
      if( pixel_in_triangle( x, y, triangle_2d ) )
      {
        fragment2[i].x = x;
        fragment2[i].y = y;
        fragment2[i].z = triangle_2d.z;
        fragment2[i].color = color;
        i++;
      }
    }
  
    Output_1.write(i);
    for(int o_num = 0; o_num < i; o_num++ ) {
      #pragma HLS PIPELINE II=1
      out_tmp(7,   0) = fragment2[o_num].x;
      out_tmp(15,  8) = fragment2[o_num].y;
      out_tmp(23, 16) = fragment2[o_num].z;
      out_tmp(31, 24) = fragment2[o_num].color;
      Output_1.write(out_tmp);
    }
  }
}
