#include <iostream>
#include "SatProblem.hh"
#include "problemMetaFeatures.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		problemMetaFeatures::generate_parallel_input();
		//TODO: add population parallel
		std::cout << "Parallel input generated. Please run parallel < problem.parallel.script ^ problem.inserts.sql(on fish)";
		//std::cout << "Parallel input generated. Please run parallel < population.parallel.script ^ population.inserts.sql(on fish)";
		return 0;
	}
	if (argv[1][0] == 'r') { //pRoblem x pOpulation
		problemMetaFeatures p(argv[2]);
		p.generate_insert();
	}
	return 0;
}

