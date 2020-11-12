#ifndef _USER_H_
#define _USER_H_

#include <systemc.h>

SC_MODULE(user){
    sc_out<double>   period;
    sc_out<bool>     reset;
    sc_out<bool>     up_down;
    sc_out<bool>     load;
    sc_out<int>      data_in;
    
    SC_CTOR(user) : period("periode"), reset("reset"), up_down("up_down"), load("load"), data_in("data_entree"){
        SC_THREAD(user_thread);
    }

    void user_thread();

};

#endif