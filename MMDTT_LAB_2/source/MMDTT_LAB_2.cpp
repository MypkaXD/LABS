#include <iostream>
#include <vector>
#include <tuple>

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

	std::vector<std::vector<double>> m_2Vec_Velocity_of_deformation; // скорость деформации (формула 41)
	std::vector<std::vector<double>> m_2Vec_Delta_of_deformation; // приращение деформации (формула из тетради)
	std::vector<std::vector<double>> m_2Vec_Deformation; // деформация (формула из тетради)
	std::vector<std::vector<double>> m_2Vec_Sphere_Component_of_velocity_deformation; // шаровая компонента скорости деформации ??????????????????????????????????? (название)
	std::vector<std::vector<std::tuple<double, double, double>>> m_2Vec_Deviator_velocity_of_deformation; // приращение девиатора деформации

	std::vector<std::vector<double>> m_2Vec_Delta_sigma_of_sphere; // приращение шаровой компоненты напряжений (50 формула)
	std::vector<std::vector<double>> m_2Vec_Sphere_components_velocity_sigma; // шаровая компонента скорости напряжений (46 формула)
	std::vector<std::vector<std::tuple<double, double, double>>> m_2Vec_Deviator_velocity_of_sigma; // девиатор скорости напряжений (47-49 формулы)
	std::vector<std::vector<std::tuple<double, double>>> m_2Vec_Deviator_delta_of_sigma; // девиатор приращения напряжнений
	std::vector<std::vector<double>> m_2Vec_Sphere_component_of_sigma; // шаровая компонента напряжений (53 формула)
	std::vector<std::vector<std::tuple<double, double>>> m_2Vec_Deviator_of_sigma; // девиатор тензора напряжений (54-55 формула)
	std::vector<std::vector<std::tuple<double, double>>> m_2Vec_Sigma; // компоненты напряжения (56-57 формулы)

	std::vector<std::vector<double>> m_2Vec_Vertex_power; // узловые силы (формула 58)
	std::vector<std::vector<double>> m_Step_Time; // шаг по времени (формула 59)
	std::vector<std::vector<double>> m_Ve2_Volume_speed; // скорость звука (формула 59)

	double m_D_Delta_Time_Main; // шаг по времени основной сетки
	double m_D_Delta_Time_Add; // шаг по времени дополнительной сетки

