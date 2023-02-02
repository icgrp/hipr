#include "../host/typedefs.h"


// project a 3D triangle to a 2D triangle
void projection (hls::stream<ap_uint<128> >& Input_1, hls::stream<ap_uint<64> >& Output_1) {
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1
  #pragma HLS INLINE off
  bit2 angle = 0;
  Triangle_3D triangle_3ds;
  Triangle_2D triangle_2ds;
  bit64 out_tmp;
  
  TRIANGLES: for (bit16 i = 0; i < NUM_3D_TRI; i++){
    #pragma HLS PIPELINE II=1

    bit128 in_tmp = Input_1.read();
    bit32 input_lo  = in_tmp(31,  0 );
    bit32 input_mi  = in_tmp(63,  32);
    bit32 input_hi  = in_tmp(95,  64);
  
    triangle_3ds.x0 = input_lo( 7,  0);
    triangle_3ds.y0 = input_lo(15,  8);
    triangle_3ds.z0 = input_lo(23, 16);
    triangle_3ds.x1 = input_lo(31, 24);
    triangle_3ds.y1 = input_mi( 7,  0);
    triangle_3ds.z1 = input_mi(15,  8);
    triangle_3ds.x2 = input_mi(23, 16);
    triangle_3ds.y2 = input_mi(31, 24);
    triangle_3ds.z2 = input_hi( 7,  0);
  
  
    // Setting camera to (0,0,-1), the canvas at z=0 plane
    // The 3D model lies in z>0 space
    // The coordinate on canvas is proportional to the corresponding coordinate
    // on space
    if(angle == 0)
    {
      triangle_2ds.x0 = triangle_3ds.x0;
      triangle_2ds.y0 = triangle_3ds.y0;
      triangle_2ds.x1 = triangle_3ds.x1;
      triangle_2ds.y1 = triangle_3ds.y1;
      triangle_2ds.x2 = triangle_3ds.x2;
      triangle_2ds.y2 = triangle_3ds.y2;
      triangle_2ds.z  = triangle_3ds.z0 / 3 + triangle_3ds.z1 / 3 + triangle_3ds.z2 / 3;
    }
  
    else if(angle == 1)
    {
      triangle_2ds.x0 = triangle_3ds.x0;
      triangle_2ds.y0 = triangle_3ds.z0;
      triangle_2ds.x1 = triangle_3ds.x1;
      triangle_2ds.y1 = triangle_3ds.z1;
      triangle_2ds.x2 = triangle_3ds.x2;
      triangle_2ds.y2 = triangle_3ds.z2;
      triangle_2ds.z  = triangle_3ds.y0 / 3 + triangle_3ds.y1 / 3 + triangle_3ds.y2 / 3;
    }
  
    else if(angle == 2)
    {
      triangle_2ds.x0 = triangle_3ds.z0;
      triangle_2ds.y0 = triangle_3ds.y0;
      triangle_2ds.x1 = triangle_3ds.z1;
      triangle_2ds.y1 = triangle_3ds.y1;
      triangle_2ds.x2 = triangle_3ds.z2;
      triangle_2ds.y2 = triangle_3ds.y2;
      triangle_2ds.z  = triangle_3ds.x0 / 3 + triangle_3ds.x1 / 3 + triangle_3ds.x2 / 3;
    }


    out_tmp(7,   0) = triangle_2ds.x0;
    out_tmp(15,  8) = triangle_2ds.y0;
    out_tmp(23, 16) = triangle_2ds.x1;
    out_tmp(31, 24) = triangle_2ds.y1;
    out_tmp(39, 32) = triangle_2ds.x2;
    out_tmp(47, 40) = triangle_2ds.y2;
    out_tmp(55, 48) = triangle_2ds.z;

    Output_1.write(out_tmp);
  }

}
