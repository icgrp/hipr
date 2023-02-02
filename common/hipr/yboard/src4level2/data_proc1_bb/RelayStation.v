
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/08/2020 07:14:32 PM
// Design Name: 
// Module Name: RelayStation
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module RelayStation #(
    parameter NUM_BRAM_ADDR_BITS = 7,
    parameter PAYLOAD_BITS=32
    )(
    input clk,
    input reset,
    input ready_downward,
    output reg val_out,
    output reg ready_upward,
    input val_in,
    input [PAYLOAD_BITS-1:0] din,
    output [PAYLOAD_BITS-1:0] dout
    );
    localparam EMPTY = 2'b00;
    localparam HALF  = 2'b01;
    localparam FULL  = 2'b10;
    localparam ERR   = 2'b11;
    
    
    reg [1:0] state;
    reg [1:0] next_state;
    reg main_en;
    reg aux_en;
    reg main_val;
    
    reg [PAYLOAD_BITS-1:0] main;
    reg [PAYLOAD_BITS-1:0] aux;
    
    always@(posedge clk) begin
        if(reset)
            state <= EMPTY;
        else
            state <= next_state;
    end
    
    
    // next_state;
    // main_en;
    // aux_en;
    // main_val;
    // val_out;
    // stop_upward   
    always@(*) begin
        case(state)
            EMPTY: begin
                if(!val_in) begin
                    next_state = EMPTY;
                    main_en  = 0;
                    aux_en   = 0;
                    main_val = 0;
                    val_out  = 0;
                    ready_upward = 1;
                end else begin
                    next_state = HALF;
                    main_en  = 1;
                    aux_en   = 0;
                    main_val = 0;
                    val_out  = 0;
                    ready_upward = 1;
                end
            end
            
            HALF: begin
                if((!val_in) && (!ready_downward)) begin
                    next_state = HALF;
                    main_en = 0;
                    aux_en = 0;
                    main_val = 0;
                    val_out  = 0;
                    ready_upward = 1;
                end else if (val_in && ready_downward) begin
                    next_state = HALF;
                    main_en = 1;
                    aux_en  = 0;
                    main_val = 1;
                    val_out = 1;
                    ready_upward = 1;
                end else if (val_in && (!ready_downward)) begin 
                    next_state = FULL;
                    main_en = 1;
                    aux_en  = 1;
                    main_val = 0;
                    val_out = 0;
                    ready_upward = 1;
                end else begin
                    next_state = EMPTY;
                    main_en = 0;
                    aux_en = 0;
                    main_val = 1;
                    val_out = 1;
                    ready_upward = 1;
                end
            end
            
            FULL: begin
                if(!ready_downward) begin
                    next_state = FULL;
                    main_en = 0;
                    aux_en = 0;
                    main_val = 0;
                    val_out = 0;
                    ready_upward = 0;
                end else begin
                    next_state = HALF;
                    main_val = 0;
                    val_out = 1;
                    main_en = 0;
                    aux_en = 0;
                    ready_upward = 0;
                end
            end
                
            ERR: begin
                next_state = 0;
                main_en = 0;
                aux_en = 0;
                main_val = 0;
                val_out = 0;
                ready_upward = 0;    
            end 
        endcase
    end
    
    always@(posedge clk) begin
        if(reset) begin
            main <= 0;
            aux  <= 0;
        end else begin
            if(main_en)
                main <= din;
            else
                main <= main;
            if(aux_en) 
                aux <= main;
            else
                aux <= aux;
        end
    end   
    
    
    assign dout = main_val ? main : aux; 
        
    
endmodule

