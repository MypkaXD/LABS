#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#pragma once

enum class ACTIONS_WITH_T {
	NOTHING,
	GET_LAST,
	STOP
};

enum class ACTIONS_WITH_H {
	NOTHING, // ничего не делаем
	MULTIPLY_BY_2, // необходимо умножить на два
	DIVIDE_BY_2_AND_RECALCULATE, // делим на два и пересчитываем
};

class RK_IV {
private:
	int m_N_max = 100; // максимальное число шагов
	int p = 4; // порядок метода
	
	// численный шаг интегрирования (начальный)
	double m_h_start = 0.1; 

	// проверка для "выхода" справа
	double m_E_check_right = 0.5 * 10e-6;

	// параметр для контроля локальной пограшености "сверху"
	double m_E_check_up = 0.5 * 10e-4; 
	
	// параметр для контроля локальной погершности "снизу",
	// по умолчанию m_E_check_down = m_E_check_up/(2^(p+1)), где p - порядок метода
	double m_E_check_down = m_E_check_up / pow(2,p+1);

	double m_a = 0; // левая граница
	double m_b = 1; // правая граница


	std::vector<std::pair<double, double>> m_coords_for_first;
	std::vector<std::pair<double, double>> m_coords_for_second;
	std::vector<std::pair<double, double>> m_coords_for_third;

	std::vector<double> m_coords_with_twice_step_for_first;
	std::vector<double> m_coords_with_twice_step_for_second;
	std::vector<double> m_coords_with_twice_step_for_third;

	std::vector<double> m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first;
	std::vector<double> m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second;
	std::vector<double> m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third;

	std::vector<double> m_h_vector;
	std::vector<double> m_S_vector;

	std::vector<int> m_C1;
	std::vector<int> m_C2;

public:


	void set_h_start(const double& h) {
		if (h <= 0)
			throw ("ERROR: The step cannot be negative or zero");

		m_h_start = h;
	
		//std::cout << "m_h_start WAS SETTED" << std::endl;
	}

	void set_e_check_right(const double& e_check_right) {
		if (e_check_right <= 0)
			throw ("ERROR: m_E_check_right cannot be negative or zero");

		m_E_check_right = e_check_right;

		//std::cout << "m_E_check_right WAS SETTED" << std::endl;
	}

	void set_e_check_up(const double& e_check_up) {
		if (e_check_up <= 0)
			throw ("ERROR: m_E_check_up cannot be negative or zero");

		m_E_check_up = e_check_up;
		m_E_check_down = e_check_up / pow(2, p + 1);

		//std::cout << "m_E_check_up WAS SETTED" << std::endl;
	}

	void set_left_border(const double& a) {
		if (a >= m_b)
			throw ("ERROR: The left border of a cannot be to the right of the right border or equal to it");

		m_a = a;

		//std::cout << "m_a WAS SETTED" << std::endl;
	}

	void set_right_border(const double& b) {
		if (b <= m_a)
			throw ("ERROR: The right border of a cannot be to the left of or equal to the left border");

		m_b = b;

		//std::cout << "m_b WAS SETTED" << std::endl;
	}

	void set_count_of_max_N(const int& N) {
		if (N <= 0)
			throw ("ERROR: N <= 0");

		m_N_max = N;

		//std::cout << "m_N_max WAS SETTED" << std::endl;
	}

	void clear_data() {

		m_coords_for_first.clear();
		m_coords_for_second.clear();
		m_coords_for_third.clear();

		m_coords_with_twice_step_for_first.clear();
		m_coords_with_twice_step_for_second.clear();
		m_coords_with_twice_step_for_third.clear();

		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.clear();
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.clear();
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.clear();

		m_h_vector.clear();
		m_S_vector.clear();

		m_C1.clear();
		m_C2.clear();
	}

	double func_for_first(double t, double u1, double u2, double u3, double a1, double w1, double b1) {
		return (u1 * (a1 - u1 - w1 * u2 - b1 * u3));
	}

	double func_for_second(double t, double u1, double u2, double u3, double a2, double w2, double b2) {
		return (u2 * (a2 - w2 * u1 - u2 - b2 * u3));
	}

	double func_for_third(double t, double u1, double u2, double u3, double g1, double g2) {
		return (u3 * (-1 + g1 * u1 + g2 * u2));
	}

