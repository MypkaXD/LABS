#pragma once

#include <vector>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

class SweepMethod {
private:

	int N = 100; // число шагов (разбиений сетки)

	double m1 = 1; // мю 1
	double m2 = 0; // мю 2

	std::vector<double> A; // вектор коэффов А из 3-х диагональной матрица
	std::vector<double> B; // вектор коэффов B из 3-х диагональной матрица
	std::vector<double> C; // вектор коэффов C из 3-х диагональной матрица

	std::vector<double> alpha; // алфа, который в прогонке
	std::vector<double> betta; // бетта, который в прогонке

	std::vector<double> v; // вектор решения

	std::vector<double> phi; // фи, хрень справа

public:

	void set_N(const int& N) { // установка числа разбиений
		this->N = N;
	}

	int& get_N() { // получение числа разбиений
		return N;
	}

	void init() { // функция, чтобы очистить все вектора от предыдущих данных, выделить размер и изменить шагов
		
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

	void set_phi(const std::vector<std::vector<double>>& data, const int& j) {

		phi[0] = data[j-1][0];
		phi[N] = data[j-1][N];

		m1 = phi[0];
		m2 = phi[N];
	
		for (size_t i = 1; i < N; ++i) {
			phi[i] = data[j-1][i];
		}
	}

	void set_phi_for_second(const std::vector<std::vector<double>>& data, const int& j, const double& lamda, const double& h, const double& tau) {

		phi[0] = data[j-1][0];
		phi[N] = data[j-1][N];

		m1 = phi[0];
		m2 = phi[N];

		for (size_t i = 1; i < N; ++i) {
			phi[i] = data[j - 1][i - 1] * (1 - lamda) * (tau / (h * h)) + (1 - 2 * (1 - lamda) * tau / (h * h)) * data[j - 1][i] + (1 - lamda) * (tau / (h * h)) * data[j - 1][i + 1];
		}
	}

	void set_ABC(const double& h, const double& tau) {

		A[0] = 0; // первая строка трехдиагональной матрицы
		B[0] = 0; // первая строка трехдиагональной матрицы
		C[0] = 1; // первая строка трехдиагональной матрицы

		A[N] = 0; // последняя строка трехдиагональной матрицы
		B[N] = 0; // последняя строка трехдиагональной матрицы
		C[N] = 1; // последняя строка трехдиагональной матрицы

		for (size_t count = 1; count < N; ++count) {
			A[count] = tau / (h * h);
			B[count] = A[count];
			C[count] = (1 + 2 * tau / (h * h));
		}

	}

	void set_ABC_for_senond(const double& h, const double& tau, const double& lamda) {

		A[0] = 0; // первая строка трехдиагональной матрицы
		B[0] = 0; // первая строка трехдиагональной матрицы
		C[0] = 1; // первая строка трехдиагональной матрицы

		A[N] = 0; // последняя строка трехдиагональной матрицы
		B[N] = 0; // последняя строка трехдиагональной матрицы
		C[N] = 1; // последняя строка трехдиагональной матрицы

		for (size_t count = 1; count < N; ++count) {
			A[count] = lamda * tau / (h * h);
			B[count] = A[count];
			C[count] = (1+2*lamda*tau/(h*h));
		}

	}

	void straight_stroke() { // прямой ход прогонки (ВСЕ СДЕЛАНО ПО ЛЕКЦИЯМ)
		// тут просто цикл, ничего сложного
		alpha[1] = 0;
		betta[1] = m1;

		alpha[N] = 0;
		betta[N] = m2;

		for (size_t count = 1; count < N; ++count) {
			alpha[count + 1] = B[count] / (C[count] - A[count] * alpha[count]);
			betta[count + 1] = (phi[count] + A[count] * betta[count]) / (C[count] - A[count] * alpha[count]);
		}
	}

	void reverse_stroke() { // обратный ход прогонки (ТОЖЕ ВСЕ ПО ЛЕКЦИЯМ)
		// обычный цикл, ничего сложного
		v[N] = m2;
		v[0] = m1;

		for (size_t count = N - 1; count > 0; count--) {
			v[count] = alpha[count + 1] * v[count + 1] + betta[count + 1];
		}
	}

	std::vector<double>& get_V() { // получить ССЫЛКУ на вектор решений

		//for (size_t count = 0; count < v.size(); ++count) {
		//	std::cout << v[count] << "\t";
		//}
		//
		//std::cout << "+++++++++++++++++++" << std::endl;

		return v;
	}

	void print_v() {
		
		for (size_t count = 0; count < v.size(); ++count) {
			std::cout << v[count] << "\t";
		}

		std::cout << std::endl;

	}

	void print() {

		for (size_t count = 0; count < N + 1; ++count) {
			std::cout << A[count] << "\t" << C[count] << "\t" << B[count] << "\t=\t" << phi[count] << std::endl;
		}

	}

};