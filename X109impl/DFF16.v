module DFF16 (
    input wire clk,      // Clock signal
    input wire rst,    // Asynchronous reset (active low)
    input wire [15:0] d,        // Input signal to be delayed
    output reg [15:0] q         // Delayed output signal (must be 'reg' type)
);

    // This block triggers every time the clock goes high or reset goes low
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            q <= 16'b0;   // Clear the output on reset
        end else begin
            q <= d;      // Capture the input on the rising edge of the clock
        end
    end

endmodule