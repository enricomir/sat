#ifndef _BnB_HH_
#define _BnB_HH_

#include <vector>
#include <chrono>
#include "SatProblem.hh"

class BnB {
	public:
		BnB(const SatProblem& s);
		SatProblem best_so_far;

		bool solve(std::chrono::seconds time);
		bool bnb(SatProblem& s);
};

#endif