	double get_h_start() {
		//std::cout << "GETTING m_h_start" << std::endl;

		return m_h_start;
	}
	double get_E_check_up() {
		//std::cout << "GETTING m_E_check_up" << std::endl;

		return m_E_check_up;
	}
	double get_E_check_down() {
		return m_E_check_down;
	}
	double get_E_check_right() {
		//std::cout << "GETTING m_E_check_right" << std::endl;

		return m_E_check_right;
	}
	std::pair<double, double> get_border() {
		return { m_a, m_b };
	}

	std::vector<std::pair<double, double>>& get_coords_for_first() {
		return m_coords_for_first;
	}

	std::vector<std::pair<double, double>>& get_coords_for_second() {
		return m_coords_for_second;
	}

	std::vector<std::pair<double, double>>& get_coords_for_third() {
		return m_coords_for_third;
	}

	std::vector<double>& get_vector_of_h() {
		return m_h_vector;
	}

	std::vector<double>& get_coords_with_twice_step_for_first() {
		return m_coords_with_twice_step_for_first;
	}

	std::vector<double>& get_coords_with_twice_step_for_second() {
		return m_coords_with_twice_step_for_second;
	}

	std::vector<double>& get_coords_with_twice_step_for_third() {
		return m_coords_with_twice_step_for_third;
	}

	std::vector<double>& get_vector_of_S() {
		return m_S_vector;
	}

	std::vector<double>& get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first() {
		return m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first;
	}

	std::vector<double>& get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second() {
		return m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second;
	}

	std::vector<double>& get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third() {
		return m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third;
	}

	std::vector<int>& get_vector_of_C1() {
		return m_C1;
	}

	std::vector<int>& get_vector_of_C2() {
		return m_C2;
	}

	int getP() {
		return p;
	}

	std::vector<std::pair<double, double>> RKIV(double t, double u1, double u2, double u3, double h, double a1, double a2, double w1, double w2, double b1, double b2, double g1, double g2) {

		double k1, k2, k3, k4;
		double l1, l2, l3, l4;
		double m1, m2, m3, m4;

		k1 = func_for_first(t, u1, u2, u3, a1, w1, b1);
		l1 = func_for_second(t, u1, u2, u3, a2, w2, b2);
		m1 = func_for_third(t, u1, u2, u3, g1, g2);



		k2 = func_for_first(t + h / 2, u1 + k1 * (h / 2), u2 + l1 * (h / 2), u3 + m1 * (h / 2) , a1, w1, b1);
		l2 = func_for_second(t + h / 2, u1 + k1 * (h / 2), u2 + l1 * (h / 2), u3 + m1 * (h / 2) , a2, w2, b2);
		m2 = func_for_third(t + h / 2, u1 + k1 * (h / 2), u2 + l1 * (h / 2), u3 + m1 * (h / 2) , g1, g2);




		k3 = func_for_first(t + h / 2, u1 + k2 * (h / 2), u2 + l2 * (h / 2), u3 + m2 * (h / 2) , a1, w1, b1);
		l3 = func_for_second(t + h / 2, u1 + k2 * (h / 2), u2 + l2 * (h / 2), u3 + m2 * (h / 2) , a2, w2, b2);
		m3 = func_for_third(t + h / 2, u1 + k2 * (h / 2), u2 + l2 * (h / 2), u3 + m2 * (h / 2) , g1, g2);


		k4 = func_for_first(t + h, u1 + k3 * h, u2 + l3 * h, u3 + m3 * h , a1, w1, b1);
		l4 = func_for_second(t + h, u1 + k3 * h, u2 + l3 * h, u3 + m3 * h , a2, w2, b2);
		m4 = func_for_third(t + h, u1 + k3 * h, u2 + l3 * h, u3 + m3 * h , g1, g2);

		return { 
			{getNewT(t,h), getNewU({k1,k2,k3,k4}, u1, h)}, 
			{getNewT(t,h), getNewU({l1,l2,l3,l4}, u2, h)}, 
			{getNewT(t,h), getNewU({m1,m2,m3,m4}, u3, h)}
		};
	}

	double getNewT(double t, double h) {
		return (t + h);
	}

	double getNewU(std::vector<double> koefs, double u, double h) {
		if (koefs.size() != 4)
			throw("ERROR: The vector of koefs isn't contain 4 elems");
		return (u + h / 6 * (koefs[0] + 2 * koefs[1] + 2 * koefs[2] + koefs[3]));
	}

	double getS(double u_with_h, double u_with_twice_half_h) {
		return ((u_with_twice_half_h - u_with_h) / (pow(2,p) - 1)); // считаем S по формуле S = (v_h/2(n+1) - v_h(n+1)) / (2^p - 1)
	}

