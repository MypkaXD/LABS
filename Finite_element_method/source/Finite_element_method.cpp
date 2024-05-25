#include <iostream>
#include <vector>

class LR_1 {
private:

	std::vector<double> vd_sigma;

	int i_n;

	double d_left_border;
	double d_right_border;

public:

	LR_1() {

	}

	LR_1(int& i_size) :
		i_n(i_size) {
		vd_sigma.resize(i_n);
	}

	void set_sigma_const(const double& value) {

		for (size_t count = 0; count < i_n; ++count) {
			vd_sigma[count] = value;
		}

	}

	void set_sigma_square(const double& coef) {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		for (size_t count = 0; count < i_n; ++count) {
			vd_sigma[count] = coef * std::pow((d_left_border + count * step), 2);
		}

	}

	void set_sigma_kube(const double& coef) {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		for (size_t count = 0; count < i_n; ++count) {
			vd_sigma[count] = coef * std::pow((d_left_border + count * step), 3);
		}

	}

	void set_sigma_quadro(const double& coef) {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		for (size_t count = 0; count < i_n; ++count) {
			vd_sigma[count] = coef * std::pow((d_left_border + count * step), 4);
		}

	}

	void set_sigma_liner(const double& coef) {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		for (size_t count = 0; count < i_n; ++count) {
			vd_sigma[count] = coef * (d_left_border + count * step) + 3;
		}

	}

	void set_n(const int& size) {
		
		if (size <= 0)
			throw ("Error: size can't be less then zero or equal zero");
		else {
			i_n = size;

			vd_sigma.clear();
			vd_sigma.resize(i_n);
		}

	}

	void set_border(const double& left_border, const double& right_border) {

		if (left_border >= right_border)
			throw ("Left border can't be less or eual then right border");
		else {
			d_left_border = left_border;
			d_right_border = right_border;
		}

	}

	double trapezoid_method() {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		double result = vd_sigma[0] / 2;

		for (size_t count = 1; count < i_n - 1; ++count) {
			result += vd_sigma[count];
		}

		result += vd_sigma[i_n - 1] / 2;

		result *= step;

		return result;
	}

	double simpson_method() {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		double result = vd_sigma[0] + vd_sigma[i_n - 1];

		for (size_t count = 1; count < i_n - 1; ++count) {
			if (count % 2 == 0)
				result += 2 * vd_sigma[count];
			else
				result += 4 * vd_sigma[count];
		}

		result *= step / 3;

		return result;
	}

	double newton() {

		double step = abs(d_right_border - d_left_border) / (i_n - 1);

		double result = vd_sigma[0] + vd_sigma[i_n - 1];

		for (size_t count = 1; count < i_n - 1; ++count) {
			result += vd_sigma[count] * 3;
		}

		result *= 3 * step / 8;

		return result;

	}

	void print_sigma() {

		for (size_t count = 0; count < i_n; ++count) {
			std::cout << "sigma[" << count << "] = " << vd_sigma[count] << std::endl;
		}

	}

	double gauss_method_3(int k) {

		double result = 0;

		std::vector<double> c_k(3);
		std::vector<double> root(3);

		root[0] = 0;
		root[1] = sqrt((double)3 / 5);
		root[2] = - sqrt((double)3 / 5);

		for (int i = 0; i < 3; ++i) {
			c_k[i] = 2 * (1 - pow(root[i], 2)) / (pow(3, 2) *
				pow((double)1/2*(3*pow(root[i], 2) - 1), 2));
		}

		std::vector<double> x_coords(3);
		
		for (int i = 0; i < 3; ++i) {
			x_coords[i] = (d_right_border + d_left_border) / 2 +
				(d_right_border - d_left_border) / 2 * root[i];
			result += c_k[i] * pow(x_coords[i], k);
		}

		return result * (d_right_border - d_left_border) / 2;
		
	}

	double gauss_method_5(int k) {

		double result = 0;

		std::vector<double> c_k(5);
		std::vector<double> root(5);
		
		root[0] = 0;
		root[1] = sqrt((70+sqrt(1120))/126);
		root[2] = sqrt((70-sqrt(1120))/126);
		root[3] = -sqrt((70+sqrt(1120))/126);
		root[4] = -sqrt((70-sqrt(1120))/126);

		for (int i = 0; i < 5; ++i) {
			c_k[i] = 2 * (1 - pow(root[i], 2)) / ( pow(5, 2) *
				pow((double)1 / 8 * (35 * pow(root[i], 4) - 30 * pow(root[i], 2) + 3), 2));
		}

		for (int count = 0; count < 5; ++count) {
			std::cout << "c_" << count << ": " << c_k[count] << std::endl;
			std::cout << "root_" << count << ": " << root[count] << std::endl;
		}

		std::vector<double> x_coords(5);

		for (int i = 0; i < 5; ++i) {
			x_coords[i] = (d_right_border + d_left_border) / 2 +
				(d_right_border - d_left_border) / 2 * root[i];
			result += c_k[i] * pow(x_coords[i], k);
		}

		return result * (d_right_border - d_left_border) / 2;

	}


};

int main() {

	LR_1 lr;

	int i_n;

	std::cout << "Enter size: ";
	std::cin >> i_n;

	lr.set_n(i_n);

	double d_left_border;
	double d_right_border;

	std::cout << "Enter left_border: ";
	std::cin >> d_left_border;

	std::cout << "Enter right_border: ";
	std::cin >> d_right_border;

	lr.set_border(d_left_border, d_right_border);

	//lr.set_sigma_square(1);

	//lr.print_sigma();

	std::cout << "Result of gauss 3: " << lr.gauss_method_3(6) << std::endl;

	return 0;
}