#ifndef _MEMORY_H_
#define _MEMORY_H_

#define ADDR_SIZE 8
#define WORD_SIZE 8

#include <systemc.h>

SC_MODULE(memory){
    sc_in<bool>                 clk;
    sc_in<bool>                 enable;
    sc_in<bool>                 rd_we;
    sc_in<sc_uint<ADDR_SIZE>>   addr;
    sc_in<sc_lv<WORD_SIZE>>     data_in;
    sc_out<sc_lv<WORD_SIZE>>    data_out;
    sc_uint<WORD_SIZE>          store[ADDR_SIZE];

    SC_CTOR(memory) : clk("horloge"), enable("enable"), rd_we("read_write"), addr("adresse"), data_in("data_in"), data_out("data_out"){
        SC_THREAD(memory_thread);
    }

    void memory_thread(){

        std::cout << "starting Memory Thread" << std::endl;

        while(true){
            wait(clk -> negedge_event());
            if(enable -> read()){
                if(rd_we -> read())
                    // write
                    store[addr -> read()] = data_in -> read();
                else
                    // read
                    data_out -> write(store[addr -> read()]);
            }       
        }
    }

};


#endif