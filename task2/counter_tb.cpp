#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;   // simulation time
    int clk; // clock signal

    Verilated::commandArgs(argc, argv);

    // INIT TOP INSTANCE
    Vcounter *top = new Vcounter;

    // INIT TRACE DUMP
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // INIT VBUDDY
    if (vbdOpen() != 1)
        return -1;
    vbdHeader("Lab1: Counter");

    // INITIAL VALUES
    top->clk = 1;
    top->rst = 1;
    top->en = 1;

    // RUN SIMULATION FOR A WHILE
    for (i = 0; i < 3000; i++)
    {
        // dump to vcd file and toggle clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // DISPLAY ON VBUDDY
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i + i);

        // APPLY RESET AND ENABLE LOGIC
        top->rst = (i < 2) | (i == 15);
        top->en = vbdFlag();

        if (Verilated::gotFinish())
            exit(0);
    }

    // CLOSE VBUDDY AND TRACE FILES
    vbdClose();
    tfp->close();
    exit(0);
}
