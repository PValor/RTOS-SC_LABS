#include <systemc.h>
#include "test_mem.h"


int sc_main(int argc, char* argv[]) {

	test_mem test_mem_inst("test_mem_inst");

	// Debut Sim
	std::cout << "Start Simulation" << std::endl;
	sc_start(50, SC_MS);

	return 0;
}

