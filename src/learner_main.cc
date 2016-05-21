#include <iostream>
#include "SatProblem.hh"
#include "testSet.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		testSet::generate_parallel_input();
		std::cout << "Parallel input generated. Please run parallel < test.ml.parallel.script ^ learner_test.inserts.sql(on fish)\n";
		return 0;
	}

	testSet t;
	std::string f(argv[1]);
	std::cout << "Trying for " << f << "\n";
	int r = t.run_trial(f);
	t.save_run(r, f);

	return 0;
}

