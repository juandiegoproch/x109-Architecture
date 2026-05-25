`define RSEL_ZR         3'b010
`define RSEL_SP         3'b100
`define RSEL_OP2        3'b101
`define RSEL_OP1        3'b110
`define RSEL_SR         3'b111
`define RSEL_ML         3'b011
`define RSEL_JR         3'b000
`define RSEL_PC         3'b001

`define ZR 3'd0
`define SP 3'd1
`define R0 3'd2
`define R1 3'd3
`define R2 3'd4
`define ML 3'd5
`define JR 3'd6
`define PC 3'd7

`define OP1_0X2         2'b11
`define OP1_IMM         2'b10
`define OP1_0X4         2'b01
`define OP1_DLY         2'b00

`define BBSRC_RAM       2'b11
`define BBSRC_MSB       2'b10
`define BBSRC_LSB       2'b01
`define BBSRC_ALU       2'b00

module Datapath (
    // timing + reset
    input wire clk,
    input wire rst,

    // write RF, instruction reg
    input wire wi,
    input wire wrf,
    // extension logic
    input wire siex,
    input wire msb,

    // selects of "alu sources"
    input wire [1:0] op1src,
    input wire [2:0] rsel,

    input wire [3:0] alucfg,
    // backbus selects
    input wire [1:0] bbsrc,
    input wire bbdly,

    input wire [15:0] ram_rv,

    // alu outputs
    output wire neg,
    output wire zero,

    output wire [15:0] inst,

    output wire [15:0] ram_addr,
    output wire [15:0] ram_wv
);

    wire [15:0] backbus;
    wire [15:0] rfout;
    wire [15:0] imm;
    wire [15:0] rfdly_w;
    wire [15:0] aluout;
    wire [15:0] bbdly_w; //delayed backbus
    wire [15:0] bbsrc_w;
    wire [15:0] op1;

    wire [15:0] extlsb;
    wire [15:0] extmsb;
    wire nib = 0;
    wire [2:0] r_sel;

    wire [15:0] addr;
    wire [15:0] inst_dly;

    wire neg_bfr;
    wire zero_bfr;

    DFF16 bbdly_r(
        .clk(clk),
        .rst(rst),
        .d(bbsrc_w),
        .q(bbdly_w)
    );

    DFF16 addrdly_r(
        .clk(clk),
        .rst(rst),
        .d(aluout),
        .q(ram_addr)
    );

    DFF16 rfdly_r(
        .clk(clk),
        .rst(rst),
        .d(rfout),
        .q(rfdly_w)
    );

    ALU alu(
        .op1(op1),
        .op2(rfout),
        .optype(alucfg[0]),
        .neg1(alucfg[1]),
        .cin(alucfg[2]),
        .zero1(alucfg[3]),
        .out(aluout),
        .out_neg(neg_bfr),
        .out_z(zero_bfr)
    );

    Bytemixer bytemixer(
        .op1(op1),
        .op2(rfout),
        .lsb(extlsb),
        .msb(extmsb)
    );

    RegFile regfile (
        .reg_value(rfout),
        .regcode(r_sel),
        .write_val(backbus),
        .write(wrf),
        .clk(clk),
        .rst(rst)
    );

    Extend extend (
        .in(inst[7:0]),
        .sext(siex),
        .msb(msb),
        .ext(imm),
        .nibble(nib)
    );

    InstReg instreg (
        .w(wi),
        .clk(clk),
        .rst(rst),
        .wv(ram_rv),
        .rv(inst_dly)
    );
    
    DFF negdly(
        .clk(clk),
        .rst(rst),
        .d(neg_bfr),
        .q(neg)
    );

    DFF zerodly(
        .clk(clk),
        .rst(rst),
        .d(zero_bfr),
        .q(zero)
    );

    assign inst = (wi == 1)? ram_rv:inst_dly;

    // selection of register from the instruction
    wire [2:0] op1_reg = inst[7:5];
    wire [2:0] op2_reg = inst[4:2];
    wire [2:0] str_reg = inst[14:12];

    assign r_sel = (rsel == `RSEL_ZR)  ? `ZR       : // 3'b010 -> 3'd0
                    (rsel == `RSEL_SP)  ? `SP      : // 3'b100 -> 3'd1
                    (rsel == `RSEL_PC)  ? `PC      : // 3'b001 -> 3'd7
                    (rsel == `RSEL_JR)  ? `JR      : // 3'b000 -> 3'd6
                    (rsel == `RSEL_ML)  ? `ML      : // 3'b011 -> 3'd5
                    (rsel == `RSEL_OP1) ? op1_reg  : // 3'b110 -> inst[7:5]
                    (rsel == `RSEL_OP2) ? op2_reg  : // 3'b101 -> inst[4:2]
                    (rsel == `RSEL_SR)  ? str_reg  : // 3'b111 -> inst[14:12]
                                            `ZR;       // Safety Default
    // selection of operand 1
    assign op1 =      (op1src == `OP1_IMM)? imm:
                      (op1src == `OP1_0X2)? 16'h02:
                      (op1src == `OP1_DLY)? rfdly_w:
                      rfout; // to allow shift by using 2x. Not supported by ISA.

    // alu out tied to RAM input
    assign ram_wv = aluout;

    // rbackbus
    assign bbsrc_w =      (bbsrc == `BBSRC_RAM)? ram_rv: 
                          (bbsrc == `BBSRC_LSB)? extlsb:
                          (bbsrc == `BBSRC_MSB)? extmsb:
                           aluout;
    assign backbus = (bbdly == 1'b0)? bbsrc_w: bbdly_w;

endmodule