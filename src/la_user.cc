#include "la_user.hh"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "mhcontroller.hh"
#include "popcontroller.hh"
#include "SatProblem.hh"
#include "problemMetaFeatures.hh"
#include "populationMetaFeatures.hh"

std::vector<std::string> laUser::get_files() {
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

void laUser::generate_parallel_input() {
	const int num_reps = 20;
	std::ofstream of("test.la.parallel.script");
	
	auto files = get_files();

	for (auto f: files) {
		for (int i = 0; i < num_reps; ++i)
			of << "./bin/learning_automata.app " << f << "\n";
	}
}

laUser::laUser(bool train) : train(train) {
	las[0].load("stage0.la");
	las[1].load("stage1.la");
	las[2].load("stage2.la");
}

laUser::~laUser() {
	las[0].save("stage0.la");
	las[1].save("stage1.la");
	las[2].save("stage2.la");
}

int laUser::getState(SatProblem p, mhController mhc, int stage) {
	std::ofstream of("./models/query.csv");

	of << stage << ", ";

	//Problem meta features
	problemMetaFeatures pm(p.name);

	problemMetaFeatures::ProblemSize ps = pm.problem_size();
	problemMetaFeatures::Balance b = pm.balance();


	of << ps.clauses/1500.0 << ", " << ps.variables/300.0 << ", "
		<< ps.ratio/10.0 << ", " << ps.inv_ratio << ", " << ps.normalized_ratio/10.0 << ", "

		<< b.clausePTN_avg << ", " << b.clausePTN_std_dev << ", "
		<< b.clausePTN_vc << ", " << b.clausePTN_min << ", "
		<< b.clausePTN_max << ", " << b.clausePTN_entropy << ", "

		<< b.varPTN_avg << ", " << b.varPTN_std_dev << ", "
		<< b.varPTN_vc << ", " << b.varPTN_min << ", "
		<< b.varPTN_max << ", " << b.varPTN_entropy << ", "

		<< b.unary << ", " << b.binary << ", " << b.ternary << ", ";

	//Population meta features
	mhc.pops.save("1");
	populationMetaFeatures pmf("1", p.variables.size());

	populationMetaFeatures::Statistic euc, ham, sha, simp, prob;

	euc = pmf.euclidean_distances();
	ham = pmf.hamming_distances();
	sha = pmf.shannon_entropy();
	simp= pmf.simpson_diversity_index();
	prob= pmf.probabilities();

	of << mhc.pops.ga_pop.size()/50.0 << ", "

		<< euc.avg << ", " << euc.stddev << ", "
		<< euc.vc << ", " << euc.min << ", "
		<< euc.max << ", " << euc.entropy << ", "

		<< ham.avg/100.0 << ", " << ham.stddev << ", "
		<< ham.vc << ", " << ham.min/100.0 << ", "
		<< ham.max/100.0 << ", " << ham.entropy << ", "

		<< sha.avg << ", " << sha.stddev << ", "
		<< sha.vc << ", " << sha.min << ", "
		<< sha.max << ", " << sha.entropy << ", "

		<< simp.avg << ", " << simp.stddev << ", "
		<< simp.vc << ", " << simp.min << ", "
		<< simp.max << ", " << simp.entropy << ", "

		<< prob.avg << ", " << prob.stddev << ", "
		<< prob.vc << ", " << prob.min << ", "
		<< prob.max << ", " << prob.entropy << "\n";

	of << std::flush;

	std::system("sed -i -- 's/nan/0/g' ./models/query.csv");
	std::system("mlpack_allknn -m models/knn.txt -n ./models/neigh.txt -q ./models/query.csv -k 1");

	std::ifstream input("./models/neigh.txt");
	long i;

	input >> i;

	return i;
}

int laUser::run_trial(std::string file) {
	SatProblem p(file);
	mhController mhc(p);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rnd(0, mhc.pop_sizes.size() - 1); 

	//Initializes pop from random number of individuals
	int i = rnd(gen);
	mhc.pops.resize(mhc.pop_sizes[i]);

	const int stages = 3;
	unsigned int fit;
	unsigned int ret;

	for (int s = 0; s < stages; ++s) {
		las[s].state = getState(p, mhc, s);
		las[s].choose();
		ret = mhc(las[s].action);
		fit += (mhc.initial - mhc.fin); //runs the MH
		std::cout << "Finished stage " << s << " for problem " << file << "\n";
	}
	std::cout << "\n";

	if (train) {
		las[0].reward(fit);
		las[1].reward(fit);
		las[2].reward(fit);
	}

	return ret;
}

void laUser::save_run(std::string file, int r) {
	std::cerr <<  "insert into test_runs values ('rl',\""<<file<<"\","<<r<<");\n";
}


