#ifndef EDA_H_
#define EDA_H_

#include "SatProblem.hh"
#include <eo>
#include <ga/eoBit.h>
#include <ga.h>
#include <eoEDA.h>
#include <eoSimpleEDA.h>

typedef eoMinimizingFitness FitT;
typedef eoBit<FitT> Indi;

double maxsat_eda(const Indi& _indi);
void run_eda(SatProblem& p, 
		unsigned int pop_size, 
		double lrBest,  unsigned int nBest,
		double lrWorst, unsigned int nWorst,
		double tolerance);
void eda_main_function(int argc, char** argv);

#endif
