#ifndef _SAT_PROBLEM_HH_
#define _SAT_PROBLEM_HH_

#include <string>
#include <vector>
#include <fstream>
#include "dynamic_bitset.hpp"

using boost::dynamic_bitset;

class SatProblem {
	public:
		SatProblem(std::string filename);

		int nvar;
		void set(int var, bool value);
		void unset(int var);
		void reset();

		std::vector<dynamic_bitset<>> clauses;
		std::vector<unsigned int> weights;

		int true_clauses;
		int false_clauses;

		void printClauses();
		void printVars();

		unsigned long getTotalClauseItems();

	private:
		dynamic_bitset<> variables;
		dynamic_bitset<> allocated;

		int eval();

		void readCNF(std::ifstream& file, const int nclauses);
		void readWCNF(std::ifstream& file, const int nclauses);
};

#endif
