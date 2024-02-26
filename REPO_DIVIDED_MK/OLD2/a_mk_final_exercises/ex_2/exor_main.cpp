#include <systemc.h>

#include "stim.h"
#include "exor.h"
#include "mon.h"

// MK_ADDED Code for GTK WAVE SIMULATOR
// For GTKWave Simulator
#include <sysc/datatypes/fx/sc_fixed.h>
#include <sysc/datatypes/fx/sc_ufixed.h>
#include <sysc/utils/sc_report.h>

sc_trace_file *tf = sc_create_vcd_trace_file("waves");
// Until here

int sc_main(int, char**)
{
    sc_signal<bool> sigA, sigB, sigZ;

    stim Stim1("Stimulus");
    Stim1.A(sigA);
    Stim1.B(sigB);

    exor DUT("exor");
    DUT.A(sigA);
    DUT.B(sigB);
    DUT.Z(sigZ);

    Monitor mon("Monitor");
    mon.A(sigA);
    mon.B(sigB);
    mon.Z(sigZ);

    // sc_start();  // run forever  // MK_CHANGED RUN forever to specific time

    // For GTKWave Simulator:
    sc_trace(tf, sigA, "signalA");
    sc_trace(tf, sigB, "signalB");
    sc_trace(tf, sigZ, "signalZ");
    
    sc_start(50, SC_NS); // run for 50ns
    
    // Fot GTKWave Simulator:
    sc_close_vcd_trace_file(tf);    //At the end of your main function, close the trace file
    
    return 0;
}
