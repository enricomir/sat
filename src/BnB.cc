#include "BnB.hh"
#include "SatProblem.hh"
#include <iostream>

BnB::BnB(const SatProblem& s) : best_so_far(s) {
}

bool BnB::solve(std::chrono::seconds time) {
	SatProblem s(best_so_far);
	s.reset();
	bnb(s);

	return true;
}

void BnB::bnb(SatProblem& s, int start_var) {
	if (start_var >= s.nvar) return;

#ifdef DEBUG_BNB
	std::cout << "-----------------------------------\n";
	std::cout << "c Evaluating: \n";
	s.printVars();
	std::cout << "c SAT = " << s.true_clauses << "\n";
#endif

	if (s.true_clauses > best_so_far.true_clauses) {
		best_so_far = s;
		std::cout << "c BnB: revised best. S=" << s.true_clauses << "\n";
	}

	//Aborts if can't be better than the actual best
#ifdef DEBUG_BNB
	std::cout << "c BSF: " << best_so_far.true_clauses
						<< "  LIM: " << (s.clauses.size() - s.false_clauses) << "\n";
#endif

	if (best_so_far.true_clauses >= (s.clauses.size() - s.false_clauses)) {
		return;
	}

	//Assign first value to variables
	s.set(start_var,true);
	bnb(s, start_var+1);

	//Assign second value to variables
	s.set(start_var,false);
	bnb(s, start_var+1);
	s.unset(start_var);

	return;
}
