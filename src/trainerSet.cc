#include "trainerSet.hh"
#include <dirent.h>
#include <random>
#include <fstream>
#include <iostream>

std::vector<std::string> trainerSet::get_files() {
	std::vector<std::string> ret;

	{
		DIR* di = opendir("./dat/cnf/crafted/");
		dirent* d;
		
		d=readdir(di); //skip .
		d=readdir(di); //skip ..

		while ( (d=readdir(di)) != NULL) {
			ret.push_back(std::string("./dat/cnf/crafted/")+d->d_name);
		}

		closedir(di);
	}
	{
		DIR* di = opendir("./dat/cnf/random/");
		dirent* d;
		
		d=readdir(di); //skip .
		d=readdir(di); //skip ..

		while ( (d=readdir(di)) != NULL) {
			ret.push_back(std::string("./dat/cnf/random/")+d->d_name);
		}

		closedir(di);
	}

	return ret;
}

std::vector<trainerSet::run> trainerSet::run_trial(std::string file) {
	std::vector<trainerSet::run> ret;

	SatProblem p(file);
	mhController mhc(p);
	std::cout << "run_trial open file " << file << "\n";

	//To choose starting population as well as meta-heuristic, total 12
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rnd(0, mhc.pop_sizes.size() - 1); 

	//Initializes pop from random number of individuals
	int i = rnd(gen);
	mhc.pops.resize(mhc.pop_sizes[i]);
	std::string pop = "./pop/" + popController::getUUID();
	mhc.pops.save(pop);


	const int stages = 3;

	for (int s = 0; s < stages; ++s) {
		std::string next_pop;
		bool chosen = false;
		for (int replications = 0; replications < 20; replications++) {
			int chosen_mh = rnd(gen);
			std::cout << "MHs for stage " << s << "(" << replications << "), pop=" << pop << "\n";
			for (int i = 0; i < mhc.pop_sizes.size(); ++i) { //Run a MH
				mhc.pops.load(pop);
				unsigned int fit = mhc(i); //runs the MH
				std::cout << i << "/" << std::flush;

				run r;
				r.mh = i;
				r.stage = s;
				r.problem = file;
				r.init_pop = pop;
				r.fit = fit;

				ret.push_back(r);

				if (i == chosen_mh && !chosen) { //Saves pop for next stage
					next_pop = "./pop/" + popController::getUUID();
					mhc.pops.save(next_pop);
					chosen = true;
				}
			}
			std::cout << "\n";
			std::cout << "Finished stage " << s << " for problem " << file << "\n";
			std::cout << "Next pop: " << pop << "\n";

		}
		pop = next_pop;
	}

	return ret;
}

void trainerSet::save_run(run r) {
	std::cerr <<  "insert into runs values (" << r.mh << ", " << r.stage
		<< ", \"" << r.problem << "\", \"" << r.init_pop << "\", " << r.fit << ");\n";
}

void trainerSet::generate_parallel_input() {
	std::ofstream of("opt.parallel.script");

	auto files = get_files();

	for (auto f: files) {
			of << "./bin/opt.app " << f << "\n";
	}
}
