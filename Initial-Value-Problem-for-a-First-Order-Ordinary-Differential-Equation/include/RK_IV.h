#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#pragma once

enum class ACTIONS_WITH_X {
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
	// численный шаг интегрирования (начальный)
	double m_h_start = 0.1; 

	// проверка для "выхода" справа
	double m_E_check_right = 0.5 * 10e-6;

	// параметр для контроля локальной пограшености "сверху"
	double m_E_check_up = 0.5 * 10e-4; 
	
	// параметр для контроля локальной погершности "снизу",
	// по умолчанию m_E_check_down = m_E_check_up/(2^(p+1)), где p - порядок метода
	double m_E_check_down = ((m_E_check_up) / 32);

	double m_E_down = 0.5 * 10e-4;

	double m_a = 0; // левая граница
	double m_b = 1; // правая граница

	int m_N_max = 100; // максимальное число шагов
	int p = 4; // порядок метода

	std::vector<std::pair<double, double>> m_data;
	std::vector<std::pair<double, double>> m_u_coords;

	std::vector<double> m_vector_of_h; // веткор для хранения шагов
	std::vector<double> m_twice_half_h_u; // вектор для хранения точек, посчитанных с двойным шагом
	std::vector<double> m_difference_of_v; // вектор разности точки с обычным шагом и двойным
	std::vector<double> m_vector_S; // вектор ОЛП
	std::vector<double> m_u_vector;
	std::vector<double> m_difference_of_u_and_v;
	std::vector<std::pair<double, double>> m_liquid_temperature_coords;

