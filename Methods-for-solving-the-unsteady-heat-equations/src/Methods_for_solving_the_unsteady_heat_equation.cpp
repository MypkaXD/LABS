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

	uhe.set_start_data_for_first();
	uhe.print_data();

	SweepMethod sm1;

	sm1.set_N(uhe.get_n()-1);

	for (size_t j = 1; j < uhe.get_m(); ++j) {
		
		sm1.init();

		sm1.set_phi(uhe.get_data(), j);
		sm1.set_ABC(uhe.get_h(), uhe.get_tau());

		sm1.straight_stroke();
		sm1.reverse_stroke();
		
		//sm1.print();
		//
		//std::cout << std::endl;
		//
		//uhe.print_data();
		//
		//std::cout << std::endl;
		//
		//sm1.print_v();
		//
		//std::cout << "=======" << std::endl;

		uhe.set_layer(j, sm1.get_V());
	}

	uhe.print_data();

	std::cout << "====================" << std::endl;

	UHE hw2;
	
	hw2.set_h(0.05);
	hw2.set_L(0, 1);

	hw2.set_tau(0.002);
	hw2.set_T(0, 0.04);

	hw2.set_n((double(1) / 0.05) + 1);
	hw2.set_m((0.04 / 0.002) + 1);

	hw2.resize_data();

	hw2.set_start_data_for_second();
	hw2.print_data();

	SweepMethod sm2;

	sm2.set_N(hw2.get_n()-1);

	for (size_t j = 1; j < hw2.get_m(); ++j) {

		sm2.init();

		sm2.set_phi_for_second(hw2.get_data(), j, 1/2, hw2.get_h(), hw2.get_tau());
		sm2.set_ABC_for_senond(hw2.get_h(), hw2.get_tau(), 1/2);

		sm2.straight_stroke();
		sm2.reverse_stroke();

		hw2.set_layer(j, sm2.get_V());
	}

	hw2.print_data();
	
}