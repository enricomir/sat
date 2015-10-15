#ifndef BINARYPSO_H_
#define BINARYPSO_H_

#include <eo>

typedef eoMaximizingFitness FitT;
typedef eoBitParticle < FitT > Particle;

double binary_value(const Particle& _particle);
void pso_main_function(int argc, char ** argv);

#endif
