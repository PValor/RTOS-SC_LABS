#ifndef _CLKUNIT_H_
#define _CLKUNIT_H_

#include <systemc.h>

SC_MODULE(clkUnit){
    sc_in<bool>     sysclk;
    sc_in<bool>     reset;
    sc_out<bool>    en_tx;
    sc_out<bool>    en_rx;
    bool temp_en_tx;         
    bool temp_en_rx;         


    SC_CTOR(clkUnit) : sysclk("sysclk"), reset("reset"), en_tx("en_tx"), en_rx("en_rx"){
        SC_THREAD(clkUnit_tx);
        SC_THREAD(clkUnit_rx);
    }


    void clkUnit_tx(){

        std::cout << "starting Clock TX Thread" << std::endl;

        int count = 0;

        while(true){
            wait(sysclk -> posedge_event());
            if(reset -> read() == true){
                count = 0;
                temp_en_tx = false;
                en_tx -> write(temp_en_tx);
            } else {
                if(count < 4166){
                    count++;
                    temp_en_tx = 0;
                } else {
                    count = 0;
                    temp_en_tx = 1;
                }
                en_tx -> write(temp_en_tx);
            }
        }
    }

    void clkUnit_rx(){

        std::cout << "starting Clock RX Thread" << std::endl;

        int count = 0;

        while(true){
            wait(sysclk -> posedge_event());
            if(reset -> read() == true){
                count = 0;
                temp_en_rx = false;
                en_rx -> write(temp_en_rx);
            } else {   
                if(count < 260){
                    count++;
                    temp_en_rx = 0;
                } else {
                    count = 0;
                    temp_en_rx = 1;
                }
                en_rx -> write(temp_en_rx);
            } 
        }
    }
};

#endif