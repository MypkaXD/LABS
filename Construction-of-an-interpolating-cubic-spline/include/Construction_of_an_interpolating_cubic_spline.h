#include <vector>

#include "Sweep_method.h"

#pragma once

class Construction_of_an_interpolating_cubic_spline {

private:

	int n = 10;
	double h = 0.2;

	double a_border = -1;
	double b_border = 1;

	std::vector<double> phi;

	std::vector<double> a;
	std::vector<double> b;
	std::vector<double> c;
	std::vector<double> d;

public:

	Construction_of_an_interpolating_cubic_spline() {}

	void set_border(const double& a_border, const double& b_border) {
		this->a_border = a_border;
		this->b_border = b_border;
	}

	std::pair<double, double> get_border() {
		return { this->a_border, this->b_border };
	}

	double& get_left_border() {
		return this->a_border;
	}

	double& get_right_border() {
		return this->b_border;
	}

	double get_f_i_for_test(const double& x) {
		if (x >= -1 && x <= 0)
			return (x * x * x + 3 * x * x);
		else if (x <= 1 && x > 0)
			return (-x * x * x + 3 * x * x);
	}

	double get_f_i_for_first_task(const double& x) {
		return (double)log(x + 1) / x;
	}

	double get_f_i_for_second_task(const double& x) {
		return (double)log(x + 1) / x + cos(10*x);
	}

	int& get_n() {
		return this->n;
	}

	void set_n(const int& n) {
		this->n = n;
	}

	void set_h() {
		h = abs(b_border - a_border) / n;
	}

	void set_phi_for_test() {

		phi.clear();
		phi.resize(n+1);

		double m1 = 0;
		double m2 = 0;

		phi[0] = m1;
		phi[n] = m2;

		for (size_t count = 1; count < n; ++count) {
			
			double f_prev = get_f_i_for_test(a_border + (count - 1) * h);
			double f_current = get_f_i_for_test(a_border + count * h);
			double f_next = get_f_i_for_test(a_border + (count + 1) * h);

			//std::cout << "f_prev: " << f_prev << std::endl;
			//std::cout << "f_current: " << f_current << std::endl;
			//std::cout << "f_next: " << f_next << std::endl;

			phi[count] = 6 * ((f_next - f_current) / h - (f_current - f_prev) / h);
		}
	}

	void set_phi_for_first_task() {

		phi.clear();
		phi.resize(n + 1);

		double m1 = 0;
		double m2 = 0;

		phi[0] = m1;
		phi[n] = m2;

		for (size_t count = 1; count < n; ++count) {

			double f_prev = get_f_i_for_first_task(a_border + (count - 1) * h);
			double f_current = get_f_i_for_first_task(a_border + count * h);
			double f_next = get_f_i_for_first_task(a_border + (count + 1) * h);

			phi[count] = 6 * ((f_next - f_current) / h - (f_current - f_prev) / h);
		}
	}

	void set_phi_for_second_task() {

		phi.clear();
		phi.resize(n + 1);

		double m1 = 0;
		double m2 = 0;

		phi[0] = m1;
		phi[n] = m2;

		for (size_t count = 1; count < n; ++count) {

			double f_prev = get_f_i_for_second_task(a_border + (count - 1) * h);
			double f_current = get_f_i_for_second_task(a_border + count * h);
			double f_next = get_f_i_for_second_task(a_border + (count + 1) * h);

			phi[count] = 6 * ((f_next - f_current) / h - (f_current - f_prev) / h);
		}
	}

	void print_phi() {

		for (size_t count = 0; count < phi.size(); ++count)
			std::cout << phi[count] << std::endl;

	}

	void print_h() {
		std::cout << "H: " << h << std::endl;
	}

	void set_c() {

		c.clear();
		c.resize(n + 1);

		SweepMethod sm;
		sm.set_N(this->n);
		sm.init();
		sm.set_phi(this->phi);
		//std::cout << "set_c H: " << this->h << std::endl;
		sm.set_ABC(this->h, this->h);
		sm.straight_stroke();
		sm.reverse_stroke();

		c = sm.get_V();

		//sm.print();
		//sm.print_v();
		//
		//std::cout << "C:::" << std::endl;
		//
		//print_c();
		//
		//std::cout << "=============" << std::endl;

	}

	void set_a(const int& index) {

		a.clear();
		a.resize(n);

		for (size_t count = 0; count < n; ++count) {
			if (index == 0)
				a[count] = get_f_i_for_test(a_border + (count + 1) * h);
			else if (index == 1)
				a[count] = get_f_i_for_first_task(a_border + (count + 1) * h);
			else if (index == 2)
				a[count] = get_f_i_for_second_task(a_border + (count + 1) * h);
		}
	}

	void set_d() {

		d.clear();
		d.resize(n);

		for (size_t count = 0; count < n; ++count)
			d[count] = (c[count + 1] - c[count]) / h;
	}

	void set_b(const int& index) {

		b.clear(); 
		b.resize(n);

		for (size_t count = 0; count < n; ++count) {
			
			//std::cout << "h in spline: " << h << std::endl;

			if (index == 0)
				b[count] = (get_f_i_for_test(a_border + (count + 1) * h) - get_f_i_for_test(a_border + count * h)) / h +
				c[count + 1] * h / 3 + c[count] * h / 6;
			else if (index == 1)
				b[count] = (get_f_i_for_first_task(a_border + (count + 1) * h) - get_f_i_for_first_task(a_border + count * h)) / h +
				c[count + 1] * h / 3 + c[count] * h / 6;
			else if (index == 2)
				b[count] = (get_f_i_for_second_task(a_border + (count + 1) * h) - get_f_i_for_second_task(a_border + count * h)) / h +
				c[count + 1] * h / 3 + c[count] * h / 6;
		}

	}

	void print_a() {
		for (size_t count = 0; count < a.size(); ++count)
			std::cout << a[count] << std::endl;
	}

	void print_b() {
		for (size_t count = 0; count < b.size(); ++count)
			std::cout << b[count] << std::endl;
	}

	void print_c() {
		for (size_t count = 0; count < c.size(); ++count)
			std::cout << c[count] << std::endl;
	}

	void print_d() {
		for (size_t count = 0; count < d.size(); ++count)
			std::cout << d[count] << std::endl;
	}

	std::vector<double>& get_a() {
		return a;
	}

	std::vector<double>& get_b() {
		return b;
	}

	std::vector<double>& get_c() {
		return c;
	}

	std::vector<double>& get_d() {
		return d;
	}

};