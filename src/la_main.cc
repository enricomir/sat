#include <iostream>
#include "la.hh"
#include "la_user.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		laUser::generate_parallel_input();
		std::cout << "Parallel input generated. Please run parallel < test.rl.parallel.script ^ inserts.rl.sql(on fish)\n";
		return 0;
	}

	laUser l;
	std::string f(argv[1]);
	std::cout << "Trying for " << f << "\n";
	int r = l.run_trial(f);
	l.save_run(f, r);
	return 0;
}

