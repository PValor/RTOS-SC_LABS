#include <systemc.h>
#include "clkUnit.h"

void test_clk_unit() {
	// Declare signal
	sc_clock clk("clk",25,SC_NS);
	sc_signal<sc_logic>         reset_sig;
	sc_signal<sc_logic>         en_tx_sig;
	sc_signal<sc_logic>         en_rx_sig;
	
	// Declare ClkUnit
	clkUnit clkUnit_inst("ClkUnit");
		clkUnit_inst.sysclk(clk);
        clkUnit_inst.reset(reset_sig);
        clkUnit_inst.en_tx(en_tx_sig);
        clkUnit_inst.en_rx(en_rx_sig);

	// Trace
	sc_trace_file *tf = sc_create_vcd_trace_file("wave_clkunit");
	sc_write_comment(tf, "Simulation of Clk Unit");
	tf->set_time_unit(1, SC_NS);
	sc_trace(tf, clk, "clock");
	sc_trace(tf, reset_sig, "reset");
	sc_trace(tf, en_tx_sig, "tx_en");
	sc_trace(tf, en_rx_sig, "rx_en");

	cout << "simulation ..."  << endl;

	for (int i=0; i<5; i++) {
		sc_start(1, SC_MS);
		cout << ".";
	}
	cout << endl;

	// CLose Trace
	sc_close_vcd_trace_file(tf);
}
