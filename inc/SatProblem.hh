#ifndef _SAT_PROBLEM_HH_
#define _SAT_PROBLEM_HH_

#include <string>
#include <vector>
#include <fstream>

class SatProblem {
	public:
		SatProblem(std::string filename);
		std::string name;
		std::vector<bool> variables;
		std::vector<bool> allocated;
		std::vector<std::vector<int>> clauses;
		std::vector<unsigned int> weights;

		int true_clauses;
		int false_clauses;
		int eval();

		void printClauses();
		void printVars();

		unsigned long getTotalClauseItems();

	private:
		void readCNF(std::ifstream& file, const int nclauses);
		void readWCNF(std::ifstream& file, const int nclauses);
};

#endif
