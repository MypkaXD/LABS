#include <iostream>
#include <vector>

class LAB2 {

private:

	/*
		Начальный момент:
			1) скорости
			2) координаты
	
	*/

	int m_I_n; // Количество узлов сетки между граничными узлами подобласти
	int m_I_k;

	double m_D_left_border = 0; // Координаты границ подобластей 
	double m_D_right_border = 0; // Координаты границ подобластей

	int m_I_ЕР = 0; // Параметр физической постановки задачи

	int m_I_GRU = 0; // Тип граничных условий (0 – отсутствует перемещение вдоль оси; 
	// 1 – свободное перемещение вдоль оси; 2 – перемещение со скоростью v(t) вдоль оси;
	// 3 – перемещение вдоль оси под действием поверхностной нагрузки P(t)

	double m_D_K = 0; // Модуль объёмного сжатия (только при EP=0)
	double m_D_G = 0; // Модуль сдвига (только при EP=0)
	double m_D_RO = 0; // Плотность (только при EP=0)

	double m_D_KB = 0; // Коэффициент запаса к временному шагу

	int m_I_N_velocity; // Размерность таблицы v(t) (только при GRU=2)
	std::vector<double> m_Vec_T_velocity; // Таблица v(t) : моменты времени (только при GRU=2)
	std::vector<double> m_Vec_F_velocity; // Таблица v(t) : значения функции (только при GRU=2)

	int m_I_N_power; // Размерность таблицы P(t) (только GRU = 3)
	std::vector<double> m_Vec_T_power; // Таблица P(t) : моменты времени (только при GRU = 3)
	std::vector<double> m_Vec_F_power; // Таблица P(t) : значения функции (только при GRU = 3)

	int m_I_POL = 0; // Тип начальных полей (0 – начальные поля нулевые; 
	// 1 – начальные поля постоянны; 2 – нестандартные начальные поля)

	int m_I_KP; // Количество задаваемых начальных полей (только при POL = 1)
	std::vector<int> m_Vec_NP; // Номера задаваемых начальных полей:
	// 1 – поле скоростей; 2 – поле перемещений; 3 – поле напряжений; 4 – поле деформаций

	std::vector<double> m_Vec_B; // Значения задаваемых полей

	////////////////////////////

	std::vector<std::vector<double>> m_2Vec_Velocity_of_moveming; // скорость перемещений
	std::vector<std::vector<double>> m_2Vec_Delta_of_moveming; // приращение перемещений
	std::vector<std::vector<double>> m_2Vec_Geometry; // геометрия

	std::vector<std::vector<double>> m_2Vec_Velocity_of_deformation; // скорость деформации
	std::vector<std::vector<double>> m_2Vec_Delta_of_deformation; // приращение деформации
	std::vector<std::vector<double>> m_2Vec_Deformation; // деформации

	std::vector<std::vector<double>> m_2Vec_Delta_deformation_of_sphere; // приращение шара деформации
	std::vector<std::vector<double>> m_2Vec_Delta_sigma_of_sphere; // приращение шара напряжений
	std::vector<std::vector<double>> m_2Vec_Sphere_of_sigma; // шар напряжения
	
	std::vector<std::vector<double>> m_2Vec_Delta_deviator_of_deformation; // приращение девиатора деформации
	std::vector<std::vector<double>> m_2Vec_Deviator_of_sigma; // девиатор напряжений
	//std::vector<std::vector<double>> m_2Vec_Sphere_of_sigma; // шар напряжения

	std::vector<std::vector<double>> m_2Vec_Sigma; // напряжения
	std::vector<std::vector<double>> m_2Vec_Vertex_power; // узловые силы
	double m_D_Delta_Time; // шаг по времени

public:

	LAB2() {
	}

