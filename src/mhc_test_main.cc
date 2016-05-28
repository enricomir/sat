#include <iostream>
#include <cstdlib>
#include "SatProblem.hh"
#include "mhcTestSet.hh"

int main(int argc, char** argv) {
	mhcTestSet t;
	if (argc == 1) {
		t.generate_parallel_input();
		std::cout << "Parallel input generated. Please run parallel < test.mh.parallel.script ^ test.mh.inserts.sql(on fish)";
		return 0;
	}

	int mh = std::atoi(argv[1]);

	std::cout << mh;

	auto r = t.run_trial(mh, argv[2]);
	t.save_run(r);
	return 0;
}

