#include <iostream>

#include "Construction_of_an_interpolating_cubic_spline.h"
#include "Sweep_method.h"
#include "App.h"

int main() {    

    Construction_of_an_interpolating_cubic_spline cs;
    cs.set_n(10);
    cs.print_h();
    std::cout << std::endl;
    cs.set_phi_for_test();
    cs.print_phi();
    std::cout << std::endl;
    cs.set_c();
    cs.print_c();
    std::cout << std::endl;

    cs.set_a();
    cs.set_b();
    cs.set_d();

    cs.print_a();
    std::cout << std::endl;
    cs.print_b();
    std::cout << std::endl;
    cs.print_c();
    std::cout << std::endl;
    cs.print_d();


    return 0;
}