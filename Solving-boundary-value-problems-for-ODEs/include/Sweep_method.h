#pragma once

#include <vector>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

class SweepMethod {
private:

	int N = 100; // число шагов (разбиений сетки"

	double a = 0; // левая граница
	double b = 1; // правая граница

	double h = 1 / (double)N; // шаг

	double break_point = M_PI / 4; // точка разрыва

	double m1 = 1; // мю 1
	double m2 = 0; // мю 2

	std::vector<double> A; // вектор коэффов А из 3-х диагональной матрица
	std::vector<double> B; // вектор коэффов B из 3-х диагональной матрица
	std::vector<double> C; // вектор коэффов C из 3-х диагональной матрица

	std::vector<double> alpha; // алфа, который в прогонке
	std::vector<double> betta; // бетта, который в прогонке
	
	std::vector<double> v; // вектор решения
	
	std::vector<double> phi; // фи, хрень справа

	std::vector<std::pair<double, double>> analytical_solution; // анал решений

public:
	
	void set_N(const int& N) { // установка числа разбиений
		this->N = N;
	}
 
	int& get_N() { // получение числа разбиений
		return N;
	}
	
	// коэффы для интегралов для тестовой задачи - это предельный значения
	
	double k1_func_test() { // функция k1*
		return sqrt(2)*sin(M_PI/4); // предел от sqrt(2)*sin(x), x->Pi/4
	}

	double k2_func_test() { // функция k2*
		return pow(cos(M_PI/4), 2); // передел от cos^2(x), x->Pi/4
	}

	double q1_func_test() { // функция q1*
		return 1; // предел от 1, x->Pi/4
	}
	
	double q2_func_test() { // функция q2*
		return (pow((M_PI/4),2)); // предел от x^2, x->Pi/4
	}

	double f1_func_test() { // функция f1*
		return sin(2*M_PI/4); // предел от sin(2*x), x->Pi/4
	}

	double f2_func_test() { // функция f2*
		return cos(M_PI/4); // предел от cos(x), x->Pi4
	}
	
	// коэффы для интегралов для основной задачи

	double k1_func_main(const double& x) { // функция k1 
		return(sqrt(2))*sin(x+0.01);
	}

	double k2_func_main(const double& x) { // функция k2
		return (pow(cos(x), 2));
	}

	double q1_func_main(const double& x) { // функция q1
		return 1;
	}

	double q2_func_main(const double& x) { // функция q2
		return (pow(x,2));
	}

	double f1_func_main(const double& x) { // функция f1
		return (sin(2 * x));
	}

	double f2_func_main(const double& x) { // функция f2
		return (cos(x));
	}

	double coef_a_i_for_main(const double& x_plus, const double& x_minus, const double& h) { // расчет коэффа a_i с помощью метода прямоугольников
		if (x_plus <= break_point) { // если верхняя точка границы интеграла меньше точки разрыва
			return k1_func_main((x_plus + x_minus) / 2); // то вызываем k1. Стоит обратить, что тут оптимизация, общая формула используется только для 3-го случвя (ниже)
		}
		else if (x_minus >= break_point) { // если нижняя точка границы интеграла больше точки разрыва
			return k2_func_main((x_plus + x_minus) / 2); // то вызываем k2
		}
		else if (x_minus < break_point && x_plus > break_point) { // если точка разрыва между границами интегрирования, то разбиваем на два интеграла. (см практику)
			return pow(1 / h * ((break_point - x_minus) / k1_func_main((break_point + x_minus) / 2) + (x_plus - break_point) / k2_func_main((x_plus + break_point) / 2)), -1);
		}
	}
	
	// АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
	double coef_d_i_for_main(const double& x_plus, const double& x_minus, const double& h) {
		if (x_plus <= break_point) {
			return q1_func_main((x_plus + x_minus) / 2);
		}
		else if (x_minus >= break_point) {
			return q2_func_main((x_plus + x_minus) / 2);
		}
		else if (x_minus < break_point && x_plus > break_point) {
			return (1 / h * ((break_point - x_minus) * q1_func_main((break_point + x_minus) / 2) + (x_plus - break_point) * q2_func_main((x_plus + break_point) / 2)));
		}
	}
	// АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
	double coef_phi_i_for_main(const double& x_plus, const double& x_minus, const double& h) {
		if (x_plus <= break_point) {
			return f1_func_main((x_plus + x_minus) / 2);
		}
		else if (x_minus >= break_point) {
			return f2_func_main((x_plus + x_minus) / 2);
		}
		else if (x_minus < break_point && x_plus > break_point) {
			return (1 / h * ((break_point - x_minus) * f1_func_main((break_point + x_minus) / 2) + (x_plus - break_point) * f2_func_main((x_plus + break_point) / 2)));
		}
	}
	// АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
	double coef_a_i_for_test(const double& x_plus, const double& x_minus, const double& h) {
		if (x_plus <= break_point) {
			return k1_func_test();
		}
		else if (x_minus >= break_point) {
			return k2_func_test();
		}
		else if (x_minus < break_point && x_plus > break_point) {
			return pow(1 / h * ((break_point - x_minus) / k1_func_test() + (x_plus - break_point) / k2_func_test()), -1);
		}
	}
	// АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
	double coef_d_i_for_test(const double& x_plus, const double& x_minus, const double& h) {
		if (x_plus <= break_point) {
			return q1_func_test();
		}
		else if (x_minus >= break_point) {
			return q2_func_test();
		}
		else if (x_minus < break_point && x_plus > break_point) {
			return (1 / h * ((break_point - x_minus) * q1_func_test() + (x_plus - break_point) * q2_func_test()));
		}
	}
	// АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
	double coef_phi_i_for_test(const double& x_plus, const double& x_minus, const double& h) {
		if (x_plus <= break_point) {
			return f1_func_test();
		}
		else if (x_minus >= break_point) {
			return f2_func_test();
		}
		else if (x_minus < break_point && x_plus > break_point) {
			return (1 / h * ((break_point - x_minus) * f1_func_test() + (x_plus - break_point) * f2_func_test()));
		}
	}

	void init() { // функция, чтобы очистить все вектора от предыдущих данных, выделить размер и изменить шагов
		h = 1 / (double)N; //  установка нового шага	
		std::cout << h << std::endl;
		A.clear(); // очистка коэффов для 3-х диагональной матрицы
		B.clear(); // очистка коэ
		C.clear(); // очистка коэ

		alpha.clear(); // очистка алфа
		betta.clear(); // очистка бетта

		v.clear(); // очистка вектора решения

		phi.clear(); // очистка вектора ФИ

		// Выделение нужного размера (N+1) - это ВАЖНО
		A.resize(N + 1);
		B.resize(N + 1);
		C.resize(N + 1);

		alpha.resize(N + 1);
		betta.resize(N + 1);

		v.resize(N + 1);

		phi.resize(N + 1);
	}

	void set_coefs_for_main() { // установка и инициализация коэффов для основной задачи
		
		init(); // очищаем от предыдущих значений
	
		phi[0] = m1; // фи[0] = m1. ЭТО ЛОГИЧНО, СМОТРИ ЛЕКЦИИ
		phi[N] = m2; // аналогично ЛОГИЧНО

		A[0] = 0; // первая строка трехдиагональной матрицы
		B[0] = 0; // первая строка трехдиагональной матрицы
		C[0] = -1; // первая строка трехдиагональной матрицы

		A[N] = 0; // последняя строка трехдиагональной матрицы
		B[N] = 0; // последняя строка трехдиагональной матрицы
		C[N] = -1; // последняя строка трехдиагональной матрицы

		for (size_t count = 1; count < N; ++count) // установка оставшихся коэффов для правой части матрицы. (от 1 до N-1) ВАЖНО
			phi[count] = coef_phi_i_for_main(a+(count + 1/2)*h, a + (count - 1 / 2) * h, h); // согласно ЛЕКЦИЯМ
		
		for (size_t count = 1; count < N; ++count) { // установка оставшихся коэффов для 3-х диагональной части матрицы. (от 1 до N-1) ВАЖНО
			A[count] = coef_a_i_for_main(a + count * h, a + (count - 1) * h, h) / pow(h, 2); // согласно ЛЕКЦИЯМ
			B[count] = coef_a_i_for_main(a + (count + 1) * h, a + count * h, h) / pow(h, 2); // согласно ЛЕКЦИЯМ
			C[count] = (A[count] + B[count]) + coef_d_i_for_main(a + (count + 1 / 2) * h, a + (count - 1 / 2) * h, h); // согласно ЛЕКЦИЯМ
		}
	}