public:

	LAB2() {
	}

	void test_first(const double& left_border, const double& right_border, int count_of_div_coords,
		int count_of_time_moments, const double& start_time_moment, const double& end_time_mometn) { // левый конец жестко закреплен, правый конец двигается

		double step_coords = abs(left_border - right_border) / (2 * count_of_div_coords);
		double step_time_moments = abs(end_time_mometn - start_time_moment) / (count_of_time_moments);

		m_D_Delta_Time_Main = step_time_moments;
		m_D_Delta_Time_Add = abs(end_time_mometn - start_time_moment) / (2 * count_of_time_moments);

		m_I_n = 2 * count_of_div_coords + 1;
		m_I_k = 2 * count_of_time_moments + 1;

		m_2Vec_Velocity_of_moveming.clear();
		m_2Vec_Delta_of_moveming.clear();
		m_2Vec_Geometry.clear();
		m_2Vec_Velocity_of_deformation.clear();
		m_2Vec_Delta_of_deformation.clear();
		m_2Vec_Deformation.clear();
		m_2Vec_Sphere_Component_of_velocity_deformation.clear();
		m_2Vec_Delta_sigma_of_sphere.clear();
		m_2Vec_Sphere_components_velocity_sigma.clear();
		m_2Vec_Deviator_velocity_of_deformation.clear();
		m_2Vec_Deviator_velocity_of_sigma.clear();
		m_2Vec_Sigma.clear();
		m_2Vec_Vertex_power.clear();
		m_2Vec_Deviator_delta_of_sigma.clear();
		m_2Vec_Sphere_component_of_sigma.clear();
		m_2Vec_Deviator_of_sigma.clear();
		m_Step_Time.clear();
		m_Ve2_Volume_speed.clear();

		m_2Vec_Geometry.resize(m_I_n);
		m_2Vec_Delta_of_moveming.resize(m_I_n);
		m_2Vec_Velocity_of_moveming.resize(m_I_n);
		m_2Vec_Velocity_of_deformation.resize(m_I_n);
		m_2Vec_Delta_of_deformation.resize(m_I_n);
		m_2Vec_Deformation.resize(m_I_n);
		m_2Vec_Sphere_Component_of_velocity_deformation.resize(m_I_n);
		m_2Vec_Delta_sigma_of_sphere.resize(m_I_n);
		m_2Vec_Sphere_components_velocity_sigma.resize(m_I_n);
		m_2Vec_Deviator_velocity_of_deformation.resize(m_I_n);
		m_2Vec_Deviator_velocity_of_sigma.resize(m_I_n);
		m_2Vec_Sigma.resize(m_I_n);
		m_2Vec_Vertex_power.resize(m_I_n);
		m_2Vec_Deviator_delta_of_sigma.resize(m_I_n);
		m_2Vec_Sphere_component_of_sigma.resize(m_I_n);
		m_2Vec_Deviator_of_sigma.resize(m_I_n);
		m_Step_Time.resize(m_I_n);
		m_Ve2_Volume_speed.resize(m_I_n);

		for (size_t i = 0; i < m_I_n; ++i) {
			m_2Vec_Geometry[i].resize(m_I_k, 0);
			m_2Vec_Delta_of_moveming[i].resize(m_I_k, 0);
			m_2Vec_Velocity_of_moveming[i].resize(m_I_k, 0);
			m_2Vec_Velocity_of_deformation[i].resize(m_I_k, 0);
			m_2Vec_Delta_of_deformation[i].resize(m_I_k, 0);
			m_2Vec_Deformation[i].resize(m_I_k, 0);
			m_2Vec_Sphere_Component_of_velocity_deformation[i].resize(m_I_k, 0);
			m_2Vec_Delta_sigma_of_sphere[i].resize(m_I_k, 0);
			m_2Vec_Sphere_components_velocity_sigma[i].resize(m_I_k, 0);
			m_2Vec_Deviator_velocity_of_deformation[i].resize(m_I_k, std::make_tuple(0.0, 0.0, 0.0));
			m_2Vec_Deviator_velocity_of_sigma[i].resize(m_I_k, std::make_tuple(0, 0, 0));
			m_2Vec_Sigma[i].resize(m_I_k, std::make_tuple(0, 0));
			m_2Vec_Vertex_power[i].resize(m_I_k, 0);
			m_2Vec_Deviator_delta_of_sigma[i].resize(m_I_k, std::make_tuple(0,0));
			m_2Vec_Sphere_component_of_sigma[i].resize(m_I_k, 0);
			m_2Vec_Deviator_of_sigma[i].resize(m_I_k, std::make_tuple(0, 0));
			m_Step_Time[i].resize(m_I_k, 0);
			m_Ve2_Volume_speed[i].resize(m_I_k, 0);
		}

		for (size_t j = 0; j < m_I_k; ++j) {
			m_2Vec_Velocity_of_moveming[m_I_n - 1][j] = 1;
		}

		for (size_t i = 0; i < m_I_n; ++i) { // по столбцам
			m_2Vec_Geometry[i][0] = left_border + i * step_coords;
		}
	}

	void print_Geometry() {

		std::cout << "Геометрия: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Geometry[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Velocity_of_moveming() {

		std::cout << "Скорость перемещений: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Velocity_of_moveming[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Delta_of_moveming() {

		std::cout << "Приращение перемещение: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Delta_of_moveming[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Velocity_of_deformation() {
		std::cout << "Скорость деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Velocity_of_deformation[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Delta_of_deformation() {
		std::cout << "Приращение деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Delta_of_deformation[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Deformation() {
		std::cout << "Деформация: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Deformation[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_Sphere_Component_of_velocity_deformation() {

		std::cout << "Шаровая компонента скорости деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Sphere_Component_of_velocity_deformation[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void print_deviator_velocity_of_deformation() {
		
		std::cout << "Девиатор скорости деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << "( " << std::get<0>(m_2Vec_Deviator_velocity_of_deformation[i][j]) <<"; " << std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i][j])
					<< "; " << std::get<2>(m_2Vec_Deviator_velocity_of_deformation[i][j]) << ")\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_delta_of_moveming() {

		for (int j = 0; j < m_I_k - 1; j+=2) {
			for (int i = 0; i < m_I_n; i+=2) {
				
				m_2Vec_Delta_of_moveming[i][j + 2] = m_2Vec_Delta_of_moveming[i][j] + m_D_Delta_Time_Main *
					m_2Vec_Velocity_of_moveming[i][j + 1];

				m_2Vec_Delta_of_moveming[i][j + 1] = m_2Vec_Delta_of_moveming[i][j] + (double)1 / 2 *
					m_D_Delta_Time_Main * m_2Vec_Velocity_of_moveming[i][j + 1];

			}
		}
	}

	void calc_geometry() {

		for (int j = 0; j < m_I_k - 1; j += 2) {
			for (int i = 0; i < m_I_n; ++i) {
				m_2Vec_Geometry[i][j + 1] = m_2Vec_Geometry[i][0] + m_2Vec_Delta_of_moveming[i][j + 1];
				m_2Vec_Geometry[i][j + 2] = m_2Vec_Geometry[i][0] + m_2Vec_Delta_of_moveming[i][j + 2];
			}
		}
	}

	void calc_velocity_of_deformation() {

		for (int j = 1; j < m_I_k - 1; j+=2) {
			for (int i = 1; i < m_I_n - 1; i+=2) {

				m_2Vec_Velocity_of_deformation[i][j] = (m_2Vec_Delta_of_moveming[i + 1][j] - m_2Vec_Delta_of_moveming[i-1][j]) /
					(m_2Vec_Geometry[i + 1][j] - m_2Vec_Geometry[i-1][j]);

			}
		}
	}

	void calc_delta_of_deformation(){
		
		std::cout << "Time: " << m_D_Delta_Time_Main << std::endl;
		
		for (int j = 0; j < m_I_k - 1; j += 2) {
			for (int i = 0; i < m_I_n - 1; i += 2) {
				m_2Vec_Delta_of_deformation[i + 2][j + 2] = m_2Vec_Velocity_of_deformation[i+1][j+1] * m_D_Delta_Time_Main;
			}
		}

	}

	void cacl_deformation() {
		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 0; i < m_I_n; i += 2) {
				m_2Vec_Deformation[i][j] = m_2Vec_Deformation[i][j-2] + m_2Vec_Delta_of_deformation[i][j];
			}
		}
	}

	void calc_component_of_velocity_deformation() {

		for (int j = 1; j < m_I_k - 1; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				m_2Vec_Sphere_Component_of_velocity_deformation[i][j] = m_2Vec_Velocity_of_deformation[i][j];
			}
		}
	}

	void calc_deviator_velocity_of_deformation() {
		for (int j = 1; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n; i += 2) {

				std::get<0>(m_2Vec_Deviator_velocity_of_deformation[i][j]) = m_2Vec_Velocity_of_deformation[i][j] -
					(double)1 / 3 * m_2Vec_Sphere_Component_of_velocity_deformation[i][j];
				std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i][j]) = -(double)1 / 3 * m_2Vec_Sphere_Component_of_velocity_deformation[i][j];
				std::get<2>(m_2Vec_Deviator_velocity_of_deformation[i][j]) = std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i][j]);

			}
		}
	}

	void calc_sphere_component_velocity_of_sigma() {

		for (int j = 1; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n; i += 2) {
				m_2Vec_Sphere_components_velocity_sigma[i][j] = -m_D_K * m_2Vec_Sphere_Component_of_velocity_deformation[i][j];
			}
		}

	}

	void print_sphere_components_velocity_of_sigma() {
		std::cout << "print_Sphere_of_sigma: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Sphere_components_velocity_sigma[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void set_bulk_compression_modulus(const double& value) {
		m_D_K = value;
	}
	
	void set_shear_modulus(const double& value) {
		m_D_G = value;
	}

	void calc_deviator_velocity_of_sigma() {

		for (int j = 1; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n; i += 2) {
				std::get<0>(m_2Vec_Deviator_velocity_of_sigma[i][j]) = 2 * m_D_G * std::get<0>(m_2Vec_Deviator_velocity_of_deformation[i][j]);
				std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i][j]) = 2 * m_D_G * std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i][j]);
				std::get<2>(m_2Vec_Deviator_velocity_of_sigma[i][j]) = std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i][j]);
			}
		}

	}

	void print_deviator_velocity_of_sigma() {

		std::cout << "print_Deviator_of_sigma: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << "( " << std::get<0>(m_2Vec_Deviator_velocity_of_sigma[i][j]) << "; " << std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i][j])
					<< "; " << std::get<2>(m_2Vec_Deviator_velocity_of_sigma[i][j]) << ")\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_delta_sphere_components_of_sigma() { // (формула 50)

		for (int j = 1; j < m_I_k - 1; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				m_2Vec_Delta_sigma_of_sphere[i][j] = m_2Vec_Sphere_components_velocity_sigma[i][j] * m_D_Delta_Time_Add;
			}
		}

	}

	void print_delta_sphere_components_of_sigma() {
		std::cout << "Приращение шаровой компоненты напряжений: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Delta_sigma_of_sphere[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_deviator_delta_of_sigma() { // формула 51-52

		for (int j = 1; j < m_I_k - 1; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				std::get<0>(m_2Vec_Deviator_delta_of_sigma[i][j]) = std::get<0>(m_2Vec_Deviator_velocity_of_sigma[i][j])
					* m_D_Delta_Time_Add;
				std::get<1>(m_2Vec_Deviator_delta_of_sigma[i][j]) = std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i][j])
					* m_D_Delta_Time_Add;
			}
		}

	}

	void print_deviator_delta_of_sigma() {

		std::cout << "Деиавтор приращения тензора напряжений: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << "( " << std::get<0>(m_2Vec_Deviator_delta_of_sigma[i][j]) << "; "
					<< std::get<1>(m_2Vec_Deviator_delta_of_sigma[i][j]) << ")\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_sphere_component_of_sigma() {

		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				m_2Vec_Sphere_component_of_sigma[i][j] = m_2Vec_Sphere_component_of_sigma[i][j - 2] +
					m_2Vec_Delta_sigma_of_sphere[i][j - 1];
			}
		}

	}

	void print_sphere_component_of_sigma() {
		std::cout << "Шаровая компонента тензора деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Sphere_component_of_sigma[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_deviator_of_sigma() {

		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				std::get<0>(m_2Vec_Deviator_of_sigma[i][j]) = std::get<0>(m_2Vec_Deviator_of_sigma[i][j - 2]) +
					std::get<0>(m_2Vec_Deviator_delta_of_sigma[i][j - 1]);
				std::get<1>(m_2Vec_Deviator_of_sigma[i][j]) = std::get<1>(m_2Vec_Deviator_of_sigma[i][j - 2]) +
					std::get<1>(m_2Vec_Deviator_delta_of_sigma[i][j - 1]);
			}
		}

	}

	void print_deviator_of_sigma() {
		std::cout << "Девиатор напряжений: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << "( " << std::get<0>(m_2Vec_Deviator_of_sigma[i][j]) << "; "
					<< std::get<1>(m_2Vec_Deviator_of_sigma[i][j]) << ")\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_sigma_components() {

		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				std::get<0>(m_2Vec_Sigma[i][j]) = std::get<0>(m_2Vec_Deviator_of_sigma[i][j]) -
					m_2Vec_Sphere_component_of_sigma[i][j];
				std::get<1>(m_2Vec_Sigma[i][j]) = std::get<1>(m_2Vec_Deviator_of_sigma[i][j]) -
					m_2Vec_Sphere_component_of_sigma[i][j];
			}
		}

	}

	void print_sigma_components() {
		std::cout << "Компоненты напряжения: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << "( " << std::get<0>(m_2Vec_Sigma[i][j]) << "; "
					<< std::get<1>(m_2Vec_Sigma[i][j]) << ")\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_vertex_power() { // ???????????????????????????????????? (вопрос по формуле 58)

		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 2; i < m_I_n - 1; i += 2) {
				m_2Vec_Vertex_power[i][j] = std::get<0>(m_2Vec_Sigma[i - 1][j]) - std::get<0>(m_2Vec_Sigma[i+1][j]);
			}
		}

	}

	void print_vertex_power() {
		std::cout << "Узловые силы: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;

		for (int j = m_I_k - 1; j >= 0; --j) {
			for (int i = 0; i < m_I_n; ++i) {
				std::cout << m_2Vec_Vertex_power[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}

	void calc_volume_speed() {


		for (int j = 2; j < m_I_k; j += 2) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				m_Ve2_Volume_speed[i][j] = sqrt((m_D_K + (double)4/3 * m_D_G) / (1));
			}
		}
	}


	void calc_step_time() {

		for (int j = 4; j < m_I_k; j += 4) {
			for (int i = 1; i < m_I_n - 1; i += 2) {
				//m_Step_Time[i][j] = 
			}
		}

	}

};

