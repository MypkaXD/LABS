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

	void gauss_method() {



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

	lr.set_sigma_square(1);

	lr.print_sigma();

	std::cout << "Result of method trap: " << lr.trapezoid_method() << std::endl;

	return 0;
}