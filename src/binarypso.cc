//-----------------------------------------------------------------------------
// BinaryPSO.cpp
//-----------------------------------------------------------------------------
//*
// An instance of a VERY simple Real-coded binary Particle Swarm Optimization Algorithm
//
//-----------------------------------------------------------------------------
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <sqlite3.h>

#include "binarypso.h"
#include "SatProblem.hh"

#include <eoSecondsElapsedContinue.h>

// Use functions from namespace std
using namespace std;

std::string myfilename;
std::string problem_category;
std::string problem_name;

double binary_value (const Particle & _particle)
{
	SatProblem p(myfilename);
	for (int i = 0; i < _particle.size(); ++i) {
		p.variables[i] = _particle[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

void save(
		double result, 
		unsigned int n_size, 
		unsigned int pop,
		double inertia, 
		double l1, 
		double l2) {

	std::string query;
	query  = "insert into pso values(\"";
	query += problem_category;

	query += "\",\"";
	query += problem_name;

	query += "\", \"linear\", ";

	query += to_string(n_size);
	query += ", ";

	query += to_string(pop);
	query += ", ";

	query += to_string(inertia);
	query += ", ";

	query += to_string(l1);
	query += ", ";

	query += to_string(l2);
	query += ", ";

	query += to_string(result);
	query += ")";

	cout << "\n" << query << "\n";

	sqlite3* db;
	while (sqlite3_open("results/results.db", &db)!=SQLITE_OK) ;

	sqlite3_stmt* stmt;
	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);

	while (sqlite3_step(stmt)!=SQLITE_DONE);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void run_pso(SatProblem& p)
{
	const unsigned int VEC_SIZE = p.variables.size();
	const unsigned int POP_SIZE = 20;
	const unsigned int NEIGHBORHOOD_SIZE= 3;

	const double VELOCITY_INIT_MIN= -1;
	const double VELOCITY_INIT_MAX= 1;

	const double VELOCITY_MIN= -1.5;
	const double VELOCITY_MAX= 1.5;

	const double INERTIA= 1;
	const double LEARNING_FACTOR1= 1.7;
	const double LEARNING_FACTOR2= 2.3;

	//rng.reseed(SEED);

	// population <=> swarm
	eoPop<Particle> pop;

	// Evaluation: from a plain C++ fn to an EvalFunc Object
	eoEvalFuncPtr<Particle, double, const Particle& > eval(  binary_value );

	// ring topology
	//eoRingTopology<Particle> topology(NEIGHBORHOOD_SIZE);
	eoLinearTopology<Particle> topology(NEIGHBORHOOD_SIZE);

	// position initialization
	eoUniformGenerator<bool> uGen;
	eoInitFixedLength < Particle > random (VEC_SIZE, uGen);
	pop.append (POP_SIZE, random);

	// velocities initialization component
	eoUniformGenerator < double >sGen (VELOCITY_INIT_MIN, VELOCITY_INIT_MAX);
	eoVelocityInitFixedLength < Particle > veloRandom (VEC_SIZE, sGen);

	eoFirstIsBestInit < Particle > localInit;

	eoInitializer <Particle> fullInit(eval,veloRandom,localInit,topology,pop);

	fullInit();

	eoRealVectorBounds bnds(VEC_SIZE,VELOCITY_MIN,VELOCITY_MAX);

	eoStandardVelocity <Particle> velocity (topology,INERTIA,LEARNING_FACTOR1,LEARNING_FACTOR2,bnds);

	eoSigBinaryFlight <Particle> flight;

	eoSecondsElapsedContinue<Particle> genCont(15);

	eoEasyPSO<Particle> pso(genCont, eval, velocity, flight);
	pso(pop);

	pop.sort();
//	cout << myfilename << " " << pop[0].best() << "\n";

	save(
		pop[0].best(), 
		NEIGHBORHOOD_SIZE,
		POP_SIZE,
		INERTIA,
		LEARNING_FACTOR1,
		LEARNING_FACTOR2);
}

void pso_main_function(int argc, char** argv) {
	myfilename = "dat/cnf/";
	myfilename += argv[1];
	myfilename += "/";
	myfilename += argv[2];

	problem_category = argv[1];
	problem_name = argv[2];

	SatProblem p(myfilename);
	run_pso(p);
}
