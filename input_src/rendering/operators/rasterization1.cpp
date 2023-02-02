
#include "../host/typedefs.h"


// Determine whether three vertices of a trianlgLe
// (x0,y0) (x1,y1) (x2,y2) are in clockwise order by Pineda algorithm
// if so, return a number > 0
// else if three points are in line, return a number == 0
// else in counterclockwise order, return a number < 0
int check_clockwise( Triangle_2D triangle_2d )
{
  int cw;

  cw = (triangle_2d.x2 - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0)
       - (triangle_2d.y2 - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);

  return cw;

}

// swap (x0, y0) (x1, y1) of a Triangle_2D
void clockwise_vertices( Triangle_2D *triangle_2d )
{

  bit8 tmp_x, tmp_y;

  tmp_x = triangle_2d->x0;
  tmp_y = triangle_2d->y0;

  triangle_2d->x0 = triangle_2d->x1;
  triangle_2d->y0 = triangle_2d->y1;

  triangle_2d->x1 = tmp_x;
  triangle_2d->y1 = tmp_y;

}




// find the min from 3 integers
bit8 find_min( bit8 in0, bit8 in1, bit8 in2 )
{
  if (in0 < in1)
  {
    if (in0 < in2)
      return in0;
    else
      return in2;
  }
  else
  {
    if (in1 < in2)
      return in1;
    else
      return in2;
  }
}


// find the max from 3 integers
bit8 find_max( bit8 in0, bit8 in1, bit8 in2 )
{
  if (in0 > in1)
  {
    if (in0 > in2)
      return in0;
    else
      return in2;
  }
  else
  {
    if (in1 > in2)
      return in1;
    else
      return in2;
  }
}

/*======================PROCESSING STAGES========================*/



// calculate bounding box for a 2D triangle
void rasterization1 (
  hls::stream<ap_uint<64> >& Input_1,
  hls::stream<ap_uint<32> >& Output_1){
#pragma HLS INTERFACE axis register port=Input_1
#pragma HLS INTERFACE axis register port=Output_1

  #pragma HLS INLINE off
  Triangle_2D triangle_2d;
  bit8 max_min[5];
  bit16 max_index;
  bit64 in_tmp;
  bit32 out_tmp;


  TRIANGLES: for (bit16 i = 0; i < NUM_3D_TRI; i++){
    #pragma HLS PIPELINE II=1
    in_tmp = Input_1.read();
    triangle_2d.x0 = in_tmp(7,   0);
    triangle_2d.y0 = in_tmp(15,  8);
    triangle_2d.x1 = in_tmp(23, 16);
    triangle_2d.y1 = in_tmp(31, 24);
    triangle_2d.x2 = in_tmp(39, 32);
    triangle_2d.y2 = in_tmp(47, 40);
    triangle_2d.z  = in_tmp(55, 48);
  
    // clockwise the vertices of input 2d triangle
    if ( check_clockwise( triangle_2d ) == 0 ){ Output_1.write(1); continue; }
    else                                      { Output_1.write(0); }
  
    if ( check_clockwise( triangle_2d ) < 0 ) { clockwise_vertices( &triangle_2d ); }
      
    out_tmp(7,   0) = triangle_2d.x0;
    out_tmp(15,  8) = triangle_2d.y0;
    out_tmp(23, 16) = triangle_2d.x1;
    out_tmp(31, 24) = triangle_2d.y1;
    Output_1.write(out_tmp);
    out_tmp(7,   0) = triangle_2d.x2;
    out_tmp(15,  8) = triangle_2d.y2;
    out_tmp(23, 16) = triangle_2d.z ;
    Output_1.write(out_tmp);
  
    
    // find the rectangle bounds of 2D triangles
    max_min[0] = find_min( triangle_2d.x0, triangle_2d.x1, triangle_2d.x2 );
    max_min[1] = find_max( triangle_2d.x0, triangle_2d.x1, triangle_2d.x2 );
    max_min[2] = find_min( triangle_2d.y0, triangle_2d.y1, triangle_2d.y2 );
    max_min[3] = find_max( triangle_2d.y0, triangle_2d.y1, triangle_2d.y2 );
    max_min[4] = max_min[1] - max_min[0];
  
    // calculate index for searching pixels
    max_index = (max_min[1] - max_min[0]) * (max_min[3] - max_min[2]);
  
    out_tmp(7,   0) = max_min[0](7, 0);
    out_tmp(15,  8) = max_min[1](7, 0);
    out_tmp(23, 16) = max_min[2](7, 0);
    out_tmp(31, 24) = max_min[3](7, 0);
    Output_1.write(out_tmp);
    out_tmp(7,   0) = max_min[4](7, 0);
    out_tmp(23,  8) = max_index(15, 0);
    Output_1.write(out_tmp);
  }
  
}