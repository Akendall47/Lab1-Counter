//counts of posedge of clk if en == 1
//synchronously reset to 0 if rst asserted
//counts by concatenating en 1 to width
// replciate x N times : N{x}

module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input  logic              clk,   // clock
    input  logic              rst,   // reset
    input  logic              en,    // counter enable
    output logic [WIDTH-1:0]  count  // count output
);

   always_ff @(posedge clk or posedge rst)
        if (rst) count <= {WIDTH{1'b0}};
        else     count <= count + {{WIDTH-1{1'b0}}, en};
    
endmodule
