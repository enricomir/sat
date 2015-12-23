#include "eda.hh"
#include <stdexcept>
#include <iostream>
#include <strstream>

#include <apply.h>

#include <ga/eoBit.h>
#include <ga.h>
#include <ga/eoPBILAdditive.h>

#include <utils/eoRNG.h>
#include <eoSecondsElapsedContinue.h>

#include <string>
#include <sqlite3.h>

using std::to_string;
std::string filename_eda;
std::string pro_category_eda;
std::string pro_name_eda;

double maxsat_eda(const Indi& _indi) {
	SatProblem p(filename_eda);
	for (int i = 0; i < _indi.size(); ++i) {
		p.variables[i] = _indi[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

void save (
		double result,
		unsigned int pop,
		double lrBest,
		unsigned int nBest,
		double lrWorst,
		unsigned int nWorst,
		double tolerance) 
{
	std::string query;
	query  = "insert into eda values(\"";
	query += pro_category_eda;

	query += "\",\"";
	query += pro_name_eda;
	query += "\"";

	query += ", ";
	query += to_string(pop);

	query += ", ";
	query += to_string(lrBest);

	query += ", ";
	query += to_string(nBest);

	query += ", ";
	query += to_string(lrWorst);

	query += ", ";
	query += to_string(nWorst);

	query += ", ";
	query += to_string(tolerance);

	query += ", ";
	query += to_string(result);
	query += ")";

	//cout << query << "\n";

	sqlite3* db;
	while (sqlite3_open("results/results.db", &db)!=SQLITE_OK) ;

	sqlite3_stmt* stmt;
	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);

	while (sqlite3_step(stmt)!=SQLITE_DONE);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void run_eda(
		SatProblem& p, unsigned int pop_size, 
		double lrBest,  unsigned int nBest, 
		double lrWorst, unsigned int nWorst,	double tolerance) {

	const unsigned int VEC_SIZE = p.variables.size();  //Number of bits in genotype
	eoEvalFuncPtr<Indi, double, const Indi&> eval(maxsat_eda); //EvalFunc object

	eoSecondsElapsedContinue<Indi> continuator(15);

	eoPBILAdditive<Indi> du(lrBest, nBest, tolerance, lrWorst, nWorst);

	eoSimpleEDA<Indi> eda(du, eval, pop_size, continuator);

	eoPBILDistrib<Indi> dist(VEC_SIZE);
	eda(dist);

	eoPop<Indi> pop(pop_size, dist);
	apply<Indi>(dist, pop);
	apply<Indi>(eval, pop);

	pop.sort();

	save(
			pop[0].fitness(),
			pop_size,
			lrBest,
			nBest,
			lrWorst,
			nWorst,
			tolerance
			);
}

void eda_main_function(int argc, char** argv) {
	filename_eda = "dat/cnf/";
	filename_eda += argv[1];
	filename_eda += "/";
	filename_eda += argv[2];

	pro_category_eda = argv[1];
	pro_name_eda = argv[2];

	SatProblem p(filename_eda);
	
	const unsigned int POP = 50;
	const unsigned int nBest = 1;
	const double lrBest = 0.05;
	const unsigned int nWorst = 1;
	const double lrWorst = 0.05;
	const double tolerance = 0.1;

	//run_eda(p, POP, lrBest, nBest, lrWorst, nWorst, tolerance);
	run_eda(p, 50, 0.05, 1, 0.01, 0, 0.05);
	run_eda(p, 50, 0.1, 1, 0.01, 0, 0.01);
	run_eda(p, 70, 0.10, 5, 0.10, 5, 0.1);
	run_eda(p, 70, 0.05, 5, 0.10, 0, 0.1);
	run_eda(p, 30, 0.05, 5, 0.05, 5, 0.1);
	run_eda(p, 200, 0.05, 10, 0.05, 10, 0.1);
}
