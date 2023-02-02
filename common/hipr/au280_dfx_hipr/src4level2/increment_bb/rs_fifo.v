// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================

`timescale 1ns/1ps

module rs_fifo 
#(parameter
    PAYLOAD_BITS  = 256,
    ADDR_WIDTH  = 10
)
(
    input                   clk
  , input                   rst_n
  , input                   push
  , input  [PAYLOAD_BITS-1:0] din
  , output reg              nfull
  , input                   pop
  , output [PAYLOAD_BITS-1:0] dout
  , output reg              nempty
);

localparam DEPTH = 2**ADDR_WIDTH ;

reg [PAYLOAD_BITS-1:0]  mem[0:DEPTH-1];


reg                   full;
reg                   full1;
reg                   full2;
reg                   empty;
reg                   empty_add1;
reg                   empty_add2;
reg                   wr_en;
reg                   rd_en;
reg [ADDR_WIDTH:0]    wr_ptr;    
reg [ADDR_WIDTH:0]    wr_ptr_add1;    
reg [ADDR_WIDTH:0]    wr_ptr_add2;    
reg [ADDR_WIDTH:0]    next_wr_ptr;  
reg [ADDR_WIDTH:0]    rd_ptr;  
reg [ADDR_WIDTH:0]    next_rd_ptr;  


always@(*) begin
  next_wr_ptr = wr_ptr + 1;
  wr_ptr_add1 = wr_ptr + 1;
  wr_ptr_add2 = wr_ptr + 2;
  next_rd_ptr = rd_ptr + 1;

  full        =   (wr_ptr[ADDR_WIDTH-1:0] == rd_ptr[ADDR_WIDTH-1:0])
               && (wr_ptr[ADDR_WIDTH]     != rd_ptr[ADDR_WIDTH]);
  
  full1       =   (wr_ptr_add1[ADDR_WIDTH-1:0] == rd_ptr[ADDR_WIDTH-1:0])
                   && (wr_ptr_add1[ADDR_WIDTH]     != rd_ptr[ADDR_WIDTH]); 
                            
  full2       =   (wr_ptr_add2[ADDR_WIDTH-1:0] == rd_ptr[ADDR_WIDTH-1:0])
                   && (wr_ptr_add2[ADDR_WIDTH]     != rd_ptr[ADDR_WIDTH]); 
                                    
  empty       =   (wr_ptr[ADDR_WIDTH:0]   == rd_ptr[ADDR_WIDTH:0]);
  

  nfull       = ~(full || full1 || full2);
  nempty      = ~empty;

  wr_en       = push && (~full);
  rd_en       = pop  && nempty;
  
end


always @(posedge clk) begin
  if(!rst_n) begin
    wr_ptr        <= 0;
    rd_ptr        <= 0;
  end else begin
    wr_ptr        <= ((~full)  && wr_en) ? next_wr_ptr : wr_ptr;
    rd_ptr        <= (nempty && rd_en) ? next_rd_ptr : rd_ptr;
  end
end
 


always @(posedge clk) begin
  if(wr_en) begin
    mem[wr_ptr[ADDR_WIDTH-1:0]] <= din;
  end
end

assign dout = mem[rd_ptr[ADDR_WIDTH-1:0]];



endmodule
