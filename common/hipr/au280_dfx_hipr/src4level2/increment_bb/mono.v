module mono(
  input         ap_clk,
  input         ap_rst_n,
  input [511:0]  Input_1_TDATA,
  input         Input_1_TVALID,
  output        Input_1_TREADY,
  output [511:0] Output_1_TDATA,
  output        Output_1_TVALID,
  input         Output_1_TREADY,
  input         ap_start);
wire [511:0] DMA_Input_1_TDATA;
wire        DMA_Input_1_TVALID;
wire        DMA_Input_1_TREADY;
wire [511:0] DMA_Output_1_TDATA;
wire        DMA_Output_1_TVALID;
wire        DMA_Output_1_TREADY;
wire [511:0] data_proc1_Input_1_TDATA;
wire        data_proc1_Input_1_TVALID;
wire        data_proc1_Input_1_TREADY;
wire [511:0] data_proc1_Output_1_TDATA;
wire        data_proc1_Output_1_TVALID;
wire        data_proc1_Output_1_TREADY;

RelayStation #(
  .PAYLOAD_BITS(512),
  .NUM_BRAM_ADDR_BITS(7)
  )stream_link_0(
  .clk(ap_clk),
  .din(DMA_Output_1_TDATA),
  .val_in(DMA_Output_1_TVALID),
  .ready_upward(DMA_Output_1_TREADY),
  .dout(data_proc1_Input_1_TDATA),
  .val_out(data_proc1_Input_1_TVALID),
  .ready_downward(data_proc1_Input_1_TREADY),
  .reset(~ap_rst_n));

stream_shell #(
  .PAYLOAD_BITS(512),
  .NUM_BRAM_ADDR_BITS(7)
  )stream_link_1(
  .clk(ap_clk),
  .din(data_proc1_Output_1_TDATA),
  .val_in(data_proc1_Output_1_TVALID),
  .ready_upward(data_proc1_Output_1_TREADY),
  .dout(DMA_Input_1_TDATA),
  .val_out(DMA_Input_1_TVALID),
  .ready_downward(DMA_Input_1_TREADY),
  .reset(~ap_rst_n));

  data_proc1 data_proc1_inst(
    .ap_clk(ap_clk),
    .ap_start(ap_start),
    .ap_done(),
    .ap_idle(),
    .ap_ready(),
    .Input_1_TDATA(data_proc1_Input_1_TDATA),
    .Input_1_TVALID(data_proc1_Input_1_TVALID),
    .Input_1_TREADY(data_proc1_Input_1_TREADY),
    .Output_1_TDATA(data_proc1_Output_1_TDATA),
    .Output_1_TVALID(data_proc1_Output_1_TVALID),
    .Output_1_TREADY(data_proc1_Output_1_TREADY),
    .ap_rst_n(ap_rst_n)
  );
assign Output_1_TDATA  = DMA_Input_1_TDATA;
assign Output_1_TVALID = DMA_Input_1_TVALID;
assign DMA_Input_1_TREADY = Output_1_TREADY;
assign DMA_Output_1_TDATA  = Input_1_TDATA;
assign DMA_Output_1_TVALID = Input_1_TVALID;
assign Input_1_TREADY = DMA_Output_1_TREADY;
endmodule
