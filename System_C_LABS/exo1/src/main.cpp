#include <systemc.h>
#include "user.h"
#include "counter.h"

int sc_main(int argc, char** argv){

    sc_signal<bool>     reset_s, up_down_s, load_s;
    sc_signal<int>      data_in_s, Q_s;
    sc_signal<double>   period_s;

    counter counter_inst("counter_inst");
            counter_inst.period(period_s);
            counter_inst.reset(reset_s);
            counter_inst.up_down(up_down_s);
            counter_inst.load(load_s);
            counter_inst.Q(Q_s);
            counter_inst.data_in(data_in_s);
    
    user user_inst("test_user_inst");
            user_inst.period(period_s);
            user_inst.reset(reset_s);
            user_inst.up_down(up_down_s);
            user_inst.load(load_s);
            user_inst.data_in(data_in_s);
    
    std::cout << "START SIMULATION ..." << std::endl;

	sc_start(400, SC_NS);

	std::cout << sc_time_stamp() << ": STOP SIM";
				

	return 0;
}