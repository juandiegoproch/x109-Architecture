`timescale 1ns/1ps

module tb_integrated_cpu();

    reg clk;
    reg rst;

    wire [15:0] ram_rv;
    wire [15:0] ram_addr;
    wire [15:0] ram_wv;
    wire ram_w;

    integer cycle;

    // =========================================================
    // DUT
    // =========================================================
    CPU dut (
        .clk(clk),
        .rst(rst),
        .ram_rv(ram_rv),
        .ram_addr(ram_addr),
        .ram_wv(ram_wv),
        .ram_w(ram_w)
    );

    // =========================================================
    // BYTE-ADDRESSABLE RAM
    // =========================================================
    reg [7:0] mem [0:255];

    // 16-bit little-endian read
    assign ram_rv = {
        mem[ram_addr + 1],
        mem[ram_addr]
    };

    // Optional write support
    always @(posedge clk) begin
        if (ram_w) begin
            mem[ram_addr]     <= ram_wv[7:0];
            mem[ram_addr + 1] <= ram_wv[15:8];
        end
    end

    // =========================================================
    // Per-cycle reporting
    // =========================================================
    always @(posedge clk) begin
        cycle <= cycle + 1;

        $display(
            "[cycle %0d] time=%0t  PC(addr)=0x%04h  ram_rv=0x%04h  mem[0]=0x%02h  mem[1]=0x%02h",
            cycle,
            $time,
            ram_addr,
            ram_rv,
            mem[0],
            mem[1]
        );
    end

    // =========================================================
    // Test
    // =========================================================
    initial begin

        $dumpfile("cpu_12cycles.vcd");
        $dumpvars(0, tb_integrated_cpu);

        // Initial state
        clk   = 0;
        rst   = 0;
        cycle = 0;

        // =====================================================
        // Program image
        // Address 0x0000 -> 16'h0900
        // =====================================================

        mem[16'h0000] = 8'h00; // low byte
        mem[16'h0001] = 8'hA0; // high byte

        mem[16'h0002] = 8'hFC; // low byte
        mem[16'h0003] = 8'hA0; // high byte

        // EXACTLY 12 posedges
        repeat (18) begin
            #1 clk = 1;
            #1 clk = 0;
        end

        $display("Finished exactly 12 clock cycles.");

        $dumpflush;
        $finish;
    end

endmodule