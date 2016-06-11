#include <iostream>
#include "la.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		LA l;
		l.state = 1;
		l.choose();
		
		l.print();
		l.reward(300);
		l.print();

		std::cout << "Conv: " << l.get_convergence() << "\n";
		for (int i = 0; i < 10; ++i) {
			l.state = i;
			l.choose();
			l.reward(300);
		}
		std::cout << "Conv: " << l.get_convergence() << "\n";

		l.save("1.la");
		l.load("1.la");
		std::cout << "Conv load: " << l.get_convergence();

		return 0;
	}
	return 0;
}

