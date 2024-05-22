#pragma once

#include <iostream>
#include <vector>
#include <tuple>

class LAB2 {

private:

	int m_I_n; // Количество узлов сетки между граничными узлами подобласти
	int m_I_k;

	int m_I_n_dop;
	int m_I_n_main;

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

	/// /////////////////

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
	std::vector<std::vector<double>> m_2Vec_Volume_speed; // скорость звука (формула 59)
	std::vector<std::vector<double>> m_Vec2_DeltaX; // разность геометрий

	/////////////////


	std::vector<double> m_Vec_Step_Time; // шаг по времени (формула 59)
	std::vector<double> m_Vec_Massa_Cell; // масса ячейки
	std::vector<double> m_Vec_Massa_Node; // масса узла
	std::vector<std::vector<double>> m_2Vec_plotnost;

public:

	LAB2() {
	}

	void test_first(const double& left_border, const double& right_border, int count_of_div_coords,
		int count_of_time_moments, const double& start_time_moment, const double& end_time_mometn) { // левый конец жестко закреплен, правый конец двигается

		double step_coords = abs(left_border - right_border) / (2 * count_of_div_coords);
		double step_time_moments = abs(end_time_mometn - start_time_moment) / (count_of_time_moments);

		//m_D_Delta_Time_Main = step_time_moments;
		//m_D_Delta_Time_Add = abs(end_time_mometn - start_time_moment) / (2 * count_of_time_moments);

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
		m_Vec_Step_Time.clear();
		m_2Vec_Volume_speed.clear();
		m_Vec2_DeltaX.clear();
		m_2Vec_plotnost.clear();

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
		m_Vec_Step_Time.resize(m_I_n);
		m_2Vec_Volume_speed.resize(m_I_n);
		m_Vec2_DeltaX.resize(m_I_n);
		m_2Vec_plotnost.resize(m_I_n);

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
			m_2Vec_Deviator_delta_of_sigma[i].resize(m_I_k, std::make_tuple(0, 0));
			m_2Vec_Sphere_component_of_sigma[i].resize(m_I_k, 0);
			m_2Vec_Deviator_of_sigma[i].resize(m_I_k, std::make_tuple(0, 0));
			//m_Step_Time[i].resize(m_I_k, 0);
			m_2Vec_Volume_speed[i].resize(m_I_k, 0);
			m_Vec2_DeltaX[i].resize(m_I_k, 0);
			m_2Vec_plotnost[i].resize(m_I_k, 0);
		}

		for (size_t j = 0; j < m_I_k; ++j) {
			m_2Vec_Velocity_of_moveming[m_I_n - 1][j] = 1;
		}

