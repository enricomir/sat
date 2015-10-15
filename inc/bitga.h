#ifndef BITGA_H_
#define BITGA_H_

#include "SatProblem.hh"
#include <eo>
#include <ga.h>

//Individual type: Bitstring
typedef eoBit<double> Indi;

double maxsat(const Indi& _indi);
double minsat(const Indi& _indi);
void run_ga(const unsigned int seed, SatProblem& p);
void ga_main_function(int argc, char** argv);

#endif

