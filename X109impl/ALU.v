module ALU (
    input wire [15:0] op1,
    input wire [15:0] op2,
    input wire optype,
    input wire neg1,
    input wire cin,
    input wire zero1,
    output wire [15:0] out,
    output wire out_neg,
    output wire out_z
);
    wire [15:0] opt_preproc1 = (op1 ^ {16{neg1}}) & ~{16{zero1}};
    wire [15:0]  log_op = opt_preproc1 | op2;
    wire [15:0] arit_op = (opt_preproc1 + cin) + op2;
    assign out = optype == 0? arit_op:log_op;



    // flags
    assign out_neg = out[15];
    assign out_z = !(|out);

endmodule