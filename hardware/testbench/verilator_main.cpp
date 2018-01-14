//
// Copyright 2011-2015 Jeff Bush
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <iostream>
#include <stdlib.h>
#include "Vsoc_tb.h"
#include "verilated.h"
#include "verilated_vpi.h"
#if VM_TRACE
#include <verilated_vcd_c.h>
#endif
using namespace std;

//
// This is compiled into the verilog simulator executable along with the
// source files generated by Verilator. It initializes and runs the simulation
// loop for the full processor.
//

namespace
{
vluint64_t currentTime = 0;
}

// Called whenever the $time variable is accessed.
double sc_time_stamp()
{
    return currentTime;
}

int main(int argc, char **argv, char **env)
{
    Verilated::commandArgs(argc, argv);
    Verilated::debug(0);

    // Default to randomizing contents of memory before the start of
    // simulation. This flag can disable it.
    const char *arg = Verilated::commandArgsPlusMatch("randomize=");
    if (arg[0] == '\0' || atoi(arg + 11) != 0)
    {
        // Initialize random seed.
        long randomSeed;
        arg = Verilated::commandArgsPlusMatch("randseed=");
        if (arg[0] != '\0')
            randomSeed = atol(arg + 10);
        else
        {
            time_t t1;
            time(&t1);
            randomSeed = (long) t1;
        }

        srand48(randomSeed);
        VL_PRINTF("Random seed is %li\n", randomSeed);
        Verilated::randReset(2);
    }
    else
        Verilated::randReset(0);

    Vsoc_tb* testbench = new Vsoc_tb;

    // As with real hardware, reset is a bit tricky.
    // - Most assertions will fail before the design has been reset.
    // - Assertions are not tested while reset is asserted.
    // BUT:
    // - Many blocks require a positive edge on reset to trigger
    //   (not all, any block that also triggers on clock will synchronously
    //   reset if it is asserted).
    //
    // This is a bit of a hack, set the 'last' state of reset to zero and reset to one.
    // This will cause a positive edge event on the next eval() that will trigger
    // all reset blocks. Reset will be deasserted in the main loop below.
    //
    testbench->__Vclklast__TOP__reset = 0;
    testbench->reset = 1;
    testbench->clk = 0;
    testbench->eval();

#if VM_TRACE // If verilator was invoked with --trace
    Verilated::traceEverOn(true);
    VL_PRINTF("Writing waveform to trace.vcd\n");
    VerilatedVcdC* tfp = new VerilatedVcdC;
    testbench->trace(tfp, 99);
    tfp->open("trace.vcd");
#endif

    while (!Verilated::gotFinish())
    {
        // Allow it to run for a few clock cycles with reset asserted. This allows
        // flops that are not reset to settle on valid values so assertions don't trip.
        if (currentTime == 4)
            testbench->reset = 0;

        testbench->clk = !testbench->clk;
        testbench->eval();
#if VM_TRACE
        tfp->dump(currentTime); // Create waveform trace for this timestamp
#endif

        currentTime++;
    }

#if VM_TRACE
    tfp->close();
#endif

    testbench->final();
    delete testbench;

    return 0;
}
