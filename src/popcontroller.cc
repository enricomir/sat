#include "popcontroller.hh"
#include <fstream>

void popController::resize(const unsigned int new_pop_size) {
	if (pso_pop.size() == new_pop_size)
		return;

	if (pso_pop.size() > new_pop_size) {
		//Shrinks the population
		for (unsigned int i = 0; i < (pso_pop.size() - new_pop_size); ++i) {
			pso_pop.pop_back();
		}

		update_from_PSO();
		return;
	}

	// Grow the population
	eoUniformGenerator<bool> uGen;
	eoInitFixedLength<Particle> random(vector_size, uGen);
	pso_pop.append(new_pop_size, random);

	ga_pop.clear();
	for (int i = 0; i < pso_pop.size(); ++i) {
		Indi ind;
		for (int j = 0; j < pso_pop[i].size(); ++j) {
			ind.push_back(pso_pop[i][j]);
		}
		ga_pop.push_back(ind);
	}
}

void popController::update_from_PSO() {
	std::vector<unsigned int> ones(vector_size, 0);

	//Update GA POP
	ga_pop.clear();
	for (int i = 0; i < pso_pop.size(); ++i) {
		Indi ind;
		for (int j = 0; j < pso_pop[i].size(); ++j) {
			ones[j]+= pso_pop[i][j];
			ind.push_back(pso_pop[i][j]);
		}
		ga_pop.push_back(ind);
	}

	for (int i = 0; i < dist.size(); ++i) {
		dist.value()[i] = (ones[i]*1.0)/pso_pop.size();
	}
}

void popController::update_from_GA() {
	std::vector<unsigned int> ones(vector_size, 0);

	//Update PSO POP
	for (int i = 0; i < ga_pop.size(); ++i) {
		for (int j = 0; j < ga_pop[i].size(); ++j) {
			ones[j]+= ga_pop[i][j];
			pso_pop[i][j] = ga_pop[i][j];
		}
	}

	for (int i = 0; i < dist.size(); ++i) {
		dist.value()[i] = (ones[i]*1.0)/pso_pop.size();
	}
}

void popController::update_from_dist() {
	for (int i = 0; i < ga_pop.size(); ++i) {
		for (int j = 0; j < ga_pop[i].size(); ++j) {
			pso_pop[i][j] = ga_pop[i][j];
		}
	}
}

void popController::save(const std::string filename) {
	std::ofstream f(filename+".pop");
	f << vector_size << " " << ga_pop.size() << "\n";

	for (int i = 0; i < ga_pop.size(); ++i) {
		for (int j = 0; j < ga_pop[i].size(); ++j) {
			f << ga_pop[i][j] << " ";
		}
		f << "\n";
	}

	for (int i = 0; i < dist.size(); ++i)
		f << dist.value()[i] << " ";
	f << "\n";
}

popController popController::load(const std::string filename) {
	std::ifstream f(filename+".pop");

	unsigned int vec_size;
	unsigned int pop_size;

	f >> vec_size >> pop_size;

	popController p(vec_size, pop_size);

	for (int i = 0; i < pop_size; ++i)
		for (int j = 0; j < vec_size; ++j) {
			bool b;
			f >> b;
			p.ga_pop[i][j] = b;
		}
	p.update_from_GA();

	for (int i = 0; i < vec_size; ++i)
		f >> p.dist.value()[i];

	return p;
}

std::string popController::getUUID() {
		std::ifstream i("/proc/sys/kernel/random/uuid");
		std::string s;
		i >> s;
		return s;
}

