#include <systemc.h>
#include "clkUnit.h"
#include "txUnit.h"

void test_tx_unit() {
	// Declare signals
	sc_clock clk("clk",25,SC_NS);
	sc_signal<sc_logic>         reset_sig;
	sc_signal<sc_logic>         en_tx_sig;
	sc_signal<sc_logic>         en_rx_sig;
    sc_signal<sc_lv<8>>   data_in_sig;
    sc_signal<sc_logic>         load_sig;
    sc_signal<sc_logic>     	txd_sig;
    sc_signal<sc_logic>         reg_empty_sig;
    sc_signal<sc_logic>         buf_empty_sig;
	
	// Declare ClkUnit
	clkUnit clkUnit_inst("ClkUnit");
		clkUnit_inst.sysclk(clk);
        clkUnit_inst.reset(reset_sig);
        clkUnit_inst.en_tx(en_tx_sig);
        clkUnit_inst.en_rx(en_rx_sig);

	// Declare TxUnit
	txUnit txUnit_inst("TxUnit");
		txUnit_inst.sysclk(clk);
        txUnit_inst.reset(reset_sig);
        txUnit_inst.enable(en_tx_sig);
        txUnit_inst.load(load_sig);
        txUnit_inst.data_in(data_in_sig);
        txUnit_inst.txd_send(txd_sig);
        txUnit_inst.reg_empty(reg_empty_sig);
        txUnit_inst.buf_empty(buf_empty_sig);

	// Trace
	sc_trace_file *tf = sc_create_vcd_trace_file("wave_txunit");
	sc_write_comment(tf, "Simulation of Tx Unit");
	tf->set_time_unit(1, SC_NS);

	sc_trace(tf, clk, "clock");
	sc_trace(tf, reset_sig, "reset");
	sc_trace(tf, en_tx_sig, "tx_en");
	sc_trace(tf, en_rx_sig, "rx_en");
	sc_trace(tf, data_in_sig, "data_in");
	sc_trace(tf, load_sig, "load");
	sc_trace(tf, txd_sig, "txd");
	sc_trace(tf, reg_empty_sig, "reg_empty");
	sc_trace(tf, buf_empty_sig, "buf_empty");

	// Reset
	cout << sc_time_stamp() << ": " << "Reset ..." << endl;
	load_sig.write(SC_LOGIC_0);
	reset_sig.write(SC_LOGIC_1);
	sc_start(1, SC_US);
	reset_sig.write(SC_LOGIC_0);
	sc_start(1, SC_US);

	// Send 0x11 during 10 us
	cout << sc_time_stamp() << ": " << "Load ..." << endl;
	data_in_sig.write(0x11);
	load_sig.write(SC_LOGIC_1);
	sc_start(50, SC_NS);
	load_sig.write(SC_LOGIC_0);
	sc_start(600, SC_US);

	// Send 0x22 and 0x33 without transition (... b7,Stop,Start,b0...)

	data_in_sig.write(0xAA);
	load_sig.write(SC_LOGIC_1);
	sc_start(50, SC_NS);
	load_sig.write(SC_LOGIC_0);
	sc_start(2400, SC_US);

	sc_close_vcd_trace_file(tf);
}
