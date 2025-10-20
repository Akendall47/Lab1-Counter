#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"
#include <iostream>

int main(int argc, char **argv, char **env)
{
    Verilated::commandArgs(argc, argv);

    Vcounter *top = new Vcounter;

    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    vbdOpen();
    vbdSetMode(1); // ONE-SHOT mode
    vbdHeader("Task 3: Loadable Counter");

    top->clk = 0;
    top->rst = 1; // Start with reset
    top->ld = 0;
    top->v = 0;

    for (int i = 0; i < 10000; i++)
    {
        // Toggle clock
        top->clk ^= 1;
        top->eval();

        if (top->clk) // Rising edge
        {
            // Release reset after first cycle
            if (i < 4)
            {
                top->rst = 1;
            }
            else
            {
                top->rst = 0;

                // Read Vbuddy inputs
                int value = vbdValue(); // 0-255 from rotary encoder
                int flag = vbdFlag();   // 1 when button pressed (ONE-SHOT)

                // Load counter when button is pressed
                top->ld = flag;
                top->v = value;
            }

            // Display on Vbuddy (8-bit counter = 2 hex digits)
            vbdHex(4, (top->count >> 4) & 0xF); // Upper nibble
            vbdHex(3, top->count & 0xF);        // Lower nibble
            vbdHex(2, 0);                       // Clear unused digits
            vbdHex(1, 0);

            // Plot counter value
            vbdPlot(int(top->count), 0, 255);

            vbdCycle(i / 2); // Cycle number (divide by 2 for clock cycles)
        }

        tfp->dump(i);

        if (Verilated::gotFinish())
            break;
    }

    vbdClose();
    tfp->close();
    delete top;
    return 0;
}