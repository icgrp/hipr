/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

/*******************************************************************************
Description:
    Kernel to kernel streaming example consisting of three compute
    uints in a linear hardware pipeline.

    1) Memory read
        This Kernel reads the input vector from Global Memory and streams its
output.

    2) Increment
        This Kernel reads stream input, increments the value and streams to
output.

    3) Memory write
        This Kernel reads from its input stream and writes into Global Memory.

                     _____________
                    |             |<----- Global Memory
                    |  mem_read   |
                    |_____________|------+
                     _____________       | AXI4 Stream
                    |             |<-----+
                    |  increment  |
                    |_____________|----->+
                     ______________      | AXI4 Stream
                    |              |<----+
                    |  mem_write   |
                    |______________|-----> Global Memory


*******************************************************************************/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <xcl2.hpp>
#include "typedefs.h"
#include "input_data.h"


void event_cb(cl_event event1, cl_int cmd_status, void* data) {
    cl_int err;
    cl_command_type command;
    cl::Event event(event1, true);
    OCL_CHECK(err, err = event.getInfo<cl_command_type>(CL_EVENT_COMMAND_TYPE, &command));
    cl_int status;
    OCL_CHECK(err, err = event.getInfo<cl_int>(CL_EVENT_COMMAND_EXECUTION_STATUS, &status));

    const char* command_str;
    const char* status_str;
    switch (command) {
        case CL_COMMAND_READ_BUFFER:
            command_str = "buffer read";
            break;
        case CL_COMMAND_WRITE_BUFFER:
            command_str = "buffer write";
            break;
        case CL_COMMAND_NDRANGE_KERNEL:
            command_str = "kernel";
            break;
    }
    switch (status) {
        case CL_QUEUED:
            status_str = "Queued";
            break;
        case CL_SUBMITTED:
            status_str = "Submitted";
            break;
        case CL_RUNNING:
            status_str = "Executing";
            break;
        case CL_COMPLETE:
            status_str = "Completed";
            break;
    }
    printf("%s %s %s\n", status_str, reinterpret_cast<char*>(data), command_str);
    fflush(stdout);
}


// Sets the callback for a particular event
void set_callback(cl::Event event, const char* queue_name) {
    cl_int err;
    OCL_CHECK(err, err = event.setCallback(CL_COMPLETE, event_cb, (void*)queue_name));
}

// Check the output data
void check_results(bit512* output);

