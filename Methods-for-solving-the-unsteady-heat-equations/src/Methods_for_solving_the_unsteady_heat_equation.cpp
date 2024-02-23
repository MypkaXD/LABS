#include <iostream>
#include <vector>

#include "UHE.h"
#include "Sweep_method.h"

int main() {

	UHE uhe;

	uhe.set_h(0.1);
	uhe.set_L(0, 1);

	uhe.set_tau(0.005);
	uhe.set_T(0, 0.2);

	uhe.set_n((double(1) / 0.1) + 1);
	uhe.set_m((0.2 / 0.005) + 1);

	std::cout << "n: " << uhe.get_n() << "\tm: " << uhe.get_m() << std::endl;

	uhe.resize_data();

	uhe.set_start_data();
	uhe.print_data();

	SweepMethod sm;

	sm.set_N(uhe.get_n());

	for (size_t j = 1; j < uhe.get_m(); ++j) {
		sm.init();

		sm.set_phi(uhe.get_data(), j);
		sm.set_ABC(uhe.get_h(), uhe.get_tau());

		sm.straight_stroke();
		sm.reverse_stroke();

		uhe.set_layer(j, sm.get_V());
	}

	uhe.print_data();

	//
}