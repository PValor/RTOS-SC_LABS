#include <systemc.h>
#include "clkUnit.h"
#include "txUnit.h"
#include "rxUnit.h"
#include "miniuart.h"
#include "testbench.h"

void test_miniuart() {
	// Declare signal
	sc_signal<bool> sysclk;
    sc_signal<sc_logic> reset, irqRX, irqTX, txd_rxd;
	sc_signal_resolved ce, rd, wr;
	sc_signal_rv<2> addr;
    sc_signal<sc_lv<8>> data_uart_tb;
    sc_signal<sc_lv<8>> data_tb_uart;


	// Deaclare MiniUart
	miniuart miniuart_inst("MiniUart");
        miniuart_inst.sysclk(sysclk);
        miniuart_inst.reset(reset);
        miniuart_inst.ce(ce);
        miniuart_inst.rd(rd);
        miniuart_inst.wr(wr);
        miniuart_inst.addr(addr);
        miniuart_inst.data_in(data_tb_uart);
        miniuart_inst.data_out(data_uart_tb);
        miniuart_inst.irqTX(irqTX);
        miniuart_inst.irqRX(irqRX);

	// Declare TestBench
	TestBench TestBench_inst("TestBench");
        TestBench_inst.sysclk(sysclk);
        TestBench_inst.reset(reset);
        TestBench_inst.ce(ce);
        TestBench_inst.rd(rd);
        TestBench_inst.wr(wr);
        TestBench_inst.addr(addr);
        TestBench_inst.irqRX(irqRX);
        TestBench_inst.irqTX(irqTX);
        TestBench_inst.data_in(data_uart_tb);
        TestBench_inst.data_out(data_tb_uart);


	// Trace
	sc_trace_file *tf = sc_create_vcd_trace_file("wave_miniuart");
	sc_write_comment(tf, "Simulation of Mini Uart");
	tf->set_time_unit(1, SC_NS);
	
    sc_trace(tf, sysclk, "clock");
	sc_trace(tf, reset, "reset");
	sc_trace(tf, irqRX, "irqRX");
	sc_trace(tf, irqTX, "irqTX");
	sc_trace(tf, ce, "ce");
	sc_trace(tf, rd, "rd");
	sc_trace(tf, wr, "wr");
	sc_trace(tf, data_uart_tb, "data_uart_tb");
	sc_trace(tf, data_tb_uart, "data_tb_uart");
    

	// Run simulation
	sc_start(8, SC_MS);

	// Close Trace
	sc_close_vcd_trace_file(tf);
}
