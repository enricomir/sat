#include "la.hh"
#include <iostream>
#include <random>
#include <fstream>

void LA::choose() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0, 1);

	float f = dis(gen);

	action = 0;

	while (f > probabilities[state][action]) {
		f -= probabilities[state][action];
		action++;
	}
}

void LA::reward(unsigned int r) {
	probabilities[state][action] *= (1+lr*r);
	float sum = 0.0;
	for (int i = 0; i < 12; ++i)
		sum += probabilities[state][i];

	for (int i = 0; i < 12; ++i)
		probabilities[state][i] = probabilities[state][i]/sum;
}

void LA::print() {
	for (int i = 0; i < 12; ++i)
		std::cout << probabilities[state][i] << " ";
	std::cout << "\n";
}

void LA::save(std::string file) {
	std::ofstream of(file);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 12; ++j)
			of << probabilities[i][j] << " ";
		of << "\n";
	}
}

void LA::load(std::string file) {
	std::ifstream of(file);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 12; ++j)
			of >> probabilities[i][j];
	}
}

float LA::get_convergence() {
	float ret = 1.0;
	for (int i = 0; i < 10; ++i) {
		float max_chance = 0;
		for (int j = 0; j < 12; ++j)
			if (probabilities[i][j] > max_chance)
				max_chance = probabilities[i][j];

		if (ret > max_chance)
			ret = max_chance;
	}

	return ret;
}


