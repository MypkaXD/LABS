#include <iostream>
#include <vector>

int main(){

	srand(0);
	
	int count_of_students = 0;

	do {
		std::cout << "Enter count of students: ";

		std::cin >> count_of_students;

		if (count_of_students <= 0)
			std::cout << "Wrong count! Try again!" << std::endl;

	} while (count_of_students <= 0);

	std::vector<int> count_of_answered_tickets(count_of_students, 0);

	double p = 0;

	do {
		std::cout << "Enter p: ";

		std::cin >> p;

		if (p > 1 || p < 0)
			std::cout << "Wrong p! Try again!" << std::endl;

	} while (p < 0 || p > 1);

	std::cout << "P: " << p << std::endl;

	double t = 0.0;

	for (size_t count = 0; count < count_of_students; ++count) {
		while (true) {

			t = ((double)rand()) / RAND_MAX;

			std::cout << t << std::endl;
			std::cout << count_of_answered_tickets[count] << std::endl;

			if (t <= p) {
				std::cout << "HERE" << std::endl;
				++count_of_answered_tickets[count];
			}
			else break;
		}
	}

	for (size_t count = 0; count < count_of_students; ++count) {
		std::cout << "Student " << count + 1 << " anserewed on " << count_of_answered_tickets[count] << " tickets!" << std::endl;
	}

	return 0;
}