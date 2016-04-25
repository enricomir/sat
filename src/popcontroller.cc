#include "popcontroller.hh"
#include <fstream>

void popController::resize(const unsigned int new_pop_size) {
	if (pso_pop.size() == new_pop_size)
		return;

	if (pso_pop.size() > new_pop_size) {
		//Shrinks the population
		while (pso_pop.size() > new_pop_size) {
			pso_pop.pop_back();
		}

		update_from_PSO();
		return;
	}

	// Grow the population

	//#1 - positions
	eoUniformGenerator<bool> uGen;
	eoInitFixedLength<Particle> random(vector_size, uGen);
	pso_pop.append(new_pop_size, random);

	//#2 - Velocities + first is best

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
			ones[j]+= pso_pop[i].bestPositions[j];
			ind.push_back(pso_pop[i].bestPositions[j]);
			pso_pop[i][j] = pso_pop[i].bestPositions[j];
		}
		ga_pop.push_back(ind);
	}

	for (int i = 0; i < dist.size(); ++i) {
		if (dist.size() != pso_pop[0].size()) {
			std::cout << "POPCONTROLLER UPDATE FROM PSO STUPID ERROR"
				<< " distsize=" << dist.size() << " psoind=" << pso_pop[0].size() << "\n";
		}
		dist.value()[i] = (ones[i]*1.0)/pso_pop.size();
	}
}

void popController::update_from_GA() {
	std::vector<unsigned int> ones(vector_size, 0);

	//Update PSO POP
	pso_pop.clear();
	for (int i = 0; i < ga_pop.size(); ++i) {
		Particle p;
		for (int j = 0; j < ga_pop[i].size(); ++j) {
			ones[j]+= ga_pop[i][j];
			p.push_back(ga_pop[i][j]);
			//pso_pop[i][j] = ga_pop[i][j];
		}
		pso_pop.push_back(p);
	}

	for (int i = 0; i < dist.size(); ++i) {
		dist.value()[i] = (ones[i]*1.0)/pso_pop.size();
	}
}

void popController::update_from_dist() {
	apply<Indi>(dist, ga_pop);

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

void popController::load(const std::string filename) {
	std::ifstream f(filename+".pop");

	unsigned int vec_size;
	unsigned int pop_size;

	f >> vec_size >> pop_size;

	if (vec_size != vector_size) {
		std::cout << "Different vector sizes on loading, new=" << vec_size
			<< " old=" << vector_size << "\n";
	}

	resize(pop_size);

	for (int i = 0; i < pop_size; ++i) {
		for (int j = 0; j < vec_size; ++j) {
			bool b;
			f >> b;
			ga_pop[i][j] = b;
		}
	}
	update_from_GA();

	for (int i = 0; i < vec_size; ++i)
		f >> dist.value()[i];

}

std::string popController::getUUID() {
		std::ifstream i("/proc/sys/kernel/random/uuid");
		std::string s;
		i >> s;
		return s;
}

