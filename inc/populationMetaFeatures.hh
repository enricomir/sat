#ifndef POPULATION_META_FEATURES_H
#define POPULATION_META_FEATURES_H

#include "SatProblem.hh"
#include "popcontroller.hh"

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cfloat>

class populationMetaFeatures {
	public:
		populationMetaFeatures(std::string population, unsigned int vecsize);
		//TODO
		void generate_insert();
		static void generate_parallel_input();

	//private:
		static std::vector<std::string> get_files();
		popController popc;

		std::string pop_name;

		struct Statistic {
			double avg;
			double stddev;
			double vc;
			double min;
			double max;
			double entropy;
		};
		Statistic calculate(std::vector<double> v);

		Statistic euclidean_distances();
		double calculate_euclidean_distance(Indi a, Indi b);

		Statistic hamming_distances();
		double calculate_hamming_distance(Indi a, Indi b);

		//Statistic diagonal_distances();
		//double calculate_diagonal_distance(Indi a, Indi b);

		Statistic shannon_entropy();

		Statistic simpson_diversity_index();

		//Statistic berger_park_index();

		Statistic probabilities();

};

#endif

