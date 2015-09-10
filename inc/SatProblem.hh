#ifndef _SAT_PROBLEM_HH_
#define _SAT_PROBLEM_HH_

#include <string>
#include <vector>
#include <fstream>

class SatProblem {
	public:
		SatProblem(std::string filename);
		std::vector<bool> variables;
		std::vector<std::vector<int>> clauses;
		std::vector<unsigned int> weights;

		void printClauses();

	private:
		void readCNF(std::ifstream& file, const int nclauses);
		void readWCNF(std::ifstream& file, const int nclauses);
};

#endif