	std::vector<int> m_C1; // вектор кол-ва деления шага
	std::vector<int> m_C2; // вектор кол-ва умножения шага

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
		m_E_check_down = e_check_right / 32;
		//std::cout << "m_E_check_right WAS SETTED" << std::endl;
	}

	void set_e_check_up(const double& e_check_up) {
		if (e_check_up <= 0)
			throw ("ERROR: m_E_check_up cannot be negative or zero");

		m_E_check_up = e_check_up;
		m_E_check_down = e_check_up / 32;
		//std::cout << "m_E_check_up WAS SETTED" << std::endl;
	}

	void set_left_border(const double& a) {
		if (a >= m_b)
			throw ("ERROR: The left border of a cannot be to the right of the right border or equal to it");

		m_a = a;

		std::cout << "m_a WAS SETTED" << std::endl;
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

	void set_e_down(const double& e_down) {
		if (e_down <= 0)
			throw ("ERROR: m_E_down <= 0");
		else
			m_E_down = e_down;
		std::cout << "SETTED E DOWN" << std::endl;
	}

	double func(double x, double u, double a, double T) {
		// a - постоянный положительный коэффициент пропорциональности
		// T - постоянная температура газа или жидкости

		// du/dx = -a * (u-T)

		return (-a * (u - T));
	
		//return (-5 / 2 * u);
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
	double get_E_down() {
		return m_E_down;
	}
	std::pair<double, double> get_border() {
		return { m_a, m_b };
	}

	std::vector<std::pair<double, double>> get_coords() {
		return m_data;
	}

	std::vector<double>& getVectorOfSteps() {
		return m_vector_of_h;
	}

	std::vector<double>& getVectorOfS() {
		return m_vector_S;
	}

	std::vector<int> getVectorOfDivisions() {
		return m_C1;
	}

	std::vector<int> getVectorOfDoublings() {
		return m_C2;
	}

	std::vector<double> getVectorOfCoordsGettingByTwiceHalfStep() {
		return m_twice_half_h_u;
	}

	std::vector<double> getDifferenceOfCoordsWithStepAndWithTwiceHalfStep() {
		return m_difference_of_v;
	}

	std::vector<double>& get_vector_of_u() {
		return m_u_vector;
	}

	std::vector<double>& get_difference_of_u_and_v() {
		return m_difference_of_u_and_v;
	}

	std::vector<std::pair<double, double>>& get_anal_solut() {
		return m_u_coords;
	}

	std::vector<std::pair<double, double>>& get_liquid_temperature_coords() {
		return m_liquid_temperature_coords;
	}

	double RKIV(double x0, double u0, double h, double a, double T) {

		double k1, k2, k3, k4;

		k1 = func(x0, u0, a, T);
		k2 = func(x0 + h / 2, u0 + h / 2 * k1, a, T);
		k3 = func(x0 + h / 2, u0 + h / 2 * k2, a, T);
		k4 = func(x0 + h, u0 + h * k3, a, T);

		return getNewU({ k1,k2,k3,k4 }, u0, h);
	}

	double getNewX(double x, double h) {
		return (x + h);
	}

	double getNewU(std::vector<double> koefs_of_k, double u, double h) {
		if (koefs_of_k.size() != 4)
			throw("ERROR: The vector of K-koefs isn't contain 4 elems");
		return (u + h / 6 * (koefs_of_k[0] + 2 * koefs_of_k[1] + 2 * koefs_of_k[2] + koefs_of_k[3]));
	}

	double getS(double u_with_h, double u_with_twice_half_h) {
		return ((u_with_twice_half_h - u_with_h) / ((2 << (p - 1)) - 1)); // считаем S по формуле S = (v_h/2(n+1) - v_h(n+1)) / (2^p - 1)
	}

	double analytical_func(double t, double u0, double a, double T) {
		return (exp(-a * t) * (u0 - T) + T);
	}

	int getMaxN() {
		return m_N_max;
	}

	int getCountOfDoublings() {
		int counter = 0;

		for (size_t count = 0; count < m_C2.size(); ++count)
			counter += m_C2[count];

		return counter;
	}

	int getCountOfDivisions() {
		int counter = 0;

		for (size_t count = 0; count < m_C1.size(); ++count)
			counter += m_C1[count];

		return counter;
	}

	int getP() {
		return p;
	}

	void clear_data() {
		m_data.clear();
		m_vector_of_h.clear(); 
		m_twice_half_h_u.clear();
		m_difference_of_v.clear(); 
		m_vector_S.clear(); 
		m_u_coords.clear();
		m_u_vector.clear();
		m_difference_of_u_and_v.clear();
		m_liquid_temperature_coords.clear();

		m_C1.clear();
		m_C2.clear();
	}

	void calculating_with_different_h(double x0, double u0, double a, double T, bool isCheckLocalDown, bool isUseEDown, double u_end) {

		m_data.push_back({ x0,u0 });
		m_vector_of_h.push_back(0);
		m_vector_S.push_back(0);
		m_twice_half_h_u.push_back(u0); // пушим в вектор v^ (u0)
		m_difference_of_v.push_back(0); // пушим в вектор разности v - v^ (0)
		m_C2.push_back(0); // пушим в вектор умножений (0)
		m_C1.push_back(0); // пушим в вектор делений (0)
		m_u_vector.push_back(u0);
		m_difference_of_u_and_v.push_back(0);

		double h = m_h_start;

		size_t N = m_N_max;
		
		std::pair<double, double> current_coords;
		std::pair<double, double> coords_with_h;
		std::pair<double, double> coords_with_half_h;
		std::pair<double, double> coords_with_twice_half_h;
		
		double next_x;

		bool wasDivideStep = false;

		int C1counter = 0, C2counter = 0;

		while (N > 0) {
			
			current_coords = m_data.back();

			next_x = getNewX(current_coords.first, h);

			ACTIONS_WITH_X act_for_x = check_next_x_for_right_border(next_x);

			if (act_for_x == ACTIONS_WITH_X::NOTHING || act_for_x == ACTIONS_WITH_X::STOP) {
				if (wasDivideStep)
					coords_with_h = coords_with_half_h;
				else
					coords_with_h = { next_x, RKIV(current_coords.first,current_coords.second,h,a,T) };
				coords_with_half_h = { getNewX(current_coords.first, h / 2), RKIV(current_coords.first, current_coords.second, h/2, a, T) };
				coords_with_twice_half_h = { getNewX(coords_with_half_h.first, h / 2), RKIV(coords_with_half_h.first, coords_with_half_h.second,h/2, a, T) };
			
				ACTIONS_WITH_H	act_for_h = check_local_up_down(coords_with_h.second, coords_with_twice_half_h.second, isCheckLocalDown);

				if (act_for_h == ACTIONS_WITH_H::NOTHING) {

					m_data.push_back(coords_with_h);
					m_vector_of_h.push_back(h);
					m_twice_half_h_u.push_back(coords_with_twice_half_h.second);
					m_difference_of_v.push_back(coords_with_h.second - coords_with_twice_half_h.second);
					m_C1.push_back(C1counter);
					m_C2.push_back(C2counter);
					m_u_vector.push_back(analytical_func(next_x, u0, a, T));
					m_difference_of_u_and_v.push_back(abs(m_u_vector.back() - coords_with_h.second));

					C1counter = C2counter = 0;
					--N;
					wasDivideStep = false;

					if (isUseEDown) {
						if (coords_with_h.second <= u_end + m_E_down && coords_with_h.second >= u_end)
							break;
					}
				}
				else if (act_for_h == ACTIONS_WITH_H::MULTIPLY_BY_2) {

					m_data.push_back(coords_with_h);
					m_vector_of_h.push_back(h);
					m_twice_half_h_u.push_back(coords_with_twice_half_h.second);
					m_difference_of_v.push_back(coords_with_h.second - coords_with_twice_half_h.second);
					m_C1.push_back(C1counter);
					m_C2.push_back(++C2counter);
					m_u_vector.push_back(analytical_func(next_x, u0, a, T));
					m_difference_of_u_and_v.push_back(abs(m_u_vector.back() - coords_with_h.second));

					C1counter = C2counter = 0;
					h *= 2;
					--N;
					wasDivideStep = false;

					if (isUseEDown) {
						if (coords_with_h.second <= u_end + m_E_down && coords_with_h.second >= u_end)
							break;
					}
				}
				else if (act_for_h == ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE) {
					h /= 2;
					++C1counter;
					wasDivideStep = true;
				}

				if (act_for_x == ACTIONS_WITH_X::STOP)
					break;
			}
			else if (act_for_x == ACTIONS_WITH_X::GET_LAST) {
				h = m_b - current_coords.first;
				wasDivideStep = false;
			}
		}
	}

	void calculating_with_const_h(double x0, double u0, double a, double T, double u_end, bool isUseEDown) {
		m_data.push_back({ x0,u0 });
		m_vector_of_h.push_back(0);
		m_vector_S.push_back(0);
		m_twice_half_h_u.push_back(u0);
		m_difference_of_v.push_back(0);
		m_u_vector.push_back(u0);
		m_difference_of_u_and_v.push_back(0);

		double h = (m_b-m_a)/m_N_max;

		size_t N = m_N_max;

		std::pair<double, double> current_coords;
		std::pair<double, double> coords_with_h;
		std::pair<double, double> coords_with_half_h;
		std::pair<double, double> coords_with_twice_half_h;

		double next_x;

		while (N > 0) {

			current_coords = m_data.back();

			next_x = getNewX(current_coords.first, h);

			ACTIONS_WITH_X act_for_x = check_next_x_for_right_border(next_x);

			coords_with_h = { next_x, RKIV(current_coords.first,current_coords.second,h,a,T) };
			coords_with_half_h = { next_x, RKIV(current_coords.first, current_coords.second, h/2, a ,T) };
			coords_with_twice_half_h = { coords_with_half_h.first, RKIV(coords_with_half_h.first, coords_with_half_h.second, h/2, a, T) };


			m_data.push_back(coords_with_h);
			m_vector_of_h.push_back(h);
			m_twice_half_h_u.push_back(coords_with_twice_half_h.second);
			m_difference_of_v.push_back(coords_with_h.second - coords_with_twice_half_h.second);
			m_vector_S.push_back(abs((coords_with_h.second - coords_with_twice_half_h.second) / 15));
			m_u_vector.push_back(analytical_func(next_x, u0, a, T));
			m_difference_of_u_and_v.push_back(abs(m_u_vector.back() - coords_with_h.second));

			--N;
			if (isUseEDown) {

				if (coords_with_h.second <= u_end + m_E_down && coords_with_h.second >= u_end)
					break;
			}

			if (act_for_x == ACTIONS_WITH_X::STOP || act_for_x == ACTIONS_WITH_X::GET_LAST)
				break;
		}
	}

	void analytical_solution(double u0, double a, double T) {
		double h = 0.001;
		double t = m_a;

		while (!(t > m_b)) {
			m_u_coords.push_back({ t, analytical_func(t, u0, a, T) });
			t += h;
		}
	}

	void liquid_temperature_coords(double T) {
		m_liquid_temperature_coords.push_back({ m_a, T });
		m_liquid_temperature_coords.push_back({ m_b, T });
	}

	ACTIONS_WITH_H check_local_up_down(double u_with_h, double u_with_twice_half_h, bool isCheckLocalDown) {
		double S = abs(getS(u_with_h, u_with_twice_half_h)); // получаем S и сразу берем от неё модуль

		if (isCheckLocalDown) {
			if (S >= m_E_check_down && S <= m_E_check_up) { // если S находится в границе "локальной погрешности" то возвращаем статус NOTHING
				m_vector_S.push_back(S*16);
				return ACTIONS_WITH_H::NOTHING;
			}
			else if (S < m_E_check_down) { // если S находится ниже нижней границы, то возвращаем статус MULTIPLY_BY_2
				m_vector_S.push_back(S * 16);
				return ACTIONS_WITH_H::MULTIPLY_BY_2;
			}
			else if (S > m_E_check_up) // если S находится выше верхней границы, то возвращаем статус DIVIDE_BY_2_AND_RECALCULATE
				return ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE;
		}
		else {
			if (S <= m_E_check_up) { // если S находится в границе "локальной погрешности" то возвращаем статус NOTHING
				m_vector_S.push_back(S * 16);
				return ACTIONS_WITH_H::NOTHING;
			}
			else if (S > m_E_check_up) // если S находится выше верхней границы, то возвращаем статус DIVIDE_BY_2_AND_RECALCULATE
				return ACTIONS_WITH_H::DIVIDE_BY_2_AND_RECALCULATE;
		}

	}

	ACTIONS_WITH_X check_next_x_for_right_border(double x) {
		if (x > m_b)
			return ACTIONS_WITH_X::GET_LAST; // если мы попали за правую границу, то нам необходимо вернуться и получить последний элемент
		else if (x == m_b || (x > m_b - m_E_check_right && x < m_b))
			return ACTIONS_WITH_X::STOP; // если мы попали ровно на правую границу, то мы получаем последний элемент и заканчиваем
		else
			return ACTIONS_WITH_X::NOTHING; // если мы попали левее эпсилон - правая-граница, то ничего не делаем и продолжаем работу
	}
};