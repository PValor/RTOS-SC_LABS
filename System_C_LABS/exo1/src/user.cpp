#include "user.h"

void user::user_thread(){
    
    cout << sc_time_stamp() << ": " << "Test du compteur" << endl; 

	period->write(10);	// 10 NS

	data_in->write(3);	// valuer = 3
	reset->write(false);	// reset = 0
	up_down->write(false);
	load->write(false);
	wait(5,SC_NS);

	// Reset
	
	cout << sc_time_stamp() << ": " << "Reset" << endl; 
	reset->write(true);
	wait(15,SC_NS);

	// Decompte
	reset->write(false);
	wait(15, SC_NS);

	// Load
	cout << sc_time_stamp() << ": " << "Load" << endl; 
	load->write(true);
	wait(15,SC_NS);
	load->write(false);
	
	// Decompte
	up_down->write(false);
	cout << sc_time_stamp() << ": " << "Count down" << endl; 
	wait(60,SC_NS);

	// Count
	up_down->write(true);
	cout << sc_time_stamp() << ": " << "Count up" << endl; 
	wait(80,SC_NS);

	// Change period
	period->write(30);	// 30 NS

};