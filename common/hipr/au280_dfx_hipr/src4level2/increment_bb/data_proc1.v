`timescale 1ns / 1ps
module data_proc1(
    input wire ap_clk,
    input wire ap_start,
    output reg ap_done,
    output reg ap_idle,
    output reg ap_ready,
    input wire [511:0] Input_1_TDATA,
    input wire Input_1_TVALID,
    output reg Input_1_TREADY,
    output reg [511:0] Output_1_TDATA,
    output reg Output_1_TVALID,
    input wire Output_1_TREADY,
    input wire ap_rst_n
    );



endmodule