	int getMaxN() {
		return m_N_max;
	}

	void calculating_with_different_h(double t0, std::vector<double> u0, double a1, double a2, double w1, double w2, double b1, double b2, double g1, double g2, bool isCheckLocalDown) {

		m_coords_for_first.push_back({ t0, u0[0] }); // вектор с координатами (tn,u1n), полученные с шагом h
		m_coords_for_second.push_back({ t0,u0[1] }); // вектор с координатами (tn,u2n), полученные с шагом h
		m_coords_for_third.push_back({ t0, u0[2] }); // вектор с координатами (tn,u3n), полученные с шагом h

		m_coords_with_twice_step_for_first.push_back(u0[0]); // вектор координат, полученные с шагом h/2
		m_coords_with_twice_step_for_second.push_back(u0[1]); // вектор координат, полученные с шагом h/2
		m_coords_with_twice_step_for_third.push_back(u0[2]); // вектор координат, полученные с шагом h/2

		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.push_back(0); // вектор разности с двойным шагом и одиночным
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.push_back(0); // вектор разности с двойным шагом и одиночным
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.push_back(0); // вектор разности с двойным шагом и одиночным

		m_S_vector.push_back(0); // вектор ОЛП
		m_h_vector.push_back(0); // вектор шагов

		m_C1.push_back(0); // вектор делений шага
		m_C2.push_back(0); // вектор умножений шага

		double h = m_h_start; // текущий шаг

		double current_t;
		
		double next_t; // следующая точка t

		int C1 = 0; // число делений шага на текущем шаге
		int C2 = 0; // число умножений шага на текущем шаге

		size_t N = m_N_max; // макисмальное число шагов
		
		bool isRecalculating = false; // булева переменная для оптимизации
		
		std::vector<double> current_coords; // вектор текущих координат
		current_coords.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_h; // вектор координат следующей точки, полученной с шагом h
		coords_with_h.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_half_h; // вектор координат промежуточной точки, полученной с шагом h/2
		coords_with_half_h.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_twice_half_h; // вектор координат следующей точки, полученной с двойным шагом h/2
		coords_with_twice_half_h.resize(u0.size());
		
		while (N > 0) { // пока допустимое число шагов > 0
			
			current_coords[0] = m_coords_for_first.back().second; // получаем текущие координаты 
			current_coords[1] = m_coords_for_second.back().second; // получаем текущие координаты
			current_coords[2] = m_coords_for_third.back().second; // получаем текущие координаты

			current_t = m_coords_for_first.back().first;

			next_t = getNewT(current_t, h); // получаем следующий t

			ACTIONS_WITH_T act_for_t = check_next_t_for_right_border(next_t); // проверяем, находится ли t в области интегрирования 

			if (act_for_t == ACTIONS_WITH_T::NOTHING || act_for_t == ACTIONS_WITH_T::STOP) { // если находится
				
				coords_with_h = RKIV(current_t, current_coords[0], current_coords[1], current_coords[2], h, a1, a2, w1, w2, b1, b2, g1, g2);
				coords_with_half_h = RKIV(current_t, current_coords[0], current_coords[1], current_coords[2], h / 2, a1, a2, w1, w2, b1, b2, g1, g2);
				coords_with_twice_half_h = RKIV(coords_with_half_h[0].first, coords_with_half_h[0].second, coords_with_half_h[1].second, coords_with_half_h[2].second , h / 2, a1, a2, w1, w2, b1, b2, g1, g2);

				if (coords_with_h[0].second < 0 || coords_with_h[1].second < 0 || coords_with_h[2].second < 0)
					break;

				ACTIONS_WITH_H	act_for_h = check_local_up_down({coords_with_h[0].second, coords_with_h[1].second, coords_with_h[2].second}, { coords_with_twice_half_h[0].second, coords_with_twice_half_h[1].second, coords_with_twice_half_h[2].second}, isCheckLocalDown);

				if (act_for_h == ACTIONS_WITH_H::NOTHING) {

					m_coords_for_first.push_back(coords_with_h[0]);
					m_coords_for_second.push_back(coords_with_h[1]);
					m_coords_for_third.push_back(coords_with_h[2]);

					m_coords_with_twice_step_for_first.push_back(coords_with_twice_half_h[0].second);
					m_coords_with_twice_step_for_second.push_back(coords_with_twice_half_h[1].second);
					m_coords_with_twice_step_for_third.push_back(coords_with_twice_half_h[2].second);

					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.push_back(abs(coords_with_h[0].second - coords_with_twice_half_h[0].second));
					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.push_back(abs(coords_with_h[1].second - coords_with_twice_half_h[1].second));
					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.push_back(abs(coords_with_h[2].second - coords_with_twice_half_h[2].second));

					--N;

					m_C1.push_back(C1);
					m_C2.push_back(C2);

					m_h_vector.push_back(h);

					C1 = C2 = 0;

					isRecalculating = false;
				}
				else if (act_for_h == ACTIONS_WITH_H::MULTIPLY_BY_2) {

					m_coords_for_first.push_back(coords_with_h[0]);
					m_coords_for_second.push_back(coords_with_h[1]);
					m_coords_for_third.push_back(coords_with_h[2]);

					m_coords_with_twice_step_for_first.push_back(coords_with_twice_half_h[0].second);
					m_coords_with_twice_step_for_second.push_back(coords_with_twice_half_h[1].second);
					m_coords_with_twice_step_for_third.push_back(coords_with_twice_half_h[2].second);

					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.push_back(abs(coords_with_h[0].second - coords_with_twice_half_h[0].second));
					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.push_back(abs(coords_with_h[1].second - coords_with_twice_half_h[1].second));
					m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.push_back(abs(coords_with_h[2].second - coords_with_twice_half_h[2].second));
					
					m_h_vector.push_back(h);

					h *= 2;
					--N;

					m_C1.push_back(C1);
					m_C2.push_back(++C2);

					C1 = C2 = 0;

					isRecalculating = false;
				}
				else if (act_for_h == ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE) {
					h /= 2;
					++C1;

					isRecalculating = true;
				}

				if (act_for_t == ACTIONS_WITH_T::STOP)
					break;
			}
			else if (act_for_t == ACTIONS_WITH_T::GET_LAST) {
				h = m_b - current_t;

				isRecalculating = false;
			}
		}
	}

