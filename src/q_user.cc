#include "q_user.hh"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "mhcontroller.hh"
#include "popcontroller.hh"
#include "SatProblem.hh"
#include "problemMetaFeatures.hh"
#include "populationMetaFeatures.hh"


std::vector<std::string> qUser::get_files() {
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

void qUser::generate_parallel_input() {
	const int num_reps = 20;
	std::ofstream of("test.ql.parallel.script");
	
	auto files = get_files();

	for (int i = 0; i < num_reps; ++i)
		for (auto f: files) {
			of << "./bin/q_learning.app " << f << "\n";
	}
}

qUser::qUser(bool train) : train(train) {
	qls[0].load("stage0.ql");
	qls[1].load("stage1.ql");
	qls[2].load("stage2.ql");
}

qUser::~qUser() {
	qls[0].save("stage0.ql");
	qls[1].save("stage1.ql");
	qls[2].save("stage2.ql");
}

int qUser::getState(SatProblem p, mhController mhc, int stage) {
	std::ofstream of("./models/q_query.csv");

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
	mhc.pops.save("q");
	populationMetaFeatures pmf("q", p.variables.size());

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

	std::system("sed -i -- 's/nan/0/g' ./models/q_query.csv");
	std::system("mlpack_knn -m models/knn.txt -n ./models/q_neigh.txt -q ./models/q_query.csv -k 1");

	std::ifstream input("./models/q_neigh.txt");
	long i;

	input >> i;

	return i;
}

int qUser::run_trial(std::string file) {
	SatProblem p(file);
	mhController mhc(p);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rnd(0, mhc.pop_sizes.size() - 1); 

	//Initializes pop from random number of individuals
	int i = rnd(gen);
	mhc.pops.resize(mhc.pop_sizes[i]);
	//std::cout << "qUser::run_trial pop created\n";

	const int stages = 3;
	int fit;
	int ret;
	int first;

	for (int s = 0; s < stages; ++s) {
		qls[s].state = getState(p, mhc, s);
		qls[s].choose();
		//std::cout << "qUser::run_trial chosen: " << qls[s].action << "\n";
		

		ret = mhc(qls[s].action);
		if (s != 0) { //Reward
			//std::cout << "qUser::run_trial rewarding intermediate\n";
			if (train)
				qls[s-1].reward(static_cast<float>(mhc.initial - mhc.fin)/mhc.initial, qls[s].max_reward());
		} else {
			first = mhc.initial;
		}
		//fit += (mhc.initial - mhc.fin); //runs the MH
		//std::cout << "Finished stage " << s << " for problem " << file << "\n";
	}
	fit = first - mhc.fin;
	//Final reward
	//std::cout << "qUser::run_trial final reward: " << static_cast<float>(fit)/mhc.fin << 
	//	" [state=" << qls[stages-1].state << "]" <<
	//	" [action=" << qls[stages-1].action << "]\n";
	if (fit > first) {
		std::cout << "Error: fit=" << fit << " first=" << first << "\n";
		exit(-1);
	}
	if (train)
		qls[stages-1].reward(static_cast<float>(/*fit*/mhc.initial-mhc.fin)/mhc.initial/*/first*//*/mhc.fin*/, 0);
	
	std::cout << "\n";

	return ret;
}

void qUser::save_run(std::string file, int r) {
	std::cerr <<  "insert into test_runs values ('ql',\""<<file<<"\","<<r<<");\n";
}



