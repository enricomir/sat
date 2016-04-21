#ifndef POPCONTROLLER_HH
#define POPCONTROLLER_HH

#include <vector>
#include <eo>
#include <string>

#include <ga.h>
#include <ga/eoBit.h>
#include <ga/eoPBILAdditive.h>

#include <eoEDA.h>
#include <eoSimpleEDA.h>

#include <utils/eoRNG.h>

typedef eoMinimizingFitness FitT;
typedef eoBitParticle<FitT> Particle;
typedef eoBit<FitT> Indi;

class popController {
	public:
		popController(const unsigned int vector_size, const unsigned int pop_size = 0) : vector_size(vector_size), dist(vector_size) { 
			resize(pop_size); 
		};

		const unsigned int vector_size;
		void resize(const unsigned int new_pop_size);

		eoPop<Particle>     pso_pop;
		void update_from_PSO();

		eoPop<Indi>         ga_pop;
		void update_from_GA();

		eoPBILDistrib<Indi> dist;
		void update_from_dist();

		void save(const std::string filename);
		void load(const std::string filename);

		static std::string getUUID();
};

#endif