	void calculating_with_const_h(double t0, std::vector<double> u0, double a1, double a2, double w1, double w2, double b1, double b2, double g1, double g2, bool& isCheckLocalDown) {

		m_coords_for_first.push_back({ t0, u0[0] });
		m_coords_for_second.push_back({ t0,u0[1] });
		m_coords_for_third.push_back({ t0, u0[2] });

		m_coords_with_twice_step_for_first.push_back(u0[0]);
		m_coords_with_twice_step_for_second.push_back(u0[1]);
		m_coords_with_twice_step_for_third.push_back(u0[2]);

		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.push_back(0);
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.push_back(0);
		m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.push_back(0);

		m_S_vector.push_back(0);
		m_h_vector.push_back(0);

		double h = (m_b-m_a)/m_N_max;

		size_t N = m_N_max;

		std::vector<std::pair<double, double>> current_coords; // вектор текущих координат
		current_coords.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_h; // вектор координат следующей точки, полученной с шагом h
		coords_with_h.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_half_h; // вектор координат промежуточной точки, полученной с шагом h/2
		coords_with_half_h.resize(u0.size());

		std::vector<std::pair<double, double>> coords_with_twice_half_h; // вектор координат следующей точки, полученной с двойным шагом h/2
		coords_with_twice_half_h.resize(u0.size());

		double next_t;

		while (N > 0) {

			current_coords[0] = m_coords_for_first.back();
			current_coords[1] = m_coords_for_second.back();
			current_coords[2] = m_coords_for_third.back();

			next_t = getNewT(current_coords[0].first, h);

			ACTIONS_WITH_T act_for_t = check_next_t_for_right_border(next_t);

			coords_with_h = RKIV(current_coords[0].first, current_coords[0].second, current_coords[1].second, current_coords[2].second , h, a1, a2, w1, w2, b1, b2, g1, g2);
			coords_with_half_h = RKIV(current_coords[0].first, current_coords[0].second, current_coords[1].second, current_coords[2].second , h / 2, a1, a2, w1, w2, b1, b2, g1, g2);
			coords_with_twice_half_h = RKIV(coords_with_half_h[0].first, coords_with_half_h[0].second, coords_with_half_h[1].second, coords_with_half_h[2].second , h / 2, a1, a2, w1, w2, b1, b2, g1, g2);

			if (coords_with_h[0].second < 0 || coords_with_h[1].second < 0 || coords_with_h[2].second < 0)
				break;

			//if (std::isnan(coords_with_h[0].second) || std::isnan(coords_with_h[1].second) || std::isnan(coords_with_h[2].second) ||
			//	std::isnan(coords_with_twice_half_h[0].second) || std::isnan(coords_with_twice_half_h[1].second) || std::isnan(coords_with_twice_half_h[2].second))
			//	break;

			ACTIONS_WITH_H	act_for_h = check_local_up_down({ coords_with_h[0].second, coords_with_h[1].second, coords_with_h[2].second }, { coords_with_twice_half_h[0].second, coords_with_twice_half_h[1].second, coords_with_twice_half_h[2].second }, isCheckLocalDown);

			m_coords_for_first.push_back(coords_with_h[0]);
			m_coords_for_second.push_back(coords_with_h[1]);
			m_coords_for_third.push_back(coords_with_h[2]);

			m_coords_with_twice_step_for_first.push_back(coords_with_twice_half_h[0].second);
			m_coords_with_twice_step_for_second.push_back(coords_with_twice_half_h[1].second);
			m_coords_with_twice_step_for_third.push_back(coords_with_twice_half_h[2].second);

			m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.push_back(coords_with_h[0].second - coords_with_twice_half_h[0].second);
			m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.push_back(coords_with_h[1].second - coords_with_twice_half_h[1].second);
			m_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.push_back(coords_with_h[2].second - coords_with_twice_half_h[2].second);

			m_h_vector.push_back(h);

			--N;

			if (act_for_t == ACTIONS_WITH_T::STOP || act_for_t == ACTIONS_WITH_T::GET_LAST)
				break;
		}
	}

