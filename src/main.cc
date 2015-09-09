#include <iostream>
#include "SatProblem.hh"

int main(int argc, char* argv[]) {
	if (argc==1) {
		std::cerr << "Error! Usage: ./sat.app <filename>" << std::endl;
		return 1;
	}
	SatProblem p(argv[1]);
	p.printClauses();
	return 0;
}

