#ifndef _MINIUART_H_
#define _MINIUART_H_

#include <systemc.h>
#include "clkUnit.h"
#include "txUnit.h"
#include "rxUnit.h"


SC_MODULE(miniuart){
    sc_in<bool>      sysclk;
    sc_in<sc_logic>  reset;
    sc_in<sc_logic>  ce;
    sc_in<sc_logic>  rd;
    sc_in<sc_logic>  wr;
    sc_in<sc_lv<2>>  addr;
    sc_in<sc_lv<8>>  data_in;
    sc_out<sc_lv<8>> data_out;
    sc_out<sc_logic> irqRX;
    sc_out<sc_logic> irqTX;

    sc_out<sc_logic>  txd;
    sc_in<sc_logic>  rxd;

    // interface Process
    sc_signal<sc_logic> load;
    sc_signal<sc_logic> read;
    sc_signal<sc_lv<8>> tx_data;
    sc_signal<sc_lv<8>> rx_data;

    // combinational
    sc_signal<sc_logic> d_rdy;
    sc_signal<sc_logic> o_err;
    sc_signal<sc_logic> f_err;
    sc_signal<sc_logic> buf_empty;
    sc_signal<sc_logic> reg_empty;

    // other signals
    sc_signal<sc_logic> en_tx;
    sc_signal<sc_logic> en_rx;

    // register
    sc_lv<8> ctrl_status_reg;

    // component declaration
    clkUnit clkUnit_inst;
    txUnit txUnit_inst;
    rxUnit rxUnit_inst;


    SC_CTOR(miniuart) : sysclk("sysclk"),
                        reset("reset"),
                        ce("ce"),
                        rd("rd"),
                        wr("wr"),
                        addr("addr"),
                        data_in("data_in"),
                        data_out("data_out"),
                        irqRX("irqRX"),
                        irqTX("irqTX"),
                        txd("txd"),
                        rxd("rxd"),
                        clkUnit_inst("clock_Unit"),
                        txUnit_inst("tx_Unit"),
                        rxUnit_inst("rx_Unit"){
        
        data_out.initialize("ZZZZZZZZ");
		irqRX.initialize(SC_LOGIC_0);
		irqTX.initialize(SC_LOGIC_0);
    
        // liaison des signaux
        // clock unit
        clkUnit_inst.sysclk(sysclk);
        clkUnit_inst.reset(reset);
        clkUnit_inst.en_tx(en_tx);
        clkUnit_inst.en_rx(en_rx);

        // tx unit
        txUnit_inst.sysclk(sysclk);
        txUnit_inst.reset(reset);
        txUnit_inst.enable(en_tx);
        txUnit_inst.load(load);
        txUnit_inst.data_in(tx_data);
        txUnit_inst.txd_send(txd);
        txUnit_inst.reg_empty(reg_empty);
        txUnit_inst.buf_empty(buf_empty);

        // rx unit
        rxUnit_inst.sysclk(sysclk);
        rxUnit_inst.reset(reset);
        rxUnit_inst.enable(en_tx);
        rxUnit_inst.read(read);
        rxUnit_inst.data_out(rx_data);
        rxUnit_inst.rxd_in(rxd);
        rxUnit_inst.frame_err(f_err);
        rxUnit_inst.output_err(o_err);
        rxUnit_inst.data_rdy(d_rdy);

        // interface Process
        SC_METHOD(interface_Process);
        	sensitive << ce << rd << wr << addr << data_in;
			dont_initialize();

        // Combinational
        SC_METHOD(combinational);
        	sensitive << d_rdy << o_err << f_err << buf_empty << reg_empty;
			dont_initialize();
    }

    void interface_Process(){
        // std::cout << "interface_Process triggered" << std::endl;
        if((ce -> read() == true)
        && (wr -> read() == true)
        && (addr -> read() == 0)){
            load.write(SC_LOGIC_1);
            tx_data.write(data_in -> read());
            std::cout << "data in = " << data_in -> read() << std::endl<< std::endl;
        } else {
            load.write(SC_LOGIC_0);
            tx_data.write("00000000");
        }

        if((ce -> read() == true)
        && (rd -> read() == true)){
            if(addr -> read() == 0){
                read.write(SC_LOGIC_1);
                data_out -> write(rx_data.read());
                std::cout << "data out = " << data_out -> read() << std::endl<< std::endl;
            }
            else if(addr -> read() == 1){
                read.write(SC_LOGIC_0);
                data_out -> write(ctrl_status_reg);    
            } else {
                read.write(SC_LOGIC_0);
                data_out -> write("ZZZZZZZZ");
            }
        } else {
            read.write(SC_LOGIC_0);
            data_out -> write("ZZZZZZZZ");
        }
    }

    void combinational(){
        // std::cout << "combinational triggered" << std::endl;

        if((buf_empty.read() == true) && (reg_empty.read() == false)){
            irqTX -> write(SC_LOGIC_1);
        } else {
            irqTX -> write(SC_LOGIC_0);    
        } 
        
        std::cout << "d_rdy = " << d_rdy << std::endl;

        if(d_rdy.read() == true){
            std::cout << "eh ba ptdr" << std::endl << std::endl;
            irqRX -> write(SC_LOGIC_1);
        } else {
            irqRX -> write(SC_LOGIC_0);    
        }
        
        ctrl_status_reg = ((sc_lv<1>)(buf_empty.read()),
                           (sc_lv<1>)(f_err.read()),
                           (sc_lv<1>)(o_err.read()));
    }


};

#endif