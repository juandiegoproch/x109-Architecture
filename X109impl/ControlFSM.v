`define STATE_FETCH     4'b0000
`define STATE_INST_WB   4'b0001
`define STATE_OP1_FETCH 4'b0010
`define STATE_COMP      4'b0011
`define STATE_WB        4'b0100
`define STATE_JMP_ADDR  4'b0101
`define STATE_JMP       4'b0110
`define STATE_MIXBYTE   4'b0111
`define STATE_ADDR      4'b1000
`define STATE_WB_RAM    4'b1001
`define STATE_WB_SP     4'b1010
`define STATE_CRASH     4'b1111

`define CFG_LDL         4'b0000
`define CFG_LDM         4'b0100

`define CFG_ST          4'b1001
`define CFG_LD          4'b0001
`define CFG_POP         4'b0010
`define CFG_PUSH        4'b1010

`define ALUCFG_ADD      4'b0000
`define ALUCFG_MOV      4'b1111

`define JCND_NO         2'b00
`define JCND_N          2'b01
`define JCND_Z          2'b10
`define JCND_NZ         2'b11

`define RSEL_ZR         3'b010
`define RSEL_SP         3'b100
`define RSEL_OP2        3'b101
`define RSEL_OP1        3'b110
`define RSEL_SR         3'b111
`define RSEL_ML         3'b011
`define RSEL_JR         3'b000
`define RSEL_PC         3'b001

`define OP1_0X2         2'b11
`define OP1_IMM         2'b10
`define OP1_0X4         2'b01
`define OP1_DLY         2'b00

`define BBSRC_RAM       2'b11
`define BBSRC_MSB       2'b10
`define BBSRC_LSB       2'b01
`define BBSRC_ALU       2'b00


module ControlFSM (
    // timing + reset
    input wire clk,
    input wire rst,

    // write RF, instruction reg
    output wire wi,
    output wire wrf,
    // extension logic
    output wire siex,
    output wire msb,
    output wire nib,

    // selects of "alu sources"
    output wire [1:0] op1src,
    output wire [2:0] rsel,

    output wire [3:0] alucfg,
    // backbus selects
    output wire [1:0] bbsrc,
    output wire bbdly,

    // ram write
    output wire ram_w,

    // alu outputs
    input wire neg,
    input wire zero,

    input wire [15:0] inst
);

reg [3:0] state = `STATE_FETCH;

wire [3:0] cfg  = inst[11:8];
wire       op   = inst[15];

wire [1:0] jcnd = inst[1:0];
wire       jn   = inst[1];
wire       jz   = inst[0];

always @(posedge clk or posedge rst) begin
    if (rst)
        state <= `STATE_FETCH;
    else begin
        case (state)

            `STATE_FETCH:
                state <= `STATE_INST_WB;

            `STATE_INST_WB:
                if (op == 1'b1) begin
                    state <= `STATE_OP1_FETCH;
                end else if ((op == 1'b0) && (cfg == `CFG_LDL || cfg == `CFG_LDM)) begin
                    state <= `STATE_MIXBYTE;
                end else if ((op == 1'b0) && (cfg == `CFG_ST || cfg == `CFG_LD || cfg == `CFG_POP || cfg == `CFG_PUSH)) begin
                    state <= `STATE_ADDR;
                end else begin
                    state <= `STATE_CRASH;
                end

            `STATE_OP1_FETCH:
                state <= `STATE_COMP;

            `STATE_COMP:
                if (jcnd == `JCND_NO) begin
                    state <= `STATE_WB;
                end else begin
                    state <= `STATE_JMP_ADDR;
                end

            `STATE_WB:
                state <= `STATE_FETCH;

            `STATE_JMP_ADDR:
                state <= `STATE_JMP;

            `STATE_JMP:
                state <= `STATE_FETCH;

            `STATE_MIXBYTE:
                state <= `STATE_FETCH;

            `STATE_ADDR:
                state <= `STATE_WB_RAM;

            `STATE_WB_RAM:
                if (cfg == `CFG_PUSH) begin 
                    state <= `STATE_WB_SP;
                end else begin
                    state <= `STATE_FETCH;
                end

            `STATE_WB_SP:
                state <= `STATE_FETCH;
            default:
                state <= `STATE_CRASH;
        endcase
    end
end

// combinational logic

assign wi    = (state == `STATE_INST_WB);

assign msb   = (state == `STATE_MIXBYTE) ? inst[10] : 1'b0;

assign wcpsr = (state == `STATE_COMP);

assign ram_w = (state == `STATE_WB_RAM) &&
               ((cfg == `CFG_ST) || (cfg == `CFG_PUSH));

assign bbdly =
    (state == `STATE_INST_WB)   ? 1'b0  :
    (state == `STATE_OP1_FETCH) ? 1'b1  :
    (state == `STATE_COMP)      ? 1'b1  :
    (state == `STATE_WB)        ? 1'b1  :
    (state == `STATE_JMP_ADDR)  ? 1'b1  :
    (state == `STATE_JMP)       ? 1'b1  :
    (state == `STATE_MIXBYTE)   ? 1'b0  :
    (state == `STATE_ADDR)      ? 1'b0  :
    (state == `STATE_WB_RAM)    ? 1'b0  :
    (state == `STATE_WB_SP)     ? 1'b0  :
                                  1'b1;

assign siex =
    (state == `STATE_MIXBYTE)   ? 1'b0  :
    (state == `STATE_ADDR)      ? 1'b1  :
                                  1'b0;

assign alucfg =
    (state == `STATE_FETCH)     ? `ALUCFG_MOV :
    (state == `STATE_INST_WB)   ? `ALUCFG_ADD :
    (state == `STATE_COMP)      ? cfg  :
    (state == `STATE_JMP_ADDR)  ? `ALUCFG_MOV :
    (state == `STATE_ADDR)      ? `ALUCFG_ADD :
    (state == `STATE_WB_SP)     ? `ALUCFG_ADD :
                                  4'b1111;

assign op1src =     
    (state == `STATE_INST_WB)   ? `OP1_0X2  :
    (state == `STATE_COMP)      ? `OP1_DLY  :
    (state == `STATE_JMP_ADDR)  ? `OP1_DLY  :
    (state == `STATE_ADDR)      ? `OP1_IMM  :
    (state == `STATE_WB_SP)     ? `OP1_0X2  :
    (state == `STATE_MIXBYTE)   ? `OP1_IMM  :
                                  `OP1_DLY;

