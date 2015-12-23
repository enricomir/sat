#include <stdexcept>
#include <iostream>
#include <strstream>
#include "SatProblem.hh"
#include "bitga.h"

#include <utils/eoRNG.h>
#include <eoSecondsElapsedContinue.h>
#include <ga/eoBitOp.h>

#include <string>
#include <sqlite3.h>

namespace eo {
	extern eoRng rng;
}

using namespace std;
using eo::rng;


std::string filename;
std::string pro_category;
std::string pro_name;

double maxsat(const Indi& _indi) {
	SatProblem p(filename);
	for (int i = 0; i < _indi.size(); ++i) {
		p.variables[i] = _indi[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

void save(
		double result, 
		unsigned int pop,
		double xover_rate, 
		std::string xover_method,
		double mut_rate,
		std::string mut_method,
		double mut_param,
		std::string sel_method,
		double sel_param
		) {

	std::string query;
	query  = "insert into ga values(\"";
	query += pro_category;

	query += "\",\"";
	query += pro_name;
	query += "\"";

	query += ", ";
	query += to_string(pop);

	query += ", \"";
	query += xover_method;
	query += "\"";

	query += ", ";
	query += to_string(xover_rate);

	query += ", ";
	query += to_string(mut_rate);

	query += ", \"";
	query += mut_method;
	query += "\"";

	query += ", ";
	query += to_string(mut_param);

	query += ", \"";
	query += sel_method;
	query += "\"";

	query += ", ";
	query += to_string(sel_param);

	query += ", ";
	query += to_string(result);
	query += ")";

	cout << query << "\n";

	sqlite3* db;
	while (sqlite3_open("results/results.db", &db)!=SQLITE_OK) ;

	sqlite3_stmt* stmt;
	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);

	while (sqlite3_step(stmt)!=SQLITE_DONE);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void run_ga(SatProblem& p) {
	//const unsigned int T_SIZE = 3;    //Tournament size
	const double T_SIZE = 0.7;
	const unsigned int VEC_SIZE = p.variables.size();  //Number of bits in genotype
	const unsigned int POP_SIZE = 70; //Pop Size
	const     float CROSS_RATE=0.8;   //Crossover rate
	const double P_MUT_PER_BIT = 0.01;//Probability bitflip - number bit flipped
	const float MUT_RATE       = 0.9; //Mutation rate

	eoEvalFuncPtr<Indi, double, const Indi&> eval(maxsat); //EvalFunc object

	eoPop<Indi> pop; //Population
	for (unsigned int igeno=0; igeno<POP_SIZE; igeno++) {
		Indi v; //Void individual
		for (unsigned ivar=0; ivar < VEC_SIZE; ivar++) {
			bool r = rng.flip(); //New random 0,1
			v.push_back(r);
		}
		//Terminou de criar o cara
		eval(v); //Avalia
		pop.push_back(v); //Poe na pop total
	}
	
	//Robust tournament selection type
	//eoDetTournamentSelect<Indi> select(T_SIZE); //T_SIZE tem que estar entre 2[POP_SIZE]
	//Roulette
	//eoProportionalSelect<Indi> select;
	//Stochastic Tournament
	eoStochTournamentSelect<Indi> select(T_SIZE);
	

	//Variation operators
	//
	//1pt crossover
	//eo1PtBitXover<Indi> xover;
	eoNPtsBitXover<Indi> xover;
	//Mutation
	eoBitMutation<Indi> mutation(P_MUT_PER_BIT);
	//eoDetBitFlip<Indi> mutation(P_MUT_PER_BIT * VEC_SIZE);

	//Termination condition
	//eoGenContinue<Indi> continuator(MAX_GEN);
	eoSecondsElapsedContinue<Indi> continuator(15);

		//StardardGA = SGA
		eoSGA<Indi> gga(select, xover, CROSS_RATE, mutation, MUT_RATE, eval, continuator);
		gga(pop); //APLICA O SGA!
	
	pop.sort();

	save(
			pop[0].fitness(),
			POP_SIZE,
			CROSS_RATE,
			"2pt xover",
			MUT_RATE,
			"bitflip",
		  P_MUT_PER_BIT,
			"stoch_tourn",
			T_SIZE
			);
}

void ga_main_function(int argc, char** argv) {
	filename = "dat/cnf/";
	filename += argv[1];
	filename += "/";
	filename += argv[2];

	pro_category = argv[1];
	pro_name = argv[2];

	SatProblem p(filename);
	run_ga(p);
}

