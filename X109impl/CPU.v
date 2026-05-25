module CPU (
    input wire clk,
    input wire rst,

    // Memory Interface
    input  wire [15:0] ram_rv,   // RAM Read Value
    output wire [15:0] ram_addr, // RAM Address
    output wire [15:0] ram_wv,   // RAM Write Value
    output wire ram_w            // RAM Write Enable
);

    // Internal Control Wires
    wire wi, wrf, siex, msb, nib, bbdly, wcpsr;
    wire [1:0] op1src;
    wire [2:0] rsel;
    wire [3:0] alucfg;
    wire [1:0] bbsrc;
    
    // Internal Feedback Wires
    wire neg, zero;
    wire [15:0] inst;

    // 1. Control Unit (The Brain)
    ControlFSM controller (
        .clk(clk),
        .rst(rst),
        .wi(wi),
        .wrf(wrf),
        .siex(siex),
        .msb(msb),
        //.nib(nib),
        //.wcpsr(wcpsr),      // Pulse to update flags
        .op1src(op1src),
        .rsel(rsel),        // Now 3 bits
        .alucfg(alucfg),
        .bbsrc(bbsrc),
        .bbdly(bbdly),
        .ram_w(ram_w),
        .neg(neg),          // Input from ALU
        .zero(zero),        // Input from ALU
        .inst(inst)         // Input from Datapath InstReg
    );

    // 2. Datapath (The Muscle)
    Datapath datapath (
        .clk(clk),
        .rst(rst),
        .wi(wi),
        .wrf(wrf),
        .siex(siex),
        .msb(msb),
        //.nib(nib),
        .op1src(op1src),
        .rsel(rsel),        // Now 3 bits
        .alucfg(alucfg),
        .bbsrc(bbsrc),
        .bbdly(bbdly),
        .ram_rv(ram_rv),
        .neg(neg),          // Output to FSM
        .zero(zero),        // Output to FSM
        .inst(inst),        // Output to FSM
        .ram_addr(ram_addr),
        .ram_wv(ram_wv)
    );

endmodule