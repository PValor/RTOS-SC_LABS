#ifndef _TEST_MEM_H_
#define _TEST_MEM_H_

#include <systemc.h>
#include "memory.h"

SC_MODULE(test_mem){
    sc_clock                        clk_sig;		
    sc_signal<bool>                 enable_sig;
    sc_signal<bool>                 rd_we_sig;
    sc_signal<sc_uint<ADDR_SIZE>>   addr_sig;
    sc_signal<sc_lv<WORD_SIZE>>     data_in_sig;
    sc_signal<sc_lv<WORD_SIZE>>     data_out_sig;
    sc_trace_file                   *tf;
    memory                          *memory_inst;
    

    SC_CTOR(test_mem) : clk_sig("clk", 20, SC_NS, 0.5, 5, SC_NS, true){
        memory_inst = new memory("memory_inst");
		memory_inst -> clk(clk_sig);
		memory_inst -> enable(enable_sig);
		memory_inst -> rd_we(rd_we_sig);
		memory_inst -> addr(addr_sig);
		memory_inst -> data_in(data_in_sig);
		memory_inst -> data_out(data_out_sig);
        
        // Tracage
	    tf = sc_create_vcd_trace_file("wave");
    
	    sc_trace(tf, clk_sig, "clock");
	    sc_trace(tf, enable_sig, "enable");
	    sc_trace(tf, rd_we_sig, "rw");
	    sc_trace(tf, addr_sig, "address");
	    sc_trace(tf, data_in_sig, "data_in");
	    sc_trace(tf, data_out_sig, "data_out");
        
        SC_THREAD(test_mem_thread);
    }

    ~test_mem(){
        delete memory_inst;
    }

    void test_mem_thread(){
        std::cout << "Write 0x2B at addr 0x01" << std::endl;
        // Write
	    enable_sig.write(true);
	    addr_sig.write(0x01);
	    rd_we_sig.write(true);
	    data_in_sig.write("00101011");
        wait(10, SC_MS);

        std::cout << "Write 0x02 at addr 0x02" << std::endl;
	    // Write
	    addr_sig.write(0x02);
	    enable_sig.write(true);
	    rd_we_sig.write(true);
	    data_in_sig.write("00000010");
	    wait(10, SC_MS);

        std::cout << "Read addr 0x01, expecting 0x2B" << std::endl;
	    // Read
	    addr_sig.write(0x01);
	    enable_sig.write(true);
	    rd_we_sig.write(false);
	    wait(10, SC_MS);
 
        sc_close_vcd_trace_file(tf);
    }

};


#endif