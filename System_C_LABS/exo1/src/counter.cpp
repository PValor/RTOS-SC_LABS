#include "counter.h"
#include "user.h"

#include <iostream>

void counter::count_thread(void){
    while(true){
        wait(period -> read(), SC_NS, reset -> posedge_event());

        if(reset -> read() == true) Q -> write(0);
        else
        if(load -> read() == true) Q -> write(data_in -> read());
        else {
            if(up_down -> read() == true)
                Q -> write((Q -> read() + 1)%10);
            else
                Q -> write(Q -> read() == 0 ? 9 : Q -> read() - 1);
        }

        std::cout << sc_time_stamp() << ": Q = " << Q -> read() << std::endl;
    }
}