		for (size_t i = 0; i < m_I_n; ++i) { // по столбцам
			m_2Vec_Geometry[i][0] = left_border + i * step_coords;
		}
	}

	//void print_Delta_of_deformation() {
	//	std::cout << "Приращение деформации: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;
	//
	//	for (int j = m_I_k - 1; j >= 0; --j) {
	//		for (int i = 0; i < m_I_n; ++i) {
	//			std::cout << m_2Vec_Delta_of_deformation[i][j] << "\t";
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	//void print_Deformation() {
	//	std::cout << "Деформация: size(" << m_I_n << " - stolb; " << m_I_k << " - stroki)" << std::endl;
	//
	//	for (int j = m_I_k - 1; j >= 0; --j) {
	//		for (int i = 0; i < m_I_n; ++i) {
	//			std::cout << m_2Vec_Deformation[i][j] << "\t";
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	//void calc_delta_of_deformation() {
	//
	//	std::cout << "Time: " << m_D_Delta_Time_Main << std::endl;
	//
	//	for (int j = 0; j < m_I_k - 1; j += 2) {
	//		for (int i = 0; i < m_I_n - 1; i += 2) {
	//			m_2Vec_Delta_of_deformation[i + 2][j + 2] = m_2Vec_Velocity_of_deformation[i + 1][j + 1] * m_D_Delta_Time_Main;
	//		}
	//	}
	//
	//}

	//void cacl_deformation() {
	//	for (int j = 2; j < m_I_k; j += 2) {
	//		for (int i = 0; i < m_I_n; i += 2) {
	//			m_2Vec_Deformation[i][j] = m_2Vec_Deformation[i][j - 2] + m_2Vec_Delta_of_deformation[i][j];
	//		}
	//	}
	//}

	void init() {

		m_2Vec_Volume_speed.clear();

		m_Vec_Step_Time.clear();

		m_2Vec_Delta_of_moveming.clear();
		m_2Vec_Delta_of_moveming.resize(m_I_n_main);

		m_2Vec_Velocity_of_deformation.clear();
		m_2Vec_Velocity_of_deformation.resize(m_I_n_dop);

		m_2Vec_Sphere_Component_of_velocity_deformation.clear();
		m_2Vec_Sphere_Component_of_velocity_deformation.resize(m_I_n_dop);

		m_2Vec_Deviator_velocity_of_deformation.clear();
		m_2Vec_Deviator_velocity_of_deformation.resize(m_I_n_dop);

		m_2Vec_Sphere_components_velocity_sigma.clear();
		m_2Vec_Sphere_components_velocity_sigma.resize(m_I_n_dop);

		m_2Vec_Deviator_velocity_of_sigma.clear();
		m_2Vec_Deviator_velocity_of_sigma.resize(m_I_n_dop);

		m_2Vec_Delta_sigma_of_sphere.clear();
		m_2Vec_Delta_sigma_of_sphere.resize(m_I_n_dop);

		m_2Vec_Deviator_delta_of_sigma.clear();
		m_2Vec_Deviator_delta_of_sigma.resize(m_I_n_dop);

		m_2Vec_Sphere_component_of_sigma.clear();
		m_2Vec_Sphere_component_of_sigma.resize(m_I_n_dop);

		m_2Vec_Deviator_of_sigma.clear();
		m_2Vec_Deviator_of_sigma.resize(m_I_n_dop);

		m_2Vec_Sigma.clear();
		m_2Vec_Sigma.resize(m_I_n_dop);
	}

	void main_algorithm() {

		input_borders();
		input_count_x();
		input_plotnost();

		print_plotnost();

		set_start_geometry();
		print_geometry();
		
		calc_massa_cell();
		print_mass_cell();
		
		calc_mass_node();
		print_mass_node();

		input_K();
		input_G();

		init();

		calc_volume_speed();
		print_volume_speed();

		calc_delta_time();
		print_delta_time();

		set_start_vertex_power();
		print_vertex_power();

		set_start_velocity();
		calc_velocity_of_moveming();
		print_velocity_of_moveming();

		calc_delta_of_moveming();
		print_delta_of_moveming();

		calc_geometry();
		print_geometry();

		calc_velocity_of_deformation();
		print_Velocity_of_deformation();

		calc_Sphere_Component_of_velocity_deformation();
		print_Sphere_Component_of_velocity_deformation();

		calc_deviator_velocity_of_deformation();
		print_deviator_velocity_of_deformation();

		calc_sphere_component_velocity_of_sigma();
		print_sphere_components_velocity_of_sigma();

		calc_deviator_velocity_of_sigma();
		print_deviator_velocity_of_sigma();

		calc_delta_sphere_components_of_sigma();
		print_delta_sphere_components_of_sigma();
		
		calc_deviator_delta_of_sigma();
		print_deviator_delta_of_sigma();
		
		calc_sphere_component_of_sigma();
		print_sphere_component_of_sigma();

		calc_deviator_of_sigma();
		print_deviator_of_sigma();

		cacl_sigma_components();
		print_sigma_components();
	}

	void cacl_sigma_components();
	void print_sigma_components();

	void calc_deviator_of_sigma();
	void print_deviator_of_sigma();

	void calc_sphere_component_of_sigma();
	void print_sphere_component_of_sigma();

	void calc_deviator_delta_of_sigma();
	void print_deviator_delta_of_sigma();
	
	void calc_delta_sphere_components_of_sigma();
	void print_delta_sphere_components_of_sigma();

	void calc_deviator_velocity_of_sigma();
	void print_deviator_velocity_of_sigma();

	void calc_sphere_component_velocity_of_sigma();
	void print_sphere_components_velocity_of_sigma();

	void calc_deviator_velocity_of_deformation();
	void print_deviator_velocity_of_deformation();

	void print_Sphere_Component_of_velocity_deformation();
	void calc_Sphere_Component_of_velocity_deformation();

	void calc_velocity_of_deformation();
	void print_Velocity_of_deformation();

	void print_delta_of_moveming();
	void calc_delta_of_moveming();

	void set_start_velocity();
	void calc_velocity_of_moveming();
	void print_velocity_of_moveming();

	void set_start_vertex_power();
	void calc_vertex_power();
	void print_vertex_power();

	// Функция для вычисления шага по времени
	void calc_delta_time();
	void print_delta_time();

	// Функция для вычисления геометрии
	void print_geometry();
	void set_start_geometry();
	void calc_geometry();

	// Функция для вычисления массы ячеек
	void calc_massa_cell();
	void print_mass_cell();

	// Функция для вычисления узловых масс
	void calc_mass_node();
	void print_mass_node();

	// Функции для вычисления сокрости звука
	void calc_volume_speed();
	void print_volume_speed();

	// Функции ввода начальных условий
	void input_borders(); // ввод границ
	void input_count_x(); // ввод числа узлов
	void input_plotnost(); // ввод начальной плотности в узлах
	void input_K(); // ввод модуля объемного сжатия K
	void input_G(); // ввод модуля сдвига G

	// Функции для вывода значений
	void print_plotnost(); // вывод плотности в узлах

};