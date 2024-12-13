#include <iostream>
#include <vector>
#include <math.h>

class Lab {
private:

	std::vector<double> u;
	std::vector<double> v;
	std::vector<double> t;

	int N = 100;
	double step = 0.1;

	double b = 1;
	double a = 0;

	double m1 = 0; // мю 1
	double m2 = 0; // мю 2
	double X1 = 0;
	double X2 = 0;


	std::vector<double> A; // вектор коэффов ј из 3-х диагональной матрица
	std::vector<double> B; // вектор коэффов B из 3-х диагональной матрица
	std::vector<double> C; // вектор коэффов C из 3-х диагональной матрица

	std::vector<double> alpha; // алфа, который в прогонке
	std::vector<double> betta; // бетта, который в прогонке

	std::vector<double> phi; // фи, хрень справа

public:

	Lab(int count) {

		this->N = count;

		step = std::abs(b - a) / N;

		u.clear();
		u.resize(N + 1, 0);

		v.clear();
		v.resize(N + 1, 0);

		t.clear();
		t.resize(N + 1, 0);
	}


	double getUPoint(double t) {
		return t * t + t;
	}

	void setUSolve() {
		for (int i = 0; i <= N; ++i) {
			u[i] = getUPoint(a + i * step);
		}
	}

	void setVSolve() {
		A.clear(); // очистка коэффов дл¤ 3-х диагональной матрицы
		B.clear(); // очистка коэ
		C.clear(); // очистка коэ

		alpha.clear(); // очистка алфа
		betta.clear(); // очистка бетта

		v.clear(); // очистка вектора решени¤

		phi.clear(); // очистка вектора ‘»

		// ¬ыделение нужного размера (N+1) - это ¬ј∆Ќќ
		A.resize(N + 1);
		B.resize(N + 1);
		C.resize(N + 1);

		alpha.resize(N + 1);
		betta.resize(N + 1);

		v.resize(N + 1);

		phi.resize(N + 1);

		phi[0] = (1.0 - 0.5 * step * coef_phi_i_for_main(a + 0.5 * step, 0.0, 0.5 * step)) /
			(-coef_a_i_for_main(a + step, a, step) / step + 3 - 0.5 * step * coef_d_i_for_main(a + step / 2, 0, step / 2));
		phi[N] = (k_func(a + step * N) - 0.5 * step * coef_phi_i_for_main(a + step * N, a + step * N - step / 2.0, step / 2.0)) /
			(2 * k_func((a + step * N) * (a + step * N)) - coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step
				- 0.5 * step * coef_d_i_for_main(a + step * N, a + step * ((double)N - 1.0 / 2.0), step / 2));

		m1 = phi[0];
		m2 = phi[N];

		A[0] = 0;
		B[0] = (coef_a_i_for_main(a + step, a, step) / step) /
			(-coef_a_i_for_main(a + step, a, step) / step + 3 - 0.5 * step * coef_d_i_for_main(a + step / 2, 0, step / 2));
		C[0] = 1;

		X1 = -B[0];

		A[N] = (coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step) /
			(2 * k_func((a + step * N) * (a + step * N)) - coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step
				- 0.5 * step * coef_d_i_for_main(a + step * N, a + step * ((double)N - 1.0 / 2.0), step / 2));
		B[N] = 0;
		C[N] = 1;

		X2 = -A[N];

		for (size_t count = 1; count < N; ++count)
			phi[count] = -coef_phi_i_for_main(a + (count + 1.0 / 2.0) * step, a + (count - 1.0 / 2.0) * step, step);

		for (size_t count = 1; count < N; ++count) { // установка оставшихс¤ коэффов дл¤ 3-х диагональной части матрицы. (от 1 до N-1) ¬ј∆Ќќ
			A[count] = coef_a_i_for_main(a + count * step, a + (count - 1) * step, step) / pow(step, 2); // согласно Ћ≈ ÷»яћ
			B[count] = coef_a_i_for_main(a + (count + 1) * step, a + count * step, step) / pow(step, 2); // согласно Ћ≈ ÷»яћ
			C[count] = (A[count] + B[count]) + coef_d_i_for_main(a + (count + 1.0 / 2.0) * step, a + (count - 1.0 / 2.0) * step, step); // согласно Ћ≈ ÷»яћ
		}

		straight_stroke();
		reverse_stroke();
	}

	double k_func(const double& x) {
		double value = exp(x * x);
		return value;
	}

	double q_func(const double& x) {
		double value = (4 - x * x) * exp(x * x);
		return value;
	}

	double f_func(const double& x) {
		double value = exp(x * x) * (-x * x * x * x - x * x * x + 2 * x - 2);
		return value;
	}

	double coef_a_i_for_main(const double& x_plus, const double& x_minus, const double& h) { // расчет коэффа a_i с помощью метода пр¤моугольников
		double value = pow(1.0 / h * ((x_plus - x_minus) / k_func((x_plus + x_minus) / 2)), -1);
		return value;
	}

	// јЌјЋќ√»„Ќќ ѕ–»ћ≈–” ¬џЎ≈
	double coef_d_i_for_main(const double& x_plus, const double& x_minus, const double& h) {
		double value = (1.0 / h * ((x_plus - x_minus) * q_func((x_plus + x_minus) / 2)));
		return value;

	}
	// јЌјЋќ√»„Ќќ ѕ–»ћ≈–” ¬џЎ≈
	double coef_phi_i_for_main(const double& x_plus, const double& x_minus, const double& h) {
		double value = (1.0 / h * ((x_plus - x_minus) * f_func((x_plus + x_minus) / 2)));
		return value;
	}

	void straight_stroke() { // пр¤мой ход прогонки (¬—≈ —ƒ≈ЋјЌќ ѕќ Ћ≈ ÷»яћ)
		// тут просто цикл, ничего сложного
		alpha[1] = X1;
		betta[1] = m1;

		for (int count = 1; count < N; ++count) {
			alpha[count + 1] = B[count] / (C[count] - A[count] * alpha[count]);
			betta[count + 1] = (-phi[count] + A[count] * betta[count]) / (C[count] - A[count] * alpha[count]);
		}
	}

	void reverse_stroke() { // обратный ход прогонки (“ќ∆≈ ¬—≈ ѕќ Ћ≈ ÷»яћ)
		// обычный цикл, ничего сложного
		v[N] = (-X2 * betta[N] - m2) / (X2 * alpha[N] - 1);

		for (int count = N - 1; count >= 0; --count) {
			v[count] = alpha[count + 1] * v[count + 1] + betta[count + 1];
		}
	}

	void setTSolve() {
		for (int i = 0; i <= N; ++i) {
			t[i] = a + i * step;
		}
	}

	void printCoords() {

		for (int i = 0; i <= N; ++i) {
			std::cout << "iter: " << i << "\t" << "t: " << t[i] << "\t" << "v: " << v[i] << "\t" << "u: " << u[i] << std::endl;
		}
	}
};

int main() {

	Lab lab(10);

	lab.setTSolve();
	lab.setUSolve();
	lab.setVSolve();

	lab.printCoords();

	return 0;
}