	ACTIONS_WITH_H check_local_up_down(std::vector<double> u_with_h, std::vector<double> u_with_twice_half_h, bool& isCheckLocalDown) {

		double S1 = std::abs(getS(u_with_h[0], u_with_twice_half_h[0]));
		double S2 = std::abs(getS(u_with_h[1], u_with_twice_half_h[1]));
		double S3 = std::abs(getS(u_with_h[2], u_with_twice_half_h[2]));

		double S = std::max({ S1, S2, S3 });

		if (isCheckLocalDown) {
			if (S >= m_E_check_down && S <= m_E_check_up) {
				double OLP1 = S1 * pow(2, p);
				double OLP2 = S2 * pow(2, p);
				double OLP3 = S3 * pow(2, p);

				double OLP = std::max({ OLP1, OLP2, OLP3 });

				m_S_vector.push_back(OLP);
				return ACTIONS_WITH_H::NOTHING;
			}
			else if (S < m_E_check_down) { // если S находится ниже нижней границы, то возвращаем статус MULTIPLY_BY_2
				double OLP1 = S1 * pow(2, p);
				double OLP2 = S2 * pow(2, p);
				double OLP3 = S3 * pow(2, p);

				double OLP = std::max({ OLP1, OLP2, OLP3 });

				m_S_vector.push_back(OLP);
				return ACTIONS_WITH_H::MULTIPLY_BY_2;
			}
			else if (S > m_E_check_up) // если S находится выше верхней границы, то возвращаем статус DIVIDE_BY_2_AND_RECALCULATE
				return ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE;
		}
		else {
			if (S <= m_E_check_up) { // если S находится в границе "локальной погрешности" то возвращаем статус NOTHING
				double OLP1 = S1 * pow(2, p);
				double OLP2 = S2 * pow(2, p);
				double OLP3 = S3 * pow(2, p);

				double OLP = std::max({ OLP1, OLP2, OLP3 });

				m_S_vector.push_back(OLP);
				return ACTIONS_WITH_H::NOTHING;
			}
			else if (S > m_E_check_up) // если S находится выше верхней границы, то возвращаем статус DIVIDE_BY_2_AND_RECALCULATE
				return ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE;
		}
	}

	ACTIONS_WITH_T check_next_t_for_right_border(double t) {
		if (t > m_b)
			return ACTIONS_WITH_T::GET_LAST; // если мы попали за правую границу, то нам необходимо вернуться и получить последний элемент
		else if (t == m_b || (t > m_b - m_E_check_right && t < m_b))
			return ACTIONS_WITH_T::STOP; // если мы попали ровно на правую границу, то мы получаем последний элемент и заканчиваем
		else
			return ACTIONS_WITH_T::NOTHING; // если мы попали левее эпсилон - правая-граница, то ничего не делаем и продолжаем работу
	}
};