#include <iostream>
#include "SatProblem.hh"
#include "problemMetaFeatures.hh"
#include "populationMetaFeatures.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		problemMetaFeatures::generate_parallel_input();
		populationMetaFeatures::generate_parallel_input();
		std::cout << "Parallel input generated. Please run parallel < problem.parallel.script ^ problem.inserts.sql(on fish)\n";
		std::cout << "Parallel input generated. Please run parallel < population.parallel.script ^ population.inserts.sql(on fish)";
		return 0;
	}
	if (argv[1][0] == 'r') { //pRoblem x pOpulation
		problemMetaFeatures p(argv[2]);
		p.generate_insert();
		return 0;
	}
	if (argv[1][0] == 'o') { //pRoblem x pOpulation
		unsigned int vec_size;
		{	
			std::ifstream f(std::string(argv[2])+".pop");
			f >> vec_size;
		}
		populationMetaFeatures p(argv[2], vec_size);
		p.generate_insert();
		return 0;
	}
	return 0;
}

