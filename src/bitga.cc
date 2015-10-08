#include <stdexcept>
#include <iostream>
#include <strstream>
#include "SatProblem.hh"

#include <eo>
#include <ga.h>
#include <utils/eoRNG.h>
#include <eoSecondsElapsedContinue.h>

namespace eo {
	extern eoRng rng;
}

using namespace std;
using eo::rng;

typedef eoBit<double> Indi; //Bitstring, fitness double

//Fitness function - computes number of ones in a bitstring
double bynary_value(const Indi& _indi) {
	double sum=0;
	for (unsigned i=0; i<_indi.size(); ++i)
		sum += _indi[i];
	return sum;
}

std::string filename;
double maxsat(const Indi& _indi) {
	SatProblem p(filename);
	for (int i = 0; i < _indi.size(); ++i) {
		p.variables[i] = _indi[i];
		p.allocated[i] = true;
	}

	return p.eval();
}

double minsat(const Indi& _indi) {
	SatProblem p(filename);
	for (int i = 0; i < _indi.size(); ++i) {
		p.variables[i] = _indi[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

void main_function(int argc, char** argv) {
	//Parameters hard-coded
	
	filename = argv[1];
	SatProblem p(filename);

	const unsigned int SEED = 42;     //Seed
	const unsigned int T_SIZE = 3;    //Tournament size
	const unsigned int VEC_SIZE = p.variables.size();  //Number of bits in genotype
	const unsigned int POP_SIZE = 100; //Pop Size
	const unsigned int MAX_GEN = 100; //Max gen to stop
	const     float CROSS_RATE=0.8;   //Crossover rate
	const double P_MUT_PER_BUT = 0.01;//Probability bitflip
	const float MUT_RATE       = 1.0; //Mutation rate

	rng.reseed(SEED); // Seed rng not-randomly

	//eoEvalFuncPtr<Indi> eval(bynary_value); //EvalFunc object
	eoEvalFuncPtr<Indi> eval(maxsat); //EvalFunc object

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

	//Sort a populacao
	pop.sort();

	//cout << "Initial pop: "<<endl;
	//cout << pop;

	//Robust tournament selection type
	eoDetTournamentSelect<Indi> select(T_SIZE); //T_SIZE tem que estar entre 2[POP_SIZE]

	//Variation operators
	//
	//1pt crossover
	eo1PtBitXover<Indi> xover;
	//Mutation
	eoBitMutation<Indi> mutation(P_MUT_PER_BUT);

	//Termination condition
	//eoGenContinue<Indi> continuator(MAX_GEN);
	eoSecondsElapsedContinue<Indi> continuator(60);

	//StardardGA = SGA
	eoSGA<Indi> gga(select, xover, CROSS_RATE, mutation, MUT_RATE, eval, continuator);

	gga(pop); //APLICA O SGA!

	pop.sort();
	//cout << "Final Pop:\n" << pop << endl;
	cout << filename << " " << maxsat(pop[0]) << "\n";
}

int main(int argc, char** argv) {
	try {
		main_function(argc,argv);
	} catch (exception& e) {
		cout << "Exception: " << e.what() << '\n';
	}
	return 1;
}

