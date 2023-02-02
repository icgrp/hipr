
`timescale 1ns / 1ps
/*=============================================================================
Name         : rs_dff.cpp
Author       : Yuanlong Xiao
Version      : 1.0
Copyright    : Your copyright notice
Affiliation  : IC group, University of Pennsylvania
Contributor  : Yuanlong Xiao
Create Date  : 12/09/2022
Design Name  : rs_dff
Project Name : Direct Wire
Dependencies : Verilog Compiler
Description  : This is a Verilog source to pipeline the streaing data 
Revision     :
               Revision 0.00 - File Created
comment      :
=============================================================================*/



module rs_dff #(
parameter PAYLOAD_BITS=1
)(
    input                      ap_clk
  , input                      ap_rst_n
  , input  [PAYLOAD_BITS-1: 0] din
  , output [PAYLOAD_BITS-1: 0] dout
);

reg [PAYLOAD_BITS-1: 0] dout_reg;

always@(posedge ap_clk) begin
  if(!ap_rst_n) begin
    dout_reg <= 0;
  end else begin
    dout_reg <= din;
  end
end

assign dout = dout_reg;


endmodule