	void test_first(const double& left_border, const double& right_border, int count_of_div_coords,
		int count_of_time_moments, const double& start_time_moment, const double& end_time_mometn) { // левый конец жестко закреплен, правый конец двигается

		double step_coords = abs(left_border - right_border) / (2 * count_of_div_coords);
		double step_time_moments = abs(end_time_mometn - start_time_moment) / (2 * count_of_time_moments);

		m_D_Delta_Time = step_time_moments;

		m_I_n = 2 * count_of_div_coords + 1;
		m_I_k = 2 * count_of_time_moments + 1;

		m_2Vec_Velocity_of_moveming.clear();
		m_2Vec_Delta_of_moveming.clear();
		m_2Vec_Geometry.clear();
		m_2Vec_Velocity_of_deformation.clear();
		m_2Vec_Delta_of_deformation.clear();
		m_2Vec_Deformation.clear();
		m_2Vec_Delta_deformation_of_sphere.clear();
		m_2Vec_Delta_sigma_of_sphere.clear();
		m_2Vec_Sphere_of_sigma.clear();
		m_2Vec_Delta_deviator_of_deformation.clear();
		m_2Vec_Deviator_of_sigma.clear();
		m_2Vec_Sigma.clear();
		m_2Vec_Vertex_power.clear();

		m_2Vec_Geometry.resize(m_I_n);
		m_2Vec_Delta_of_moveming.resize(m_I_n);
		m_2Vec_Velocity_of_moveming.resize(m_I_n);
		m_2Vec_Velocity_of_deformation.resize(m_I_n);
		m_2Vec_Delta_of_deformation.resize(m_I_n);
		m_2Vec_Deformation.resize(m_I_n);
		m_2Vec_Delta_deformation_of_sphere.resize(m_I_n);
		m_2Vec_Delta_sigma_of_sphere.resize(m_I_n);
		m_2Vec_Sphere_of_sigma.resize(m_I_n);
		m_2Vec_Delta_deviator_of_deformation.resize(m_I_n);
		m_2Vec_Deviator_of_sigma.resize(m_I_n);
		m_2Vec_Sigma.resize(m_I_n);
		m_2Vec_Vertex_power.resize(m_I_n);


		for (size_t i = 0; i < m_I_n; ++i) {
			m_2Vec_Geometry[i].resize(m_I_k, 0);
			m_2Vec_Delta_of_moveming[i].resize(m_I_k, 0);
			m_2Vec_Velocity_of_moveming[i].resize(m_I_k, 0);
			m_2Vec_Velocity_of_deformation[i].resize(m_I_k, 0);
			m_2Vec_Delta_of_deformation[i].resize(m_I_k, 0);
			m_2Vec_Deformation[i].resize(m_I_k, 0);
			m_2Vec_Delta_deformation_of_sphere[i].resize(m_I_k, 0);
			m_2Vec_Delta_sigma_of_sphere[i].resize(m_I_k, 0);
			m_2Vec_Sphere_of_sigma[i].resize(m_I_k, 0);
			m_2Vec_Delta_deviator_of_deformation[i].resize(m_I_k, 0);
			m_2Vec_Deviator_of_sigma[i].resize(m_I_k, 0);
			m_2Vec_Sigma[i].resize(m_I_k, 0);
			m_2Vec_Vertex_power[i].resize(m_I_k, 0);
		}

		for (size_t j = 0; j < m_I_k; ++j) {
			m_2Vec_Velocity_of_moveming[m_I_n - 1][j] = 1;
		}

		for (size_t i = 0; i < m_I_n; ++i) { // по столбцам
			m_2Vec_Geometry[i][0] = left_border + i * step_coords;
		}
	}

	void print_Geometry() {

		std::cout << "m_2Vec_Geometry: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Geometry[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Velocity_of_moveming() {

		std::cout << "print_Velocity_of_moveming: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Velocity_of_moveming[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Delta_of_moveming() {

		std::cout << "print_Vec_Delta_of_moveming: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Delta_of_moveming[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_delta_of_moveming() {

		for (int j = 0; j < m_I_k; j+=2) {
			for (int i = 0; i < m_I_n; i+=2) {
				m_2Vec_Delta_of_moveming[i][j + 2] = m_2Vec_Delta_of_moveming[i][j] + m_D_Delta_Time *
					m_2Vec_Velocity_of_moveming[i][j + 1];
				m_2Vec_Delta_of_moveming[i][j + 1] = m_2Vec_Delta_of_moveming[i][j] + (double)1 / 2 *
					m_D_Delta_Time * m_2Vec_Velocity_of_moveming[i][j + 1];
			}
		}

	}

};

int main() {

	LAB2 lab2;

	lab2.test_first(0, 1, 10, 10, 0, 5);

	lab2.print_Geometry();
	lab2.print_Velocity_of_moveming();

	lab2.calc_delta_of_moveming();
	lab2.print_Delta_of_moveming();

	return 0;
}