int main(int argc, char** argv) {
    //if (argc != 2) {
    //    std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
    //    return EXIT_FAILURE;
    //}

    std::string binaryFile = argv[argc-1];
    std::vector<cl::Event> kernel_wait_events;
    cl_int err;
    cl::CommandQueue q;
    cl::Context context;
    cl::Kernel krnl_mem_read1, krnl_mem_write1;
    cl::Kernel krnl_mem_read2, krnl_mem_write2;
    cl::Kernel krnl_mem_read3, krnl_mem_write3;


    // Reducing the data size for emulation mode
    char* xcl_mode = getenv("XCL_EMULATION_MODE");
    if (xcl_mode != nullptr) {
        std::cout << "We are performing Emulation" << std::endl;
    }

    // Allocate Memory in Host Memory
    size_t vector_size_bytes1 = sizeof(ap_uint<64>)  * CONFIG_SIZE;
    size_t vector_size_bytes2 = sizeof(ap_uint<512>) * INPUT_SIZE;
    size_t vector_size_bytes3 = sizeof(ap_uint<64>)  * CONFIG_SIZE;
    size_t vector_size_bytes4 = sizeof(ap_uint<512>) * OUTPUT_SIZE;
    size_t vector_size_bytes5 = sizeof(ap_uint<32> ) * CONFIG_SIZE;


    std::vector<ap_uint<64>,  aligned_allocator<ap_uint<64>>  > source_input1     (CONFIG_SIZE);
    std::vector<ap_uint<512>, aligned_allocator<ap_uint<512>> > source_input2     (INPUT_SIZE );
    std::vector<ap_uint<32>,  aligned_allocator<ap_uint<32>>  > source_input3     (CONFIG_SIZE);
    std::vector<ap_uint<64>,  aligned_allocator<ap_uint<64>>  > source_hw_results1(CONFIG_SIZE);
    std::vector<ap_uint<512>, aligned_allocator<ap_uint<512>> > source_hw_results2(OUTPUT_SIZE);
    std::vector<ap_uint<32>,  aligned_allocator<ap_uint<32>>  > source_hw_results3(CONFIG_SIZE);
    std::vector<ap_uint<64>,  aligned_allocator<ap_uint<64>>  > source_sw_results (CONFIG_SIZE);

    // Create the test data and Software Result
    // configure packets
    // source_input1[0] = 1;
    // source_input1[1] = 2;
    // source_input1[2] = 3;
    // source_input1[3] = 4;
    // source_input1[4] = 5;
//zculling_bot.Output_1->coloringFB_bot_m.Input_1
      source_input1[0].range(63, 32) = 0x00007800; 
      source_input1[0].range(31,  0) = 0x98100fe0;
      source_input1[1].range(63, 32) = 0x00008080; 
      source_input1[1].range(31,  0) = 0x27c80000;
//rasterization2_m.Output_3->zculling_top.Input_2
      source_input1[2].range(63, 32) = 0x00007000; 
      source_input1[2].range(31,  0) = 0xb2980fe0;
      source_input1[3].range(63, 32) = 0x00002880; 
      source_input1[3].range(31,  0) = 0x37580000;
//data_redir_m.Output_1->rasterization2_m.Input_1
      source_input1[4].range(63, 32) = 0x00001800; 
      source_input1[4].range(31,  0) = 0x97100fe0;
      source_input1[5].range(63, 32) = 0x00007080; 
      source_input1[5].range(31,  0) = 0x21c80000;
//DMA.Output_1->data_transfer.Input_1
      source_input1[6].range(63, 32) = 0x00000800; 
      source_input1[6].range(31,  0) = 0x93100fe0;
      source_input1[7].range(63, 32) = 0x00003080; 
      source_input1[7].range(31,  0) = 0x20c80000;
//coloringFB_bot_m.Output_1->coloringFB_top_m.Input_2
      source_input1[8].range(63, 32) = 0x00008000; 
      source_input1[8].range(31,  0) = 0x91180fe0;
      source_input1[9].range(63, 32) = 0x00001080; 
      source_input1[9].range(31,  0) = 0x38480000;
//zculling_top.Output_1->coloringFB_top_m.Input_1
      source_input1[10].range(63, 32) = 0x00002800; 
      source_input1[10].range(31,  0) = 0x91100fe0;
      source_input1[11].range(63, 32) = 0x00001080; 
      source_input1[11].range(31,  0) = 0x22c80000;
//rasterization2_m.Output_1->zculling_top.Input_1
      source_input1[12].range(63, 32) = 0x00007000; 
      source_input1[12].range(31,  0) = 0x92900fe0;
      source_input1[13].range(63, 32) = 0x00002880; 
      source_input1[13].range(31,  0) = 0x27480000;
//data_redir_m.Output_2->rasterization2_m.Input_2
      source_input1[14].range(63, 32) = 0x00001800; 
      source_input1[14].range(31,  0) = 0xa7180fe0;
      source_input1[15].range(63, 32) = 0x00007080; 
      source_input1[15].range(31,  0) = 0x31d00000;
//coloringFB_top_m.Output_1->DMA.Input_1
      source_input1[16].range(63, 32) = 0x00001000; 
      source_input1[16].range(31,  0) = 0x90900fe0;
      source_input1[17].range(63, 32) = 0x00000880; 
      source_input1[17].range(31,  0) = 0x21480000;
//data_transfer.Output_1->data_redir_m.Input_1
      source_input1[18].range(63, 32) = 0x00003000; 
      source_input1[18].range(31,  0) = 0x91900fe0;
      source_input1[19].range(63, 32) = 0x00001880; 
      source_input1[19].range(31,  0) = 0x23480000;
//rasterization2_m.Output_4->zculling_bot.Input_2
      source_input1[20].range(63, 32) = 0x00007000; 
      source_input1[20].range(31,  0) = 0xc7980fe0;
      source_input1[21].range(63, 32) = 0x00007880; 
      source_input1[21].range(31,  0) = 0x37600000;
//rasterization2_m.Output_2->zculling_bot.Input_1
      source_input1[22].range(63, 32) = 0x00007000; 
      source_input1[22].range(31,  0) = 0xa7900fe0;
      source_input1[23].range(63, 32) = 0x00007880; 
      source_input1[23].range(31,  0) = 0x27500000;
      // start page15; 
      source_input1[24].range(63, 32) = 0x00007802; 
      source_input1[24].range(31,  0) = 0x00000000;
      // start page5; 
      source_input1[25].range(63, 32) = 0x00002802; 
      source_input1[25].range(31,  0) = 0x00000000;
      // start page16; 
      source_input1[26].range(63, 32) = 0x00008002; 
      source_input1[26].range(31,  0) = 0x00000000;
      // start page2; 
      source_input1[27].range(63, 32) = 0x00001002; 
      source_input1[27].range(31,  0) = 0x00000000;
      // start page3; 
      source_input1[28].range(63, 32) = 0x00001802; 
      source_input1[28].range(31,  0) = 0x00000000;
      // start page14; 
      source_input1[29].range(63, 32) = 0x00007002; 
      source_input1[29].range(31,  0) = 0x00000000;
      // start page6; 
      source_input1[30].range(63, 32) = 0x00003002; 
      source_input1[30].range(31,  0) = 0x00000000;
    // end of configure packets

    for ( int i = 0; i < NUM_3D_TRI/4; i++)
    {
    	for (int j=0; j<4; j++){
		  source_input2[i](128*j+7,  128*j+0)   = triangle_3ds[4*i+j].x0;
		  source_input2[i](128*j+15, 128*j+8)   = triangle_3ds[4*i+j].y0;
		  source_input2[i](128*j+23, 128*j+16)  = triangle_3ds[4*i+j].z0;
		  source_input2[i](128*j+31, 128*j+24)  = triangle_3ds[4*i+j].x1;
		  source_input2[i](128*j+39, 128*j+32)  = triangle_3ds[4*i+j].y1;
		  source_input2[i](128*j+47, 128*j+40)  = triangle_3ds[4*i+j].z1;
		  source_input2[i](128*j+55, 128*j+48)  = triangle_3ds[4*i+j].x2;
		  source_input2[i](128*j+63, 128*j+56)  = triangle_3ds[4*i+j].y2;
		  source_input2[i](128*j+71, 128*j+64)  = triangle_3ds[4*i+j].z2;
		  source_input2[i](128*j+127,128*j+72)  = 0;
    	}
    }

    for (size_t i = 0; i < CONFIG_SIZE; i++) {
        source_input3[i].range(31, 0) = i;
        for(int j=0; j<2; j++){source_sw_results[i].range(j*32+31, j*32) = i;}
        source_sw_results[i] = source_sw_results[i] + 1;
    }

    // OPENCL HOST CODE AREA START
    // get_xil_devices() is a utility API which will find the Xilinx
    // platforms and will return list of devices connected to Xilinx platform
    std::vector<cl::Device> devices = xcl::get_xil_devices();

    // read_binary_file() is a utility API which will load the binaryFile
    // and will return pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;




        // Load xclbin
        for (int i = 1; i < argc-1; i++)
        {
            char *xclbinFilename = argv[i];
            std::cout << "Loading: '" << xclbinFilename << "'\n";
            std::ifstream bin_file(xclbinFilename, std::ifstream::binary);
            bin_file.seekg(0, bin_file.end);
            unsigned nb = bin_file.tellg();
            bin_file.seekg(0, bin_file.beg);
            char *buf = new char[nb];
            bin_file.read(buf, nb);

            // Creating Program from Binary File
            cl::Program::Binaries bins;
            bins.push_back({buf, nb});
            devices.resize(1);
            cl::Program program(context, devices, bins);
        }
        std::cout << "Done!" << std::endl;




        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, krnl_mem_read1 = cl::Kernel(program, "mem_read1", &err));
            OCL_CHECK(err, krnl_mem_read2 = cl::Kernel(program, "mem_read2", &err));
            OCL_CHECK(err, krnl_mem_read3 = cl::Kernel(program, "mem_read3", &err));
            OCL_CHECK(err, krnl_mem_write1 = cl::Kernel(program, "mem_write1", &err));
            OCL_CHECK(err, krnl_mem_write2 = cl::Kernel(program, "mem_write2", &err));
            OCL_CHECK(err, krnl_mem_write3 = cl::Kernel(program, "mem_write3", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    // Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
    // Device-to-host communication
    std::cout << "Creating Buffers..." << std::endl;
    OCL_CHECK(err, cl::Buffer buffer_input1(context,
                                           CL_MEM_USE_HOST_PTR, // | CL_INCREMENT_ONLY,
                                           vector_size_bytes1, source_input1.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_input2(context,
                                           CL_MEM_USE_HOST_PTR, // | CL_INCREMENT_ONLY,
                                           vector_size_bytes2, source_input2.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_input3(context,
                                           CL_MEM_USE_HOST_PTR, // | CL_INCREMENT_ONLY,
                                           vector_size_bytes5, source_input3.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_output1(context,
                                            CL_MEM_USE_HOST_PTR, // | CL_MEM_WRITE_ONLY,
                                            vector_size_bytes1, source_hw_results1.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_output2(context,
                                            CL_MEM_USE_HOST_PTR, // | CL_MEM_WRITE_ONLY,
                                            vector_size_bytes4, source_hw_results2.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_output3(context,
                                            CL_MEM_USE_HOST_PTR, // | CL_MEM_WRITE_ONLY,
                                            vector_size_bytes5, source_hw_results3.data(), &err));




    // Set the Kernel Arguments
    
    OCL_CHECK(err, err = krnl_mem_read1.setArg(0, buffer_input1));
    OCL_CHECK(err, err = krnl_mem_read1.setArg(2, CONFIG_SIZE));
    OCL_CHECK(err, err = krnl_mem_read2.setArg(0, buffer_input2));
    OCL_CHECK(err, err = krnl_mem_read2.setArg(2, INPUT_SIZE));
    OCL_CHECK(err, err = krnl_mem_read3.setArg(0, buffer_input3));
    OCL_CHECK(err, err = krnl_mem_read3.setArg(2, CONFIG_SIZE));
 
    OCL_CHECK(err, err = krnl_mem_write1.setArg(1, buffer_output1));
    OCL_CHECK(err, err = krnl_mem_write1.setArg(2, CONFIG_SIZE));
    OCL_CHECK(err, err = krnl_mem_write2.setArg(1, buffer_output2));
    OCL_CHECK(err, err = krnl_mem_write2.setArg(2, OUTPUT_SIZE));
    OCL_CHECK(err, err = krnl_mem_write3.setArg(1, buffer_output3));
    OCL_CHECK(err, err = krnl_mem_write3.setArg(2, CONFIG_SIZE));
    
    std::vector<cl::Event> kernel_events(6);
    bool match = true;

    // Launch the Kernel
    std::cout << "Launching Kernel..." << std::endl;


    // Kernel 1 related
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input1}, 0 /*0 means from host*/));
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_read1,  nullptr, &kernel_events[0]));
    set_callback(kernel_events[0], "memread1");
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_write1, nullptr, &kernel_events[3]));
    set_callback(kernel_events[3], "memwrite1");
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output1}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    int max_config = CONFIG_SIZE; //> 20 ? 20: CONFIG_SIZE;
    for(int i=0; i<max_config; i++){
        printf("%d: %08x_%08x\n", i, (unsigned int)source_hw_results1[i].range(63, 32), (unsigned int) source_hw_results1[i].range(31, 0));
    }


    // Kernel 2 related
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input2}, 0 /*0 means from host*/));
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_read2,  nullptr, &kernel_events[1]));
    set_callback(kernel_events[1], "memwread2");
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_write2, nullptr, &kernel_events[4]));
    set_callback(kernel_events[4], "memwrite2");
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output2}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    for(int i=0; i<10; i++){
        printf("%08x", (unsigned int)source_hw_results2[0].range(31, 0)); 
        for(int j=1; j<16; j++) { printf("_%08x", (unsigned int)source_hw_results2[i].range(32*j+31, 32*j)); }
        printf("\n");
    }


    // Kernel 3 related
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input3}, 0 /*0 means from host*/));
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_read3,  nullptr, &kernel_events[2]));
    set_callback(kernel_events[2], "memread3");
    OCL_CHECK(err, err = q.enqueueTask(krnl_mem_write3, nullptr, &kernel_events[5]));
    set_callback(kernel_events[5], "memwrite3");
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output3}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.finish()); /////////////////////////////////////////////////////////////
    for(int i=0; i<max_config; i++){
        printf("-%d: %08x\n", i, (unsigned int) source_hw_results3[i].range(31, 0));
    }

    
    
   
    // kernel_wait_events.resize(0);
    // kernel_wait_events.push_back(kernel_events[3]);
    
    
    
    bit512 output[OUTPUT_SIZE];
    for(int i=0; i<OUTPUT_SIZE; i++){
      output[i] = source_hw_results2[i];
    }
    
    //check_results(output);

    // OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input3}, 0 /*0 means from host*/));
    // OCL_CHECK(err, err = q.enqueueTask(krnl_mem_read3,  nullptr, &kernel_events[2]));
    // set_callback(kernel_events[2], "memread3");
    //OCL_CHECK(err, err = q.enqueueTask(krnl_mem_write3, &kernel_wait_events, &kernel_events[5]));
    // OCL_CHECK(err, err = q.enqueueTask(krnl_mem_write3, nullptr, &kernel_events[5]));
    // set_callback(kernel_events[5], "memwrite3");


    // wait for all kernels to finish their operations
   
    std::cout << "Getting Results..." << std::endl;
    // Copy Result from Device Global Memory to Host Local Memory
    
    
    // OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output3}, CL_MIGRATE_MEM_OBJECT_HOST));
    
    // OPENCL HOST CODE AREA END

    

    

    std::cout << "TEST " << (match ? "PASSED" : "FAILED") << std::endl;
    return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}


void check_results(bit512* output)
{
    bit8 frame_buffer_print[MAX_X][MAX_Y];

    // read result from the 32-bit output buffer
    for (int i=0; i<NUM_FB/16; i++){
      for(int j=0; j<64; j++){
        int n=i*64+j;
        int row = n/256;
        int col = n%256;
        frame_buffer_print[row][col] = output[i](8*j+7, 8*j);
      }
    }

  // print result
  {
    for (int j = MAX_X - 1; j >= 0; j -- )
    {
      for (int i = 0; i < MAX_Y; i ++ )
      {
        int pix;
        pix = frame_buffer_print[i][j].to_int();
        if (pix){
          std::cout << "1";
        }else{
          std::cout << "0";
        }
      }
      std::cout << std::endl;
    }
  }

}
