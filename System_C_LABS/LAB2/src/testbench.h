#ifndef _TESTBENCH_H_
#define _TESTBENCH_H_

#include <systemc.h>

SC_MODULE(TestBench) {
	// Port Declaration
	sc_out<bool> sysclk;
    sc_out<sc_logic> reset;

	sc_out_resolved ce, rd, wr;
	sc_out_rv<2> addr;

	sc_in<sc_lv<8> > data_in;
	sc_out<sc_lv<8> > data_out;

	sc_in<sc_logic> irqRX, irqTX;

	// Channel Members
	sc_clock clock;
	sc_mutex bus_mutex;

	// Data Members
	sc_time CLK_PERIOD;	// 40 MHz
	sc_uint<8> mess;
	unsigned int cpt_car;

	// Constructor
	SC_CTOR(TestBench) : clock("Clock", sc_time(25, SC_NS)) {
		CLK_PERIOD = sc_time(25, SC_NS);

		cpt_car = 0;

		ce.initialize(SC_LOGIC_Z);
		rd.initialize(SC_LOGIC_Z);
		wr.initialize(SC_LOGIC_Z);
		addr.initialize(0);
		data_out.initialize("ZZZZZZZZ");

		SC_METHOD(sysclk_method);
			sensitive << clock.default_event();
		SC_THREAD(main_thread);
		SC_THREAD(tx_isr_thread);
		SC_THREAD(rx_isr_thread);
	}

	// Processes
	void sysclk_method(){
	    sysclk->write(clock.read());
    }

	void main_thread(){
        cout << sc_time_stamp() << ": " << "TestBench Main start " << endl;
        cpt_car = 0;
	    mess = 0xA0;

	    resetTest();
	    wait(CLK_PERIOD * 5);

	    cout << sc_time_stamp() << ": " << "Write Boot " << mess << endl;
	    write(0x00, mess);
    }

	void tx_isr_thread(){
        cout << sc_time_stamp() << ": " << "TestBench tx_thread start " << endl;
        while (true) {
            wait(irqTX->posedge_event());
            if (cpt_car < 5) {
                cpt_car += 1;
                mess = mess+1;
                std::cout << sc_time_stamp() << ": " << "Write " << mess << std::endl;
                write(0x00, mess);
            }
        }
    }

	void rx_isr_thread(){ 
        cout << sc_time_stamp() << ": " << "TestBench rx_thread start " << endl;
        sc_uint<8> value;
        while (true) {
            wait(irqRX->posedge_event());
            cout << "RX Interrupt at time : " << sc_time_stamp() << endl;
            read(0x00, value);
            cout << sc_time_stamp() << ": " << "Read " << char(value) << endl;
        }
    }  

	// Helper Functions
	void resetTest(void){
        bus_mutex.lock();
        ce->write(SC_LOGIC_0);
        wr->write(SC_LOGIC_0);
        rd->write(SC_LOGIC_0);
        addr->write(0);
        data_out->write("ZZZZZZZZ");
        reset->write(SC_LOGIC_1);
        wait(CLK_PERIOD * 20);
        reset->write(SC_LOGIC_0);

        ce->write(SC_LOGIC_Z);
        wr->write(SC_LOGIC_Z);
        rd->write(SC_LOGIC_Z);
    }

	void write(unsigned int addr_, sc_uint<8> data_){
        bus_mutex.lock();
        ce->write(SC_LOGIC_1);
        wr->write(SC_LOGIC_0);
        rd->write(SC_LOGIC_0);
        addr->write(addr_);

        data_out->write("ZZZZZZZZ");

        wait(CLK_PERIOD);

        wr->write(SC_LOGIC_1);
        data_out->write(data_);
        wait(CLK_PERIOD * 3);

        ce->write(SC_LOGIC_0);
        wr->write(SC_LOGIC_0);
        data_out->write("ZZZZZZZZ");
        wait(CLK_PERIOD);

        ce->write(SC_LOGIC_Z);
        wr->write(SC_LOGIC_Z);
        rd->write(SC_LOGIC_Z);
        bus_mutex.unlock();
    }

	void read(unsigned int addr_, sc_uint<8>& data_){
        std::cout << "read method called" << std::endl;
        bus_mutex.lock();
        ce->write(SC_LOGIC_1);
        wr->write(SC_LOGIC_0);
        rd->write(SC_LOGIC_0);
        addr->write(addr_);

        data_out->write("ZZZZZZZZ");

        wait(CLK_PERIOD);

        rd->write(SC_LOGIC_1);
        wait(CLK_PERIOD*2);
        data_= data_in -> read();

        ce->write(SC_LOGIC_0);
        rd->write(SC_LOGIC_0);
        wait(CLK_PERIOD*2);

        ce->write(SC_LOGIC_Z);
        wr->write(SC_LOGIC_Z);
        rd->write(SC_LOGIC_Z);
        bus_mutex.unlock();
    }
};

#endif
