#ifndef POPCONTROLLER_HH
#define POPCONTROLLER_HH

#include <vector>
#include <eo>

#include <ga.h>
#include <ga/eoBit.h>
#include <ga/eoPBILAdditive.h>

#include <eoEDA.h>
#include <eoSimpleEDA.h>

#include <utils/eoRNG.h>

typedef eoMinimizingFitness FitT;
typedef eoBitParticle<FitT> Particle;
typedef eoBit<FitT> Indi;

//TODO Must put evals!

class popController {
	public:
		popController(const unsigned int vector_size = 0) : vector_size(vector_size), dist(vector_size) { } ; //TODO: implement initialization

		const unsigned int vector_size;
		void resize(const unsigned int new_pop_size);

		eoPop<Particle>     pso_pop;
		void update_from_PSO();

		eoPop<Indi>         ga_pop;
		void update_from_GA();

		eoPBILDistrib<Indi> dist;
		void update_from_dist();

};

#endif
