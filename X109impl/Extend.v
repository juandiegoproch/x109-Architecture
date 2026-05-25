module Extend (
    input wire [7:0] in,
    input wire sext,
    input wire msb,
    input wire nibble,
    output wire [15:0] ext
);
    wire [15:0] sign_extended = {{8{in[7]}},in};
    wire [15:0] zero_extended_lsb = {8'b0,in};
    wire [15:0] zero_extended_msb = {in,8'b0};

    wire [15:0] sign_extended_nibble = {{12{in[3]}},in[3:0]};

    assign ext =    (nibble == 1)? sign_extended_nibble:
                    (sext == 1)? sign_extended: 
                    (msb  == 1)? zero_extended_msb:
                                 zero_extended_lsb;
endmodule