	void set_coefs_for_test() { // аналогично для основной задачи, только функции вызываются, как для тестовой

		init();

		phi[0] = m1;
		phi[N] = m2;

		A[0] = 0;
		B[0] = 0;
		C[0] = 1;

		A[N] = 0;
		B[N] = 0;
		C[N] = 1;

		for (size_t count = 1; count < N; ++count)
			phi[count] = coef_phi_i_for_test(a + (count + 1 / 2) * h, a + (count - 1 / 2) * h, h);

		for (size_t count = 1; count < N; ++count) {
			A[count] = coef_a_i_for_test(a + count * h, a + (count - 1) * h, h) / pow(h, 2);
			B[count] = coef_a_i_for_test(a + (count + 1) * h, a + count * h, h) / pow(h, 2);
			C[count] = (A[count] + B[count]) + coef_d_i_for_test(a + (count + 1 / 2) * h, a + (count - 1 / 2) * h, h);
		}
	}

	void straight_stroke() { // прямой ход прогонки (ВСЕ СДЕЛАНО ПО ЛЕКЦИЯМ)
	// тут просто цикл, ничего сложного
		alpha[1] = 0; 
		betta[1] = m1;

		alpha[N] = 0;
		betta[N] = m2;

		for (size_t count = 1; count < N; ++count) {
			alpha[count+1] = B[count] / (C[count] - A[count] * alpha[count]);
			betta[count+1] = (phi[count] + A[count] * betta[count]) / (C[count] - A[count] * alpha[count]);
		}
	}

	void reverse_stroke() { // обратный ход прогонки (ТОЖЕ ВСЕ ПО ЛЕКЦИЯМ)
	// обычный цикл, ничего сложного
		v[N] = m2; 
		v[0] = m1;

		for (size_t count = N - 1; count > 0; count--) {
			v[count] = alpha[count+1] * v[count+1] + betta[count+1];
		}
	}

	std::vector<double>& get_V() { // получить ССЫЛКУ на вектор решений
		return v;
	}

	void analytical_solution_init() { // АНАЛ РЕШЕНИЕ

		analytical_solution.clear();
		analytical_solution.resize(N+1);

		double c1 = -0.3393176033;
		double c2 = 0.3393176033;
		double c1_with_header = -0.4920418012;
		double c2_wint_header = 1.05607826;

		for (size_t count = 0; count < analytical_solution.size(); ++count) {
			analytical_solution[count].first = a + count * h;
			if (analytical_solution[count].first < break_point) {
				analytical_solution[count].second = c1 * exp(analytical_solution[count].first) + c2 * exp(analytical_solution[count].first * (-1)) + 1;
			}
			else {
				analytical_solution[count].second = c1_with_header * exp(sqrt(2) * M_PI * analytical_solution[count].first / 4) + c2_wint_header * exp((-1) * sqrt(2) * M_PI * analytical_solution[count].first / 4) + 8 * sqrt(2) / (M_PI * M_PI);
			}
		}
	}

	std::vector<std::pair<double,double>>& get_analytical_solution() { // получить ССЫЛКУ на анал решение
		return analytical_solution;
	}

};