assign bbsrc =
    (state == `STATE_FETCH)     ? `BBSRC_ALU  :
    (state == `STATE_INST_WB)   ? `BBSRC_ALU  :
    (state == `STATE_COMP)      ? `BBSRC_ALU  :
    (state == `STATE_JMP_ADDR)  ? `BBSRC_ALU  :
    (state == `STATE_MIXBYTE)   ? (inst[10]?`BBSRC_MSB:`BBSRC_LSB) :
    (state == `STATE_WB_RAM)    ? `BBSRC_RAM  :
                                  `BBSRC_RAM;

assign rsel =
    (state == `STATE_FETCH)     ? `RSEL_PC  :
    (state == `STATE_INST_WB)   ? `RSEL_PC  :
    (state == `STATE_OP1_FETCH) ? `RSEL_OP1  :
    (state == `STATE_COMP)      ? `RSEL_OP2  :
    (state == `STATE_WB)        ? `RSEL_SR  :
    (state == `STATE_JMP_ADDR)  ? `RSEL_JR  :
    (state == `STATE_JMP)       ? `RSEL_PC  :
    (state == `STATE_MIXBYTE)   ? `RSEL_SR  :
    (state == `STATE_ADDR)      ? (inst[8] == 1?`RSEL_ML:`RSEL_SP) :
    (state == `STATE_WB_RAM)    ? `RSEL_SR  :
    (state == `STATE_WB_SP)     ? `RSEL_SP  :
                                  `RSEL_ZR;

wire wrf_jmp = ((jn && !jz && neg) || (jz && zero && neg) || (jz&&zero&&!jn))?1:0;

assign wrf =
    (state == `STATE_FETCH)     ? 1'b0  :
    (state == `STATE_INST_WB)   ? 1'b1  :
    (state == `STATE_OP1_FETCH) ? 1'b0  :
    (state == `STATE_COMP)      ? 1'b0  :
    (state == `STATE_WB)        ? 1'b1  :
    (state == `STATE_JMP_ADDR)  ? 1'b0  :
    (state == `STATE_JMP)       ? wrf_jmp  :
    (state == `STATE_MIXBYTE)   ? 1'b1  :
    (state == `STATE_ADDR)      ? ((cfg == `CFG_POP)?1:0):
    (state == `STATE_WB_RAM)    ? ((cfg == `CFG_POP|| cfg == `CFG_LD)?1:0):
    (state == `STATE_WB_SP)     ? 1'b1  :
                                  1'b0;

endmodule
