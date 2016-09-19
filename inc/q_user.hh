#ifndef Q_USER_HH
#define Q_USER_HH

#include "q_learn.hh"
#include <vector>
#include "mhcontroller.hh"

class qUser {
	public:
		static std::vector<std::string> get_files(); //List all problems
		static void generate_parallel_input();

		Q_Learn qls[3];
		
		qUser(bool train = true);
		~qUser();

		int getState(SatProblem p, mhController mhc, int stage);

		int run_trial(std::string file);

		void save_run(std::string file, int r);

	private:
		bool train;
};

#endif

