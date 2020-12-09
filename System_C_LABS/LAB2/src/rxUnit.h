#ifndef _RXUNIT_H_
#define _RXUNIT_H_

#include <systemc.h>

SC_MODULE(rxUnit){
    sc_in<bool>         sysclk;
    sc_in<sc_logic>         reset;
    sc_in<sc_logic>         enable;
    sc_in<sc_logic>         read;
    sc_in<sc_logic>         rxd_in;
    sc_out<sc_lv<8>>  data_out;
    sc_out<sc_logic>        frame_err;
    sc_out<sc_logic>        output_err;
    sc_out<sc_logic>        data_rdy;

    sc_logic temp_frame_err;
    sc_logic temp_output_err;
    sc_logic temp_data_rdy;
    sc_logic rxd;

    sc_logic get_data;

    sc_lv<8> shift_reg;
    sc_lv<8> TBuf;

    int  bitCount;

    SC_CTOR(rxUnit) : sysclk("sysclk"),
                      reset("reset"),
                      enable("enable"),
                      read("read"),
                      rxd_in("rxd"),
                      data_out("data_out"),
                      frame_err("frame_err"),
                      output_err("output_err"),
                      data_rdy("data_rdy"){
        SC_THREAD(rx_Unit_Thread);
    }
    
    void rx_Unit_Thread(){
        // std::cout << "Starting RxUnit Thread" << std::endl;

        int bitCount;
        int clockCount = 0;

        while(true){
            wait(sysclk -> posedge_event());
             if(reset -> read() == true){
                bitCount = 0;
                get_data = false;

                temp_frame_err = false;
                temp_output_err = false;
                temp_data_rdy = false;
                frame_err -> write(temp_frame_err);
                output_err -> write(temp_output_err);
                data_rdy -> write(temp_data_rdy);

                data_out-> write(0x00);
            } else {

                if(read -> read() == true){
                    // std::cout << sc_time_stamp() << " read_received " << std::endl;
                    temp_data_rdy = false;
                    temp_output_err = false;
                    temp_data_rdy = false;
                    frame_err -> write(temp_frame_err);
                    output_err -> write(temp_output_err);
                    data_rdy -> write(temp_data_rdy);
                }

                if(enable -> read() == true){
                    if(clockCount < 15){
                        clockCount++;
                        get_data = (clockCount == 7)? true : false;
                    } else {
                        clockCount = 0;
                        // std::cout << sc_time_stamp() << " RAZ"<< std::endl;
                    }

                if(get_data == true){
                    rxd = rxd_in -> read();
                    // std::cout << sc_time_stamp() << " rxd = " << rxd << std::endl;
                    switch(bitCount){
                        case 0 : 
                            if(rxd == false){
                                std::cout << sc_time_stamp() << " bit start detecte" << std::endl;
                                bitCount++;
                            }
                        break;

                        case  1 ... 8:
                            shift_reg[bitCount-1] = rxd;
                            bitCount++;
                        break;

                        case 9 :
                            if(rxd != true){
                                temp_frame_err = true;
                                frame_err -> write(temp_frame_err);
                            }

                            if(temp_data_rdy == true){
                                temp_output_err = true;
                                output_err -> write(temp_output_err);
                            }
                            temp_data_rdy = true;
                            data_rdy -> write(temp_data_rdy);

                            data_out -> write(shift_reg);
                            bitCount = 0;
                        break;
                    }
                    }
                }
            }
        }
    }

};

#endif