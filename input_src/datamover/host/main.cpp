// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <time.h>
#include <sys/time.h>

// benchmark headers
#include "typedefs.h"
#include "top.h"

#define INPUT_SIZE MAX_X
#define OUTPUT_SIZE MAX_X

// Results checking function
void check_results(bit512* output);

// ------------------------------------------------------------------------------------
// Main program
// ------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  // Variables for time measurement
  struct timeval start, end;

  // Input and output temporary variables
  bit512 in;
  bit512 out[OUTPUT_SIZE];
 
  // The input and output variables for top kernel 
  hls::stream< bit512 > Input_1("kernel_in");
  hls::stream< bit512 > Output_1("kernel_out");
 
  // Prepare the input data 
  for ( int i = 0; i < MAX_X; i++)
  {
    in = i;
    Input_1.write(in);
  }
 
  // run the kernel 
  gettimeofday(&start, NULL);
  top(Input_1, Output_1);
  gettimeofday(&end, NULL);

  // grab the data from output stream
  for(int i=0; i<OUTPUT_SIZE; i++){
    out[i] = Output_1.read();
  }

  // Check the output results
  check_results(out); 

  // print time
  long long elapsed = (end.tv_sec - start.tv_sec) * 1000000LL + end.tv_usec - start.tv_usec;   
  printf("elapsed time: %lld us\n", elapsed);


  return 0;
}

void check_results(bit512* output)
{
    unsigned int tmp;
    // read result from the 32-bit output buffer
    //for (int i=0; i<NUM_FB/16; i++){
    for (int i=0; i<MAX_X; i++){
      std::cout << "data[" << i << "]=";
      for(int j=15; j>=0; j--){
        tmp = output[i](j*32+31, j*32);
        std::cout << tmp << " ";
      }
      std::cout << std::endl;
    }
}


