#ifndef TESTSET_H
#define TESTSET_H

#include <vector>
#include <string>
#include "mhcontroller.hh"
#include "popcontroller.hh"
#include "SatProblem.hh"
#include "problemMetaFeatures.hh"
#include "populationMetaFeatures.hh"

class testSet {
	public:
		static std::vector<std::string> get_files(); //List all problems
		static void generate_parallel_input();

		testSet();

		//Runs a single run while asking for the classifier to choose
		int run_trial(std::string file); 

		int get_best_match(SatProblem p, mhController mhc, int stage);

		void save_run(int r, std::string f); //Just prints SQL statements

		std::vector<unsigned char> classes;
};

#endif //TESTSET_H

