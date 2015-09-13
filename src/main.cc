#include <iostream>
#include "SatProblem.hh"
#include "BnB.hh"

int main(int argc, char* argv[]) {
	if (argc==1) {
		std::cerr << "Error! Usage: ./sat.app <filename>" << std::endl;
		return 1;
	}
	SatProblem p(argv[1]);
	p.printClauses();

	std::cout << "c Eval: " << p.eval() << "\n";

	for (int i = 0; i < p.allocated.size(); ++i) {
		p.allocated[i] = true;
	}

	std::cout << "c Eval: " << p.eval() << "\n";

	/*BnB c(p);
	std::cout << c.solve(std::chrono::seconds(5)); */
	return 0;
}

