module InstReg (
    input wire w,
    input wire clk,
    input wire rst,
    input wire [15:0] wv,
    output wire [15:0] rv
);
    reg [15:0] r;
    assign rv = r;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            r <= 16'h0;
        end else if (w) begin
            r <= wv;
        end
    end
endmodule