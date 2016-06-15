#ifndef MHCONTROLLER_HH
#define MHCONTROLLER_HH

#include <vector>
#include <eo>
#include <string>

#include <ga.h>
#include <ga/eoBit.h>
#include <ga/eoPBILAdditive.h>

#include <eoEDA.h>
#include <eoSimpleEDA.h>

#include <utils/eoRNG.h>

#include "popcontroller.hh"
#include "SatProblem.hh"

class mhController {
	public:
		mhController(SatProblem problem);

		popController pops;
		SatProblem p;

		int operator()(unsigned int algo);

		void refill_vectors();

		std::vector<eoSGA<Indi> > ga;
		std::vector<eoEasyPSO<Particle>> pso;
		std::vector<eoSimpleEDA<Indi>> eda;

		std::vector<unsigned int> pop_sizes;

		int initial;
		int fin;
	private:
		bool reset;

		//Eval Functions GA
		eoEvalFuncPtr<Indi, double, const Indi&> eval;
		eoEvalFuncCounter<Indi> funccounter;
		eoEvalContinue<Indi> continuator;

		//Eval Functions PSO
		eoEvalFuncPtr<Particle, double, const Particle&> psoeval;
		eoEvalFuncCounter<Particle> pso_counter;
		eoEvalContinue<Particle> pso_continuator;
};

#endif
