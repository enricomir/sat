#include <iostream>
#include "q_learn.hh"
#include "q_user.hh"

int main(int argc, char** argv) {
	if (argc == 1) {
		qUser::generate_parallel_input();
		std::cout << "Parallel input generated. Please run parallel < test.rl.parallel.script ^ inserts.rl.sql(on fish)\n";
		return 0;
	}

	qUser q(false);
	//std::cout << "q_main created qUser\n";
	std::string f(argv[1]);
	std::cout << "Trying for " << f << "\n";
	int r = q.run_trial(f);
	q.save_run(f, r);
	/*
	std::cout << "It works!\n";
	Q_Learn q;

	q.state = 1;
	q.print();
	
	std::cout << "Rewarding: 10, future state 100\n";
	q.state = 1;
	q.action = 1;
	q.reward(10, 100);
	
	q.print();
	
	std::cout << "Saving to q.txt\n";
	q.save("q.txt");
	
	Q_Learn q2;
	
	q2.state = 1;
	
	std::cout << "Q2:\n";
	q2.print();
	
	q2.load("q.txt");
	std::cout << "After load:\n";
	q2.print();*/

	return 0;
}

