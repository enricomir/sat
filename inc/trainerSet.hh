#ifndef TRAINERSET_H
#define TRAINERSET_H

#include <vector>
#include <string>
#include "mhcontroller.hh"
#include "popcontroller.hh"
#include "SatProblem.hh"

class trainerSet {
	public:
		struct run {
			int mh;
			int stage;
			std::string problem;
			std::string init_pop;
			unsigned int fit; //Final fitness
		}; //Information about a run

		std::vector<std::string> get_files(); //List all possible files

		//Runs a single set of MH's
		//Select a random step at each trial
		std::vector<run> run_trial(std::string file); 

		void save_run(run r); //Just prints SQL statements; safer
	
		void generate_parallel_input();
};

#endif //TRAINERSET_H

