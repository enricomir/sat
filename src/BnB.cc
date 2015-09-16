#include "BnB.hh"
#include "SatProblem.hh"
#include <iostream>

BnB::BnB(const SatProblem& s) : best_so_far(s) {
}

bool BnB::solve(std::chrono::seconds time) {
	SatProblem s(best_so_far);
	for (auto b: s.allocated) b = false;
	bnb(s);

	return true;
}

bool BnB::bnb(SatProblem& s) {
	//Assume it is a complete solution
	bool complete = true;

#ifdef DEBUG_BNB
	std::cout << "-----------------------------------\n";
	std::cout << "c Evaluating: \n";
	s.printVars();
	std::cout << "c SAT = " << s.eval() << "\n";
#endif
	if (s.eval() > best_so_far.eval()) {
		best_so_far = s;
		std::cout << "c BnB: revised best. S=" << s.eval() << "/BSF=" << best_so_far.eval() << "\n";
	}
#ifdef DEBUG_BNB
	//Aborts if can't be better than the actual best
	std::cout << "c BSF: " << best_so_far.eval()
						<< "  LIM: " << (s.clauses.size() - s.false_clauses) << "\n";
#endif
	if (best_so_far.eval() >= (s.clauses.size() - s.false_clauses)) {
		return false;
	}
	//Loop variables
	for (int i = 0; i < s.variables.size(); ++i) {
		//Continue if allocated
		if (s.allocated[i] == true) continue;

		//If not, this is a partial solution
		complete = false;


		//Assign first value to variables
		s.variables[i] = true;
		s.allocated[i] = true;
		bnb(s);

		//Assign second value to variables
		s.variables[i] = false;
		bnb(s);
		s.allocated[i] = false;
		break;
	}

	return false;
}
