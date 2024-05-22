#include "MMDTT_LAB_2.h"

void LAB2::cacl_sigma_components() {

	for (int i = 0; i < m_I_n_dop; ++i) {

		double xx = std::get<0>(m_2Vec_Deviator_of_sigma[i].back()) - m_2Vec_Sphere_component_of_sigma[i].back();
		double yy = std::get<1>(m_2Vec_Deviator_of_sigma[i].back()) - m_2Vec_Sphere_component_of_sigma[i].back();

		m_2Vec_Sigma[i].push_back(std::make_tuple(xx, yy));

	}

}
void LAB2::print_sigma_components() {
	std::cout << "Компоненты напряжения (по времени в основоной, по координатам в дополнительной):" << std::endl;

	for (int j = m_2Vec_Sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << "( " << std::get<0>(m_2Vec_Sigma[i][j]) << "; "
				<< std::get<1>(m_2Vec_Sigma[i][j]) << ")\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_deviator_of_sigma() {

	for (int i = 0; i < m_I_n_dop; ++i) {

		double prev_xx = m_2Vec_Deviator_of_sigma[i].size() == 0 ? 0 : std::get<0>(m_2Vec_Deviator_of_sigma[i].back());
		double prev_yy = m_2Vec_Deviator_of_sigma[i].size() == 0 ? 0 : std::get<1>(m_2Vec_Deviator_of_sigma[i].back());

		double xx = (prev_xx) +
			std::get<0>(m_2Vec_Deviator_delta_of_sigma[i].back());
		double yy = (prev_yy) +
			std::get<1>(m_2Vec_Deviator_delta_of_sigma[i].back());

		m_2Vec_Deviator_of_sigma[i].push_back(std::make_tuple(xx, yy));
	}
}
void LAB2::print_deviator_of_sigma() {
	std::cout << "Девиатор напряжений (по времени в основоной, по координатам в дополнительной):" << std::endl;

	for (int j = m_2Vec_Deviator_of_sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << "( " << std::get<0>(m_2Vec_Deviator_of_sigma[i][j]) << "; "
				<< std::get<1>(m_2Vec_Deviator_of_sigma[i][j]) << ")\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_sphere_component_of_sigma() {

	for (int i = 0; i < m_I_n_dop; ++i) {

		double prev_value = m_2Vec_Sphere_component_of_sigma[i].size() == 0 ? 0 : m_2Vec_Sphere_component_of_sigma[i].back();

		m_2Vec_Sphere_component_of_sigma[i].push_back(prev_value +
			m_2Vec_Delta_sigma_of_sphere[i].back());
	}
}
void LAB2::print_sphere_component_of_sigma() {
	std::cout << "Шаровая компонента тензора деформации (по времени в основоной, по координатам в дополнительной):" << std::endl;

	for (int j = m_2Vec_Sphere_component_of_sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Sphere_component_of_sigma[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_deviator_delta_of_sigma() { // формула 51-52

	double half_step_time = (m_Vec_Step_Time.back() - (m_Vec_Step_Time.size() == 1 ? 0 : m_Vec_Step_Time[m_Vec_Step_Time.size() - 2])) / 2;

	for (int i = 0; i < m_I_n_dop; ++i) {

		double xx = std::get<0>(m_2Vec_Deviator_velocity_of_sigma[i].back())
			* half_step_time;
		double yy = std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i].back())
			* half_step_time;

		m_2Vec_Deviator_delta_of_sigma[i].push_back(std::make_tuple(xx, yy));
	}

}
void LAB2::print_deviator_delta_of_sigma() {

	std::cout << "Деиавтор приращения тензора напряжений (по времени и по координатам в дополнительной):" << std::endl;

	for (int j = m_2Vec_Deviator_delta_of_sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << "( " << std::get<0>(m_2Vec_Deviator_delta_of_sigma[i][j]) << "; "
				<< std::get<1>(m_2Vec_Deviator_delta_of_sigma[i][j]) << ")\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_delta_sphere_components_of_sigma() { // (формула 50)
	
	double half_step_time = (m_Vec_Step_Time.back() - (m_Vec_Step_Time.size() == 1 ? 0 : m_Vec_Step_Time[m_Vec_Step_Time.size() - 2])) / 2;

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_Delta_sigma_of_sphere[i].push_back(m_2Vec_Sphere_components_velocity_sigma[i].back() * half_step_time);
	}
}
void LAB2::print_delta_sphere_components_of_sigma() {
	std::cout << "Приращение шаровой компоненты напряжений (по времени и по координатам в дополнительной): " << std::endl;

	for (int j = m_2Vec_Delta_sigma_of_sphere[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Delta_sigma_of_sphere[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_deviator_velocity_of_sigma() {

	for (int i = 0; i < m_I_n_dop; ++i) {

		double xx = 2 * m_D_G * std::get<0>(m_2Vec_Deviator_velocity_of_deformation[i].back());
		double yy = 2 * m_D_G * std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i].back());
		double zz = yy;
	
		m_2Vec_Deviator_velocity_of_sigma[i].push_back(std::make_tuple(xx, yy, zz));
	}

}
void LAB2::print_deviator_velocity_of_sigma() {

	std::cout << "Девиатор скорости напряжения (по времени и по координатам в дополнительной): " << std::endl;

	for (int j = m_2Vec_Deviator_velocity_of_sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << "( " << std::get<0>(m_2Vec_Deviator_velocity_of_sigma[i][j]) << "; " << std::get<1>(m_2Vec_Deviator_velocity_of_sigma[i][j])
				<< "; " << std::get<2>(m_2Vec_Deviator_velocity_of_sigma[i][j]) << ")\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_sphere_component_velocity_of_sigma() {

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_Sphere_components_velocity_sigma[i].push_back(- m_D_K * m_2Vec_Sphere_Component_of_velocity_deformation[i].back());
	}
}
void LAB2::print_sphere_components_velocity_of_sigma() {
	std::cout << "Шаровая компонента скорости напряжения (по времени и по координатам в дополнительной) : " << std::endl;

	for (int j = m_2Vec_Sphere_components_velocity_sigma[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Sphere_components_velocity_sigma[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::calc_deviator_velocity_of_deformation() {
	for (int i = 0; i < m_I_n_dop; ++i){

		double xx = m_2Vec_Velocity_of_deformation[i].back() -
			(double)1 / 3 * m_2Vec_Sphere_Component_of_velocity_deformation[i].back();
		double yy = -(double)1 / 3 * m_2Vec_Sphere_Component_of_velocity_deformation[i].back();
		double zz = yy;

		m_2Vec_Deviator_velocity_of_deformation[i].push_back(std::make_tuple(xx, yy, zz));

	}
}
void LAB2::print_deviator_velocity_of_deformation() {

	std::cout << "Девиатор скорости деформации (по времени и по координатам в дополнительной) :" << std::endl;

	for (int j = m_2Vec_Deviator_velocity_of_deformation[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << "( " << std::get<0>(m_2Vec_Deviator_velocity_of_deformation[i][j]) << "; " << std::get<1>(m_2Vec_Deviator_velocity_of_deformation[i][j])
				<< "; " << std::get<2>(m_2Vec_Deviator_velocity_of_deformation[i][j]) << ")\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::print_Sphere_Component_of_velocity_deformation() {

	std::cout << "Шаровая компонента скорости деформации (по времени и по координатам в дополнительной) : " << std::endl;

	for (int j = m_2Vec_Sphere_Component_of_velocity_deformation[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Sphere_Component_of_velocity_deformation[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}
void LAB2::calc_Sphere_Component_of_velocity_deformation() {

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_Sphere_Component_of_velocity_deformation[i].push_back(m_2Vec_Velocity_of_deformation[i].back());
	}

}

void LAB2::calc_velocity_of_deformation() {

	int j_move = m_2Vec_Delta_of_moveming[m_I_n_main - 1].size() - 1;
	int j_geom = m_2Vec_Geometry[m_I_n_main - 1].size() - 1;

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_Velocity_of_deformation[i].push_back((m_2Vec_Delta_of_moveming[i + 1][j_move - 1] - m_2Vec_Delta_of_moveming[i][j_move - 1]) /
			(m_2Vec_Geometry[i + 1][j_geom] - m_2Vec_Geometry[i][j_geom]));
	}

}
void LAB2::print_Velocity_of_deformation() {
	std::cout << "Скорость деформации (по времени и по координатам в дополнительной) :" << std::endl;

	for (int j = m_2Vec_Velocity_of_deformation[m_I_n_dop - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Velocity_of_deformation[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::print_delta_of_moveming() {

	std::cout << "Приращение перемещение (во всех узлах по времени и только в основных по координате): " << std::endl;

	for (int j = m_2Vec_Delta_of_moveming[m_I_n_main - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_main; ++i) {
			std::cout << m_2Vec_Delta_of_moveming[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}
void LAB2::calc_delta_of_moveming() {

	for (int i = 0; i < m_I_n_main; ++i) {
		
		double prev_delta = m_2Vec_Delta_of_moveming[i].size() == 0 ? 0 : m_2Vec_Delta_of_moveming[i].back();

		m_2Vec_Delta_of_moveming[i].push_back(prev_delta + (double)1 / 2 *
			m_Vec_Step_Time.back() * m_2Vec_Velocity_of_moveming[i].back());

		m_2Vec_Delta_of_moveming[i].push_back(prev_delta + m_Vec_Step_Time.back() *
			m_2Vec_Velocity_of_moveming[i].back());

	}
}

void LAB2::set_start_vertex_power() {

	m_2Vec_Vertex_power.clear();
	m_2Vec_Vertex_power.resize(m_I_n_main);

	for (int i = 0; i < m_I_n_main; ++i) {
		m_2Vec_Vertex_power[i].push_back(0);
	}

}
void LAB2::calc_vertex_power() { // ???????????????????????????????????? (вопрос по формуле 58)

	for (int j = 2; j < m_I_k; j += 2) {
		for (int i = 2; i < m_I_n - 1; i += 2) {
			m_2Vec_Vertex_power[i][j] = std::get<0>(m_2Vec_Sigma[i - 1][j]) - std::get<0>(m_2Vec_Sigma[i + 1][j]);
		}
	}

}
void LAB2::print_vertex_power() {
	std::cout << "Узловые силы (по координате в осн узлах) : " << std::endl;

	for (int j = m_2Vec_Vertex_power.back().size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_main; ++i) {
			std::cout << m_2Vec_Vertex_power[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LAB2::set_start_velocity() {

	m_2Vec_Velocity_of_moveming.clear();
	m_2Vec_Velocity_of_moveming.resize(m_I_n_main);

	for (int i = 0; i < m_I_n_main - 1; ++i) {
		m_2Vec_Velocity_of_moveming[i].push_back(0);
	}
	m_2Vec_Velocity_of_moveming[m_I_n_main - 1].push_back(-1);
}
void LAB2::calc_velocity_of_moveming() {

	double half_step_time = (m_Vec_Step_Time.back() - (m_Vec_Step_Time.size() == 1 ? 0 : m_Vec_Step_Time[m_Vec_Step_Time.size() - 2])) / 2;
	
	int j = m_2Vec_Velocity_of_moveming[m_I_n_main - 1].size() - 1 == -1 ? 0 : m_2Vec_Velocity_of_moveming[m_I_n_main - 1].size() - 1;

	for (int i = 0; i < m_I_n_main; ++i) {
		m_2Vec_Velocity_of_moveming[i].push_back(m_2Vec_Velocity_of_moveming[i].size() == 0 ? 0 : 
			m_2Vec_Velocity_of_moveming[i].back() - m_2Vec_Vertex_power[i][j] * m_Vec_Step_Time[j] / 
		m_Vec_Massa_Node[i]);
	}
}
void LAB2::print_velocity_of_moveming() {

	std::cout << "Скорость перемещений ( по координате в осн узлах, по времени в доп узлах) : " << std::endl;

	for (int j = m_2Vec_Velocity_of_moveming.back().size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_main; ++i) {
			std::cout << m_2Vec_Velocity_of_moveming[i][j] << "\t";
		}
		std::cout << std::endl;
	}

}

//
void LAB2::calc_delta_time() {

	std::vector<double> temp_step_time(m_I_n_dop, 0);

	int j = m_2Vec_Geometry[m_I_n_main - 1].size() - 1 == -1 ? 0 : m_2Vec_Geometry[m_I_n_main - 1].size() - 1;

	for (int i = 0; i < m_I_n_dop; ++i) {
		temp_step_time[i] = abs(m_2Vec_Geometry[i + 1][j] - m_2Vec_Geometry[i][j]) /
			m_2Vec_Volume_speed[i][j];
	}

	double babinbubinbubz = *std::max_element(temp_step_time.begin(), temp_step_time.end());

	m_Vec_Step_Time.push_back(babinbubinbubz);

}
void LAB2::print_delta_time() {

	std::cout << "Моменты времени: " << std::endl;

	for (int j = 0; j < m_Vec_Step_Time.size(); j+=2) {
		std::cout << j + 1 << "-ый момент времени: " << m_Vec_Step_Time[j] << std::endl;
	}

}

// Функция для вычисления геометрии
void LAB2::print_geometry() {

	std::cout << "Геометрия (в каждом узле по времени и в основном по кооринате):" << std::endl;

	for (int j = m_2Vec_Geometry[m_I_n_main - 1].size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_main; ++i) {
			std::cout << m_2Vec_Geometry[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}
void LAB2::set_start_geometry() {

	m_2Vec_Geometry.clear();
	m_2Vec_Geometry.resize(m_I_n_main);

	for (int i = 0; i < m_I_n_main; ++i) {
		m_2Vec_Geometry[i].push_back(m_D_left_border + i * abs(m_D_right_border - m_D_left_border) / (m_I_n_main - 1));
	}
}
void LAB2::calc_geometry() {

	int j = m_2Vec_Geometry[m_I_n_main - 1].size() - 1;

	for (int i = 0; i < m_I_n_main; ++i) {

		double prev_geometry = m_2Vec_Geometry[i].back();

		m_2Vec_Geometry[i].push_back(prev_geometry + m_2Vec_Delta_of_moveming[i][j]);
		m_2Vec_Geometry[i].push_back(prev_geometry + m_2Vec_Delta_of_moveming[i][j+1]);
	
	}

}

// Функция для вычисления массы ячеек
void LAB2::calc_massa_cell() { // одинаковые во всех моментах времени

	m_Vec_Massa_Cell.clear();
	m_Vec_Massa_Cell.resize(m_I_n_dop, 0);

	std::cout << "SIZE m_Vec_Massa_Cell: " << m_Vec_Massa_Cell.size() << std::endl;

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_Vec_Massa_Cell[i] = m_2Vec_plotnost[i][0] * 
			(abs(m_2Vec_Geometry[i][m_2Vec_Geometry[i].size() - 1] - m_2Vec_Geometry[i+1][m_2Vec_Geometry[i + 1].size() - 1]));
	}

}
void LAB2::print_mass_cell() {
	std::cout << "Масса ячеек (в дополнительных узлах): " << std::endl;

	for (int i = 0; i < m_I_n_dop; ++i) {
		std::cout << m_Vec_Massa_Cell[i] << "\t";
	}

	std::cout << std::endl;

}

// Функция для вычисления узловых масс
void LAB2::calc_mass_node() { // одинаковые во всех моментах времени

	m_Vec_Massa_Node.clear();
	m_Vec_Massa_Node.resize(m_I_n_main, 0);

	m_Vec_Massa_Node[0] = m_Vec_Massa_Cell[0];
	m_Vec_Massa_Node[m_I_n_main - 1] = m_Vec_Massa_Cell.back();

	for (int i = 1; i < m_I_n_main - 1; ++i) {
		m_Vec_Massa_Node[i] = (m_Vec_Massa_Cell[i-1] + m_Vec_Massa_Cell[i]);
	}

}
void LAB2::print_mass_node() {
	std::cout << "Узловая масса ( в основных узлах): " << std::endl;

	for (int i = 0; i < m_I_n_main; ++i) {
		std::cout << m_Vec_Massa_Node[i] << "\t";
	}

	std::cout << std::endl;
}

// Функции для вычисления сокрости звука
void LAB2::calc_volume_speed() {

	m_2Vec_Volume_speed.resize(m_I_n_dop);

	int j = m_2Vec_Volume_speed.back().size() - 1 == -1 ? 0 : m_2Vec_Volume_speed.back().size() - 1;

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_Volume_speed[i].push_back(sqrt((m_D_K + (double)4 / 3 * m_D_G) / (m_2Vec_plotnost[i][j])));
	}
}
void LAB2::print_volume_speed() {

	std::cout << "Скорость звука ( по координате в доп узлах, по времени в основн узлах):" << std::endl;

	for (int j = m_2Vec_Volume_speed.back().size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_Volume_speed[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

// Функции ввода начальных условий

// ввод границ
void LAB2::input_borders() {

	m_D_left_border = 0;

	std::cout << "Введите длину учаска: ";
	std::cin >> m_D_right_border;

}

// ввод числа узлов
void LAB2::input_count_x() {

	std::cout << "Введите число разбиений по оси Х: ";
	std::cin >> m_I_n;

	m_I_n_main = m_I_n;
	
	m_I_n = 2 * (m_I_n) - 1;

	m_I_n_dop = m_I_n - (m_I_n + 1) / 2;

	std::cout << "Число узловых сеток (основоной и дополнительной вместе): " << m_I_n << std::endl;

}

// ввод начальной плотности в узлах
void LAB2::input_plotnost() {

	double plotnost = 7850;

	//std::cout << "Введите плотность: ";
	//std::cin >> plotnost;

	m_2Vec_plotnost.clear();
	m_2Vec_plotnost.resize(m_I_n_dop);

	for (int i = 0; i < m_I_n_dop; ++i) {
		m_2Vec_plotnost[i].push_back(plotnost);
	}

}

// ввод модуля объемного сжатия K
void LAB2::input_K() {

	//std::cout << "Модуль объёмного сжатия K: ";
	//std::cin >> m_D_K;

	m_D_K = 51e12;
}

// ввод модуля сдвига G
void LAB2::input_G() {

	//std::cout << "Модуль сдвига G: ";
	//std::cin >> m_D_G;

	m_D_G = 97e12;

}

/////////////////////

// Функции для вывода значений

// вывод плотности в узлах
void LAB2::print_plotnost() {

	std::cout << "Плотность (только в дополнительных узлах):" << std::endl;

	for (int j = m_2Vec_plotnost.back().size() - 1; j >= 0; --j) {
		for (int i = 0; i < m_I_n_dop; ++i) {
			std::cout << m_2Vec_plotnost[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}


////////////////////