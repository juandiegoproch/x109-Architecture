`define ZR 3'd0
`define SP 3'd1
`define R0 3'd2
`define R1 3'd3
`define R2 3'd4
`define ML 3'd5
`define JR 3'd6
`define PC 3'd7

    // Reg| Decimal |  Binary |
    //__________________________
    // ZR |    0    |   000   |
    // SP |    1    |   001   |
    // R0 |    2    |   010   |
    // R1 |    3    |   011   |
    // R2 |    4    |   100   |
    // ML |    5    |   101   |
    // JR |    6    |   110   |
    // PC |    7    |   111   |

module RegFile (
    output wire [15:0] reg_value,
    input wire [2:0] regcode,
    input wire [15:0] write_val,
    input wire write,
    input wire clk,
    input wire rst
);
    //reg [15:0] zr;
    reg [15:0] sp = 0;
    reg [15:0] r0 = 0;
    reg [15:0] r1 = 0;
    reg [15:0] r2 = 0;
    reg [15:0] ml = 0;
    reg [15:0] jr = 0;
    reg [15:0] pc = 0;

    assign reg_value =  (regcode == `ZR)? 16'h0:
                        (regcode == `SP)? sp:
                        (regcode == `R0)? r0:
                        (regcode == `R1)? r1:
                        (regcode == `R2)? r2:
                        (regcode == `ML)? ml:
                        (regcode == `JR)? jr:
                        (regcode == `PC)? pc:16'h0;


    always @(posedge clk or posedge rst) begin
        if (rst) begin
            // reset logic
            sp <= 16'h0;
            r0 <= 16'h0;
            r1 <= 16'h0;
            r2 <= 16'h0;
            ml <= 16'h0;
            jr <= 16'h0;
            pc <= 16'h0;
        end else begin
            if (write) begin
                case (regcode)
                    `SP: sp <= write_val;
                    `R0: r0 <= write_val;
                    `R1: r1 <= write_val;
                    `R2: r2 <= write_val;
                    `ML: ml <= write_val;
                    `JR: jr <= write_val;
                    `PC: pc <= write_val;
                endcase
            end
        end
    end

endmodule