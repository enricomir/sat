#include "populationMetaFeatures.hh"

#include <dirent.h>
#include <fstream>

populationMetaFeatures::populationMetaFeatures(std::string population, unsigned int vecsize) : popc(vecsize) {
	popc.load(population);
	pop_name = population;
}

void populationMetaFeatures::generate_parallel_input() {
	std::vector<std::string> files = get_files();
	std::ofstream of("population.parallel.script");

	for (auto f: files) {
		of << "./bin/meta_features.app o " << f << "\n";
	}
}

populationMetaFeatures::Statistic populationMetaFeatures::calculate(std::vector<double> v) {
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

std::vector<std::string> populationMetaFeatures::get_files() {
	std::vector<std::string> ret;

	DIR* di = opendir("./pop/");
	dirent* d;

	d=readdir(di); //skip .
	d=readdir(di); //skip ..

	while ( (d=readdir(di)) != NULL) {
		std::string n = std::string("./pop/")+d->d_name;
		n.resize(n.size()-4);
		ret.push_back(n);
	}

	closedir(di);

	return ret;
}

double populationMetaFeatures::calculate_euclidean_distance(Indi a, Indi b) {
	double sum = 0.0;

	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) //For binary, euclidean is almost equal to hamming
			sum += 1;
	}

	return std::sqrt(sum);
}

double populationMetaFeatures::calculate_hamming_distance(Indi a, Indi b) {
	double sum = 0.0;

	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) //For binary, euclidean is almost equal to hamming
			sum += 1;
	}

	return sum;
}

populationMetaFeatures::Statistic populationMetaFeatures::euclidean_distances() {
	std::vector<double> distances;
	for (int i = 0; i < (popc.ga_pop.size()-1); ++i) {
		for (int j = i +1; j < popc.ga_pop.size(); ++j) {
			distances.push_back(calculate_euclidean_distance(popc.ga_pop[i], popc.ga_pop[j]));
		}
	}

	return calculate(distances);
}

populationMetaFeatures::Statistic populationMetaFeatures::hamming_distances() {
	std::vector<double> distances;
	for (int i = 0; i < (popc.ga_pop.size()-1); ++i) {
		for (int j = i +1; j < popc.ga_pop.size(); ++j) {
			distances.push_back(calculate_hamming_distance(popc.ga_pop[i], popc.ga_pop[j]));
		}
	}

	return calculate(distances);
}

populationMetaFeatures::Statistic populationMetaFeatures::shannon_entropy() {
	std::vector<double> entropies, ones(popc.vector_size, 0);

	for (int i = 0; i < popc.ga_pop.size(); ++i) {
		for (int j = 0; j < popc.vector_size; ++j) {
			ones[j] += popc.ga_pop[i][j];
		}
	}

	for (int i = 0; i < ones.size(); ++i) {
		ones[i] = ones[i] / popc.vector_size;
	}

	for (int i = 0; i < ones.size(); ++i) {
		entropies.push_back(ones[i] * std::log2(ones[i]));
	}

	return calculate(entropies);
}

populationMetaFeatures::Statistic populationMetaFeatures::simpson_diversity_index() {
	std::vector<double> entropies, ones(popc.vector_size, 0);

	for (int i = 0; i < popc.ga_pop.size(); ++i) {
		for (int j = 0; j < popc.vector_size; ++j) {
			ones[j] += popc.ga_pop[i][j];
		}
	}

	for (int i = 0; i < ones.size(); ++i) {
		ones[i] = ones[i] / popc.vector_size;
	}

	for (int i = 0; i < ones.size(); ++i) {
		entropies.push_back(ones[i] * ones[i]);
	}

	return calculate(entropies);
}

populationMetaFeatures::Statistic populationMetaFeatures::probabilities() {
	return calculate(popc.dist.value());
}

void populationMetaFeatures::generate_insert() {
	Statistic euc, ham, sha, simp, prob;

	euc = euclidean_distances();
	ham = hamming_distances();
	sha = shannon_entropy();
	simp= simpson_diversity_index();
	prob= probabilities();

	std::cerr << "insert into population_meta values (\"" << pop_name << "\", "
		<< popc.ga_pop.size() << ", " << popc.vector_size << ", "

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
		<< prob.max << ", " << prob.entropy << ")\n";
}

