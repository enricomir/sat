#include <iostream>
#include <chrono>
#include "SatProblem.hh"
#include "BnB.hh"

int main(int argc, char* argv[]) {
	if (argc==1) {
		std::cerr << "Error! Usage: ./sat.app <filename>" << std::endl;
		return 1;
	}

	SatProblem p(argv[1]);
	p.printClauses();

	/*
	std::cout << "c true=" << p.true_clauses << " / false=" << p.false_clauses
		<< "\n";

	for (int i = 0; i < p.nvar; ++i) {
		p.set(i,true);
	}

	std::cout << "c true=" << p.true_clauses << " / false=" << p.false_clauses
		<< "\n";

		*/

	BnB c(p);

	c.solve(std::chrono::seconds(5));
	std::cout << "c BestVars \n";
	c.best_so_far.printVars();
	std::cout << "c BestMax: " << c.best_so_far.true_clauses << "\n";
	std::cout << "c BestMin: " << c.best_so_far.false_clauses << "\n";
	c.best_so_far.printClauses();
	return 0;
}

