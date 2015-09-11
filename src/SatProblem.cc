#include "SatProblem.hh"
#include <iostream>
#include <sstream>
#include <numeric>

SatProblem::SatProblem(std::string filename) {
	std::cout << "c Opening file " << filename << "\n";
	std::ifstream in(filename);
	std::string str;
	while (std::getline(in, str)) {
		std::istringstream iss(str);

		char c;
		iss >> c;

		//First char is C -> comment line
		if (c=='c') continue;

		//First char is P -> problem specification
		if (c=='p') {

			//Universal problem variables
			std::string mode;
			int var, clauses, top;
			iss >> mode;
			iss >> var;
			iss >> clauses;

			//Create clauses
			for (int i = 0; i < var; ++i) {
				variables.push_back(false);
			}

			//Check for CNF (unweighted maxsat) mode
			if (mode==std::string("cnf")) {
				std::cout << "c CNF file var=" << var << " clauses=" << clauses << "\n";
				readCNF(in, clauses);
				return;
			}

			//Tries to read TOP value.
			//If top value exists, problem is PMS (partial maxsat)
			//or WPMS (weighted partial maxsat). If not, just common weighted maxsat
			if (iss >> top) {
				std::cout << "tem top\n";
			} else {
				std::cout << "c WCNF file var=" << var << " clauses=" << clauses << "\n";
				readWCNF(in, clauses);
			}

		}
	}
}

void SatProblem::readCNF(std::ifstream& file, const int nclauses) {
	std::string str;
	for (int i = 0; i < nclauses; ++i) {
		std::getline(file, str);
		std::istringstream iss(str);

		clauses.emplace_back();
		int n;
		iss >> n;
		while (n != 0) {
			clauses[i].push_back(n);
			iss >> n;
		}
	}
}

void SatProblem::readWCNF(std::ifstream& file, const int nclauses) {
	std::string str;
	for (int i = 0; i < nclauses; ++i) {
		std::getline(file, str);
		std::istringstream iss(str);

		unsigned int w;
		iss >> w;
		if (i!=weights.size()) {
			std::cerr << "error! " << w << "\n";
			std::cerr << str;
		}
		weights.push_back(w);

		clauses.emplace_back();
		int n;
		iss >> n;
		while (n != 0) {
			clauses[i].push_back(n);
			iss >> n;
		}
	}
}

void SatProblem::printClauses() {
	int i = 0;
	for (auto c: clauses) {
		std::cout << "c Clause " << i << ": ";
		if (weights.size() != 0)
			std::cout << "(" << weights[i] << ") ";
		for (auto n: c) {
			std::cout << n << " ";
		}
		std::cout << "\n";
		++i;
	}
}

unsigned long SatProblem::getTotalClauseItems() {
	return std::accumulate(clauses.begin(), clauses.end(), 0,
			[](unsigned long sum, const std::vector<int>& clause) {
				return sum + clause.size();
				}
			);
}

