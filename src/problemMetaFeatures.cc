#include "problemMetaFeatures.hh"
#include <dirent.h>
#include <fstream>

void problemMetaFeatures::generate_parallel_input() {
	std::vector<std::string> files = get_files();
	std::ofstream of("problem.parallel.script");

	for (auto f: files) {
		of << "./bin/meta_features.app r " << f << "\n";
	}
}

std::vector<std::string> problemMetaFeatures::get_files() {
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

problemMetaFeatures::ProblemSize problemMetaFeatures::problem_size() {
	problemMetaFeatures::ProblemSize ret;

	ret.clauses = p.clauses.size();
	ret.variables = p.variables.size();

	ret.ratio = static_cast<double>(ret.clauses)/ret.variables;
	ret.inv_ratio = static_cast<double>(ret.variables)/ret.clauses;
	ret.normalized_ratio = std::abs(4.26 - ret.ratio);

	return ret;
}

problemMetaFeatures::Balance problemMetaFeatures::balance() {
	problemMetaFeatures::Balance ret;

	//Clauses Positive to negative-ratio
	std::vector<double> pos_to_neg_ratio;

	int i = 0;
	for (auto c: p.clauses) {
		double positive = 0.0, negative = 0.0;
		for (auto i: c) {
			if (i>0) {
				positive++;
			} else {
				negative++;
			}
		}
		pos_to_neg_ratio.push_back(positive/(positive+negative));
	}

	Statistic sta = calculate(pos_to_neg_ratio);

	ret.clausePTN_avg = sta.avg;
	ret.clausePTN_std_dev = sta.stddev;
	ret.clausePTN_vc = sta.vc;
	ret.clausePTN_min = sta.min;
	ret.clausePTN_max = sta.max;
	ret.clausePTN_entropy = sta.entropy;

	//Variables positive to negative ratio
	std::vector<double> positive_var, negative_var;
	for (auto i: p.variables) {
		positive_var.push_back(0);
		negative_var.push_back(0);
	}

	for (auto c: p.clauses) {
		for (auto i: c) {
			if (i>0) {
				positive_var[i  -1]++;
			} else {
				negative_var[-i -1]++;
			}
		}
	}

	std::vector<double> var_ratio;
	for (unsigned long int i = 0; i < p.variables.size(); ++i) {
		var_ratio.push_back(static_cast<double>(positive_var[i])/(negative_var[i] + positive_var[i]));
	}

	sta = calculate(var_ratio);

	ret.varPTN_avg = sta.avg;
	ret.varPTN_std_dev = sta.stddev;
	ret.varPTN_vc = sta.vc;
	ret.varPTN_min = sta.min;
	ret.varPTN_max = sta.max;
	ret.varPTN_entropy = sta.entropy;

	ret.unary = 0;
	ret.binary = 0;
	ret.ternary = 0;
	for (auto c: p.clauses) {
		if (c.size() == 1)
			ret.unary++;
		else if (c.size() == 2)
			ret.binary++;
		else if (c.size() == 3)
			ret.ternary++;
	}

	ret.unary   = ret.unary/p.clauses.size(); 
	ret.binary  = ret.binary/p.clauses.size(); 
	ret.ternary = ret.ternary/p.clauses.size(); 

	return ret;
}

problemMetaFeatures::Statistic problemMetaFeatures::calculate(std::vector<double> v) {
	Statistic ret;
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	ret.avg = sum/v.size();

	double acc = 0.0;
	std::for_each(v.begin(), v.end(), 
			[&acc,ret](const double d) { acc += (d-ret.avg)*(d-ret.avg); } );
	ret.stddev = std::sqrt(acc / v.size());

	ret.vc = ret.stddev / ret.avg;

	ret.min = v[0];
	std::for_each(v.begin(), v.end(),
			[&ret](const double d) { ret.min = (ret.min > d? d : ret.min); } );

	ret.max = v[0];
	std::for_each(v.begin(), v.end(),
			[&ret](const double d) { ret.max = (d > ret.max ? d : ret.max); } );

	ret.entropy = 0.0;
	std::for_each(v.begin(), v.end(),
			[&ret,sum](const double d) { 
			double p_i = d/sum;
			if (p_i != 0)
			ret.entropy += p_i*std::log(p_i);
			} );

	return ret;
}

void problemMetaFeatures::generate_insert() {
	ProblemSize ps = problem_size();
	Balance b = balance();


	std::cerr << "insert into problem_meta values (\"" << p.name << "\", "
		<< ps.clauses << ", " << ps.variables << ", "
		<< ps.ratio << ", " << ps.inv_ratio << ", " << ps.normalized_ratio << ", "

		<< b.clausePTN_avg << ", " << b.clausePTN_std_dev << ", "
		<< b.clausePTN_vc << ", " << b.clausePTN_min << ", "
		<< b.clausePTN_max << ", " << b.clausePTN_entropy << ", "

		<< b.varPTN_avg << ", " << b.varPTN_std_dev << ", "
		<< b.varPTN_vc << ", " << b.varPTN_min << ", "
		<< b.varPTN_max << ", " << b.varPTN_entropy << ", "
		
		<< b.unary << ", " << b.binary << ", " << b.ternary << ");\n";
}



