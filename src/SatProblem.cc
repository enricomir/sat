#include "SatProblem.hh"
#include <iostream>
#include <sstream>

SatProblem::SatProblem(std::string filename) {
	std::cout << "c Opening file " << filename << "\n";
	std::ifstream in(filename);
	std::string str;
	while (std::getline(in, str)) {
		std::istringstream iss(str);

		char c;
		iss >> c;
		if (c=='c') continue;
		if (c=='p') {
			std::string mode;
			int var, clauses, top;
			iss >> mode;
			iss >> var;
			iss >> clauses;

			for (int i = 0; i < var; ++i) {
				variables.push_back(false);
			}

			if (mode==std::string("cnf")) {
				std::cout << "c CNF file var=" << var << " clauses=" << clauses << "\n";
				readCNF(in, clauses);
				return;
			}

		}
		std::cout << str << "\n";
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

void SatProblem::printClauses() {
	int i = 0;
	for (auto c: clauses) {
		std::cout << "c Clause " << i++ << ": ";
		for (auto n: c) {
			std::cout << n << " ";
		}
		std::cout << "\n";
	}
}
