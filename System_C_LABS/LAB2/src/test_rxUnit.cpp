#include <systemc.h>
#include "clkUnit.h"
#include "rxUnit.h"
#include "txUnit.h"

void test_rx_unit() {
	// Declare signals
	sc_clock clk("clk",25,SC_NS);
	sc_signal<bool>         reset_sig;
	sc_signal<bool>         en_tx_sig;
	sc_signal<bool>         en_rx_sig;
    sc_signal<bool>         read_sig;
    sc_signal<bool>         load_sig;
    sc_signal<bool>         rxd_sig;
    sc_signal<bool>         txd_sig;
    sc_signal<sc_uint<8>>   data_out_sig;
    sc_signal<sc_uint<8>>   data_in_sig;
    sc_signal<bool>         frame_err_sig;
    sc_signal<bool>         output_err_sig;
    sc_signal<bool>         data_rdy_sig;
    sc_signal<bool>         reg_empty_sig;
    sc_signal<bool>         buf_empty_sig;
	
	// Declare ClkUnit
	clkUnit clkUnit_inst("ClkUnit");
		clkUnit_inst.sysclk(clk);
        clkUnit_inst.reset(reset_sig);
        clkUnit_inst.en_tx(en_tx_sig);
        clkUnit_inst.en_rx(en_rx_sig);

	// Declare RxUnit
	rxUnit rxUnit_inst("RxUnit");
		rxUnit_inst.sysclk(clk);
        rxUnit_inst.reset(reset_sig);
        rxUnit_inst.enable(en_rx_sig);
        rxUnit_inst.read(read_sig);
        rxUnit_inst.rxd_in(txd_sig);
        rxUnit_inst.data_out(data_out_sig);
        rxUnit_inst.frame_err(frame_err_sig);
        rxUnit_inst.output_err(output_err_sig);
        rxUnit_inst.data_rdy(data_rdy_sig);

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
	sc_trace_file *tf = sc_create_vcd_trace_file("wave_rxunit");
	sc_write_comment(tf, "Simulation of Rx Unit");
	tf->set_time_unit(1, SC_NS);

	sc_trace(tf, clk, "clock");
	sc_trace(tf, reset_sig, "reset");
	sc_trace(tf, en_tx_sig, "tx_en");
	sc_trace(tf, en_rx_sig, "rx_en");
	sc_trace(tf, read_sig, "read");
	sc_trace(tf, txd_sig, "txd");
	sc_trace(tf, data_out_sig, "data_out");
	sc_trace(tf, data_in_sig, "data_in");
	sc_trace(tf, frame_err_sig, "frame_err");
	sc_trace(tf, output_err_sig, "output_err");
	sc_trace(tf, data_rdy_sig, "data_rdy");

	// Reset
	cout << sc_time_stamp() << ": " << "Reset ..." << endl;
	load_sig.write(false);
	reset_sig.write(true);
	sc_start(1, SC_US);
	reset_sig.write(false);
	sc_start(1, SC_US);

	// Send 0x11 during 10 us
	cout << sc_time_stamp() << ": " << "Load ..." << endl;
	data_in_sig.write(0x11);
	load_sig.write(true);
	read_sig.write(true);
	sc_start(50, SC_NS);
	load_sig.write(false);
	read_sig.write(false);
	sc_start(2000, SC_US);

	data_in_sig.write(0xAA);
	load_sig.write(true);
	sc_start(50, SC_NS);
	load_sig.write(false);
	sc_start(2400, SC_US);

    
	sc_close_vcd_trace_file(tf);
}
