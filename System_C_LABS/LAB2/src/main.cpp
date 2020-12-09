#include <systemc.h>
#include "txUnit.h"
#include "rxUnit.h"
#include "clkUnit.h"
#include "miniuart.h"
#include "benches.h"

#define TEST_CLK_UNIT 0
#define TEST_TX_UNIT 1
#define TEST_RX_UNIT 2
#define TEST_MINIUART 3

// Change scenario:
#define TEST TEST_MINIUART

int sc_main(int argc, char* argv[]) {
	sc_set_time_resolution(1, SC_NS);

	#if TEST == TEST_CLK_UNIT
		test_clk_unit();
	#elif TEST == TEST_TX_UNIT
		test_tx_unit();
	#elif TEST == TEST_RX_UNIT
		test_rx_unit();
	#elif TEST == TEST_MINIUART
		test_miniuart();
	#endif

	return 0;
}

