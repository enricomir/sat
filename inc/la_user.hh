#ifndef LA_USER_HH
#define LA_USER_HH

#include "la.hh"
#include <vector>
#include "mhcontroller.hh"

class laUser {
	public:
		static std::vector<std::string> get_files(); //List all problems
		static void generate_parallel_input(); //OK

		LA las[3];
		
		laUser(bool train = true); //OK
		~laUser(); //Ok

		int getState(SatProblem p, mhController mhc, int stage); //OK

		int run_trial(std::string file); //OK

		void save_run(std::string file, int r);

	private:
		bool train;
};

#endif

