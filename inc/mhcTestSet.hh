#ifndef MHCTESTSET_H
#define MHCTESTSET_H

#include <vector>
#include <string>
#include "mhcontroller.hh"
#include "popcontroller.hh"
#include "SatProblem.hh"

class mhcTestSet {
	public:
		struct run {
			int mh;
			std::string problem;
			unsigned int fit; //Final fitness
		}; //Information about a run

		std::vector<std::string> get_files(); //List all possible files

		//Runs a single set of MH's
		run run_trial(int mh, std::string file); 

		void save_run(run r); //Just prints SQL statements; safer
	
		void generate_parallel_input();
};

#endif