int main() {

	setlocale(LC_ALL, "rus");

	LAB2 lab2;

	lab2.test_first(0, 1, 2, 2, 0, 5);

	lab2.print_Geometry();
	lab2.print_Velocity_of_moveming();

	lab2.calc_delta_of_moveming();
	lab2.print_Delta_of_moveming();
	lab2.calc_geometry();
	lab2.print_Geometry();
	lab2.calc_velocity_of_deformation();
	lab2.print_Velocity_of_deformation();
	lab2.calc_delta_of_deformation();
	lab2.print_Delta_of_deformation();
	lab2.cacl_deformation();
	lab2.print_Deformation();
	lab2.calc_component_of_velocity_deformation();
	lab2.print_Sphere_Component_of_velocity_deformation();
	lab2.calc_deviator_velocity_of_deformation();
	lab2.print_deviator_velocity_of_deformation();

	lab2.set_bulk_compression_modulus(5);
	lab2.calc_sphere_component_velocity_of_sigma();
	lab2.print_sphere_components_velocity_of_sigma();

	lab2.set_shear_modulus(10);
	lab2.calc_deviator_velocity_of_sigma();
	lab2.print_deviator_velocity_of_sigma();

	lab2.calc_delta_sphere_components_of_sigma();
	lab2.print_delta_sphere_components_of_sigma();

	lab2.calc_deviator_delta_of_sigma();
	lab2.print_deviator_delta_of_sigma();

	lab2.calc_sphere_component_of_sigma();
	lab2.print_sphere_component_of_sigma();

	lab2.calc_deviator_of_sigma();
	lab2.print_deviator_of_sigma();

	lab2.calc_sigma_components();
	lab2.print_sigma_components();

	lab2.calc_vertex_power();
	lab2.print_vertex_power();

	return 0;
}