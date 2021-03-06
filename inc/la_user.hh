#ifndef LA_USER_HH
#define LA_USER_HH

#include "la.hh"
#include <vector>
#include "mhcontroller.hh"

class laUser {
	public:
		static std::vector<std::string> get_files(); //List all problems
		static void generate_parallel_input();

		LA las[3];
		
		laUser(bool train = true);
		~laUser();

		int getState(SatProblem p, mhController mhc, int stage);

		int run_trial(std::string file);

		void save_run(std::string file, int r);

	private:
		bool train;
};

#endif

