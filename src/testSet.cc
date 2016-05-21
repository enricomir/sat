#include "testSet.hh"
#include <dirent.h>
#include <random>
#include <fstream>
#include <iostream>
#include <cstdlib>

std::vector<std::string> testSet::get_files() {
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

int testSet::run_trial(std::string file) {
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

	const int stages = 3;
	unsigned int fit;

	for (int s = 0; s < stages; ++s) {
		int chosen_mh = get_best_match(p, mhc, s);
		fit = mhc(i); //runs the MH
		std::cout << "Finished stage " << s << " for problem " << file << "\n";
	}
	std::cout << "\n";

	return fit;
}

void testSet::save_run(int r, std::string f) {
	std::cerr <<  "insert into test_runs values ('ml2',\""<<f<<"\","<<r<<");\n";
}

void testSet::generate_parallel_input() {
	const int num_reps = 20;
	std::ofstream of("test.ml.parallel.script");
	
	auto files = get_files();

	for (auto f: files) {
		for (int i = 0; i < num_reps; ++i)
			of << "./bin/meta_learner.app " << f << "\n";
	}
}

int testSet::get_best_match(SatProblem p, mhController mhc, int stage) {
	std::ofstream of("./models/query.csv");

	of << stage << ", ";

	//Problem meta features
	problemMetaFeatures pm(p.name);

	problemMetaFeatures::ProblemSize ps = pm.problem_size();
	problemMetaFeatures::Balance b = pm.balance();


	of << ps.clauses << ", " << ps.variables << ", "
		<< ps.ratio << ", " << ps.inv_ratio << ", " << ps.normalized_ratio << ", "

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

	of << mhc.pops.ga_pop.size() << ", " << mhc.pops.vector_size << ", "

		<< euc.avg << ", " << euc.stddev << ", "
		<< euc.vc << ", " << euc.min << ", "
		<< euc.max << ", " << euc.entropy << ", "

		<< ham.avg << ", " << ham.stddev << ", "
		<< ham.vc << ", " << ham.min << ", "
		<< ham.max << ", " << ham.entropy << ", "

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

	std::system("sed -i -- 's/nan/0/g' query.csv");
	std::system("mlpack_allkrann -m models/krann.txt -n ./models/neigh.txt -q ./models/query.csv -k 1");

	std::ifstream input("./models/neigh.txt");
	long i;

	input >> i;

	std::cout << "Neighbor: " << i << " (" << classes[i] << ")\n";
	//return classes[i];
	return 3;
}

testSet::testSet() {
	std::ifstream input("./models/labels.csv");
	while (input.good()) {
		unsigned char i;
		input >> i;
		classes.push_back(i);
	}
}


