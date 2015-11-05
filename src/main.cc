#include <iostream>
#include <chrono>
#include <stdexcept>
#include "SatProblem.hh"
#include "BnB.hh"
#include "bitga.h"
#include "binarypso.h"

int main(int argc, char** argv) {
	try {
		ga_main_function(argc, argv);
		//pso_main_function(argc, argv);
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << '\n';
	}
	return 0;
}



int bnb_main(int argc, char* argv[]) {
	if (argc==1) {
		std::cerr << "Error! Usage: ./sat.app <filename>" << std::endl;
		return 1;
	}

	SatProblem p(argv[1]);
	p.printClauses();

	BnB c(p);

	c.solve(std::chrono::seconds(5));
	c.best_so_far.eval();
	std::cout << "c BestVars \n";
	c.best_so_far.printVars();
	std::cout << "c BestMax: " << c.best_so_far.true_clauses << "\n";
	std::cout << "c BestMin: " << c.best_so_far.false_clauses << "\n";
	c.best_so_far.printClauses();
	return 0;
}

