#ifndef PROBLEM_META_FEATURES_H
#define PROBLEM_META_FEATURES_H

#include "SatProblem.hh"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cfloat>

class problemMetaFeatures {
	public:
		problemMetaFeatures(std::string problem) : p(problem) { } ;
		void generate_insert();
		static void generate_parallel_input();

	//private:
		static std::vector<std::string> get_files();
		SatProblem p;
		std::string problem_name;
		struct Statistic {
			double avg;
			double stddev;
			double vc;
			double min;
			double max;
			double entropy;
		};
		Statistic calculate(std::vector<double> v);
		
		struct ProblemSize{
			int clauses;
			int variables;
			double ratio;
			double inv_ratio;
			double normalized_ratio;
		};
		ProblemSize problem_size();

		struct Balance {
			double clausePTN_avg;
			double clausePTN_std_dev;
			double clausePTN_vc;
			double clausePTN_min;
			double clausePTN_max;
			double clausePTN_entropy;

			double varPTN_avg;
			double varPTN_std_dev;
			double varPTN_vc;
			double varPTN_min;
			double varPTN_max;
			double varPTN_entropy;

			double unary;
			double binary;
			double ternary;
		};
		Balance balance();
};

#endif
