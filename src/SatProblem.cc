#include "SatProblem.hh"
#include <iostream>
#include <sstream>
#include <numeric>

SatProblem::SatProblem(std::string filename) 
	: true_clauses(0), false_clauses(0) {
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
			nvar = var;
			iss >> clauses;

			//Create variables
			for (int i = 0; i < var; ++i) {
				variables = dynamic_bitset<>(var*2);
				allocated = dynamic_bitset<>(var*2);
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

		clauses.emplace_back(nvar*2);
		int n;
		iss >> n;
		while (n != 0) {
			int abs = n>0 ? n : -n;
			int sign = n>0? 0 : 1;
			abs--;
			clauses[i][abs*2+sign] = 1;
			iss >> n;
		}
	}
}

void SatProblem::readWCNF(std::ifstream& file, const int nclauses) {
	/* TODO: future fix for bitset
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
	*/
}

void SatProblem::printClauses() {
	int i = 0;
	for (auto c: clauses) {
		std::cout << "c Clause " << i << ": ";
		if (weights.size() != 0) {
			std::cout << "(" << weights[i] << ") ";
		}

		std::cout << c << "\n";
		++i;
	}
}

unsigned long SatProblem::getTotalClauseItems() {
	/* TODO return std::accumulate(clauses.begin(), clauses.end(), 0,
			[](unsigned long sum, const std::vector<int>& clause) {
			return sum + clause.size();
			}
			);*/
	return 0;
}

void SatProblem::set(int var, bool value) {
#ifdef DEBUG_SAT_SET
	std::cout << "c Setting var=" << var << " / value=" << value 
						<< " nvar=" << nvar << "\n";
	std::cout << "c vars size=" << variables.size() 
						<< "/alloc=" << allocated.size() << "\n";
#endif
	variables[var*2]=value;
	variables[var*2+1]=!value;
	allocated[var*2]=true;
	allocated[var*2+1]=true;
	eval();
}

void SatProblem::unset(int var) {
	variables[var*2]=false;
	variables[var*2+1]=false;
	allocated[var*2]=false;
	allocated[var*2+1]=false;
}

int SatProblem::eval() {
	int sat = 0;
	int unsat = 0;
	for (auto c: clauses) {
		bool false_clause = true;

		dynamic_bitset<> res = variables & c;

#ifdef DEBUG_SAT_EVAL
		std::cout << "--------------------------\n"
			        << "c var=" << variables << "\n"
							<< "c cls=" << c << "\n"
							<< "c res=" << res << "\n";
#endif

		if (res.any()) {
			sat++;
			continue;
		}
		res = c & (~allocated);
		if (!res.any()) unsat++;
	}
	true_clauses = sat;
	false_clauses = unsat;
	return sat;
}

void SatProblem::reset() {
	variables.reset();
	allocated.reset();
}

void SatProblem::printVars() {
	// TODO
	/*
	std::cout << "c Vars: ";
	for (int i = 0; i < variables.size(); ++i) {
		if (allocated[i]) {
			std::cout << (variables[i]?"":"-") << i+1 << " ";
		}
	}
	std::cout << "\n"; */
}
