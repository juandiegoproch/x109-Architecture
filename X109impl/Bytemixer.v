module Bytemixer (
   input wire [15:0] op1,
   input wire [15:0] op2,
   output wire [15:0] lsb,
   output wire [15:0] msb
);
    assign lsb = {op2[15:8], op1[7:0]};
    assign msb = {op1[15:8], op2[7:0]};
    
endmodule