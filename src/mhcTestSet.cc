#include "mhcTestSet.hh"
#include <dirent.h>
#include <random>
#include <fstream>
#include <iostream>

std::vector<std::string> mhcTestSet::get_files() {
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

mhcTestSet::run mhcTestSet::run_trial(int mh, std::string file) {
	SatProblem p(file);
	mhController mhc(p);
	std::cout << "run_trial open file " << file << "\n";

	//Initializes pop from random number of individuals
	mhc.pops.resize(mhc.pop_sizes[mh]);

	unsigned int fit = mhc(mh); //runs the MH
	fit = mhc(mh);
	fit = mhc(mh);

	run r;
	r.mh = mh;
	r.problem = file;
	r.fit = fit;

	std::cout << "\n";

	std::cout << "Finished for problem " << file << "\n";

	return r;
}

void mhcTestSet::save_run(run r) {
	std::cerr <<  "insert into test_runs values (\"" << r.mh << "\", " 
		<< "\"" << r.problem << "\", " << r.fit << ");\n";
}

void mhcTestSet::generate_parallel_input() {
	const int num_reps = 20;
	std::ofstream of("test.mh.parallel.script");

	auto files = get_files();

	for (auto f: files) {
		for (int mh = 0; mh < 12; mh++) {
			for (int i = 0; i < num_reps; ++i)
				of << "./bin/meta_heuristics.app " << mh << " " << f << "\n";
		}
	}
}
