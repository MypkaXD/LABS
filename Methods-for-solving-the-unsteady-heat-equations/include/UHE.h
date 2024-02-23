#include <iostream>
#include <vector>

#pragma once

#define _USE_MATH_DEFINES // for C
#include <math.h>

class UHE {

private:

	double h; // шаг для разбиения отрезка [0,L]
	double tau; // шаг для разбиения отрезка [0, T]

	int n; // кол-во разбиений отрезка [0,L]
	int m; // кол-во разбиений отрезка [0, T]

	double L_left;
	double L_right;

	double T_down;
	double T_up;

	std::vector<std::vector<double>> data;

	// i - по x
	// j - по t

public:

	UHE() {}

	int& get_n() { // получение числа разбиений
		return n;
	}

	int& get_m() {
		return m;
	}

	double& get_h() {
		return h;
	}

	double& get_tau() {
		return tau;
	}

	std::vector<std::vector<double>>& get_data() {
		return data;
	}

	void set_n(const double& n) {
		this->n = n;
	}

	void set_m(const double& m) {
		this->m = m;
	}

	void set_L(const double& L_left, const double& L_right) {
		this->L_left = L_left;
		this->L_right = L_right;
	}

	void set_T(const double& T_down, const double& T_up) {
		this->T_down = T_down;
		this->T_up = T_up;
	}

	void calc_h() {
		h = abs(L_right - L_left) / n;
	}

	void calc_t() {
		tau = abs(T_up - T_down) / m;
	}

	void set_h(const double& h) {
		this->h = h;
	}

	void set_tau(const double& tau) {
		this->tau = tau;
	}

	void resize_data() {

		data.resize(m);

		for (size_t j = 0; j < m; ++j) {
			data[j].resize(n);
		}

		std::cout << "SIZE OF DATA: " << data.size() << std::endl;
		for (size_t j = 0; j < data.size(); ++j) {
			std::cout << "SIZE OF DATA OF " << j << ": " << data[j].size() << std::endl;
		}
	}

	void set_start_data_for_first() {

		for (size_t j = 0; j < m; ++j) {
			data[j][0] = 0;
			data[j][n - 1] = 0;
		}

		for (size_t i = 0; i < n; ++i) {
			if (i != n - 1)
				data[0][i] = sin(M_PI * i * h);
			else
				data[0][i] = 0;
		}
	}

	void set_start_data_for_second() {

		for (size_t j = 0; j < m; ++j) {
			data[j][0] = 1;
			data[j][n - 1] = 0;
		}

		for (size_t i = 0; i < n; ++i) {
			data[0][i] = 1-i*h-1/M_PI*(sin(2*M_PI*i*h));
		}
	}

	void print_data() {

		std::cout << "m: " << m << "\tn: " << n << std::endl;

		for (int j = m - 1; j >= 0; --j) {
			for (size_t i = 0; i < n; ++i) {
				std::cout << data[j][i] << " ";
			}
			std::cout << std::endl;
		}
	}


	void set_layer(const int& j, std::vector<double>& layer) {

		for (size_t i = 0; i < n; ++i) {
			data[j][i] = layer[i];
		}
	}

};