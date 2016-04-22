#include "mhcontroller.hh"

#include <eoSecondsElapsedContinue.h>
#include <apply.h>

namespace eo {
	extern eoRng rng;
}

using eo::rng;
static std::string filename;
static SatProblem* gp;

static double ga_eval(const Indi& _indi) {
	SatProblem& p = (*gp);
	for (int i = 0; i < _indi.size(); ++i) {
		p.variables[i] = _indi[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

static double pso_eval(const Particle & _particle) {
	SatProblem& p = (*gp);
	for (int i = 0; i < _particle.size(); ++i) {
		p.variables[i] = _particle[i];
		p.allocated[i] = true;
	}

	p.eval();
	return p.false_clauses;
}

mhController::mhController(SatProblem problem) : p(problem), pops(problem.variables.size()) {
	gp = &p;
	refill_vectors();
}

void mhController::refill_vectors() {
	const unsigned int max_ofc = 2000;

	static bool reset = false;
	static eoEvalFuncPtr<Indi, double, const Indi&> eval(ga_eval);
	static eoEvalFuncCounter<Indi> funccounter(eval);
	static eoEvalContinue<Indi> continuator(funccounter, max_ofc);
	funccounter.value(0);

	static eoEvalFuncPtr<Particle, double, const Particle&> psoeval(pso_eval);
	static eoEvalFuncCounter<Particle> pso_counter(psoeval);
	static eoEvalContinue<Particle> pso_continuator(pso_counter, max_ofc);
	pso_counter.value(0);

	if (reset)
		return;
	reset = true;

	//Fills GA's
	{
		//GA #1 - Winner
		{
			static eoDetTournamentSelect<Indi> select(5);
			static eo1PtBitXover<Indi> xover;
			static eoDetBitFlip<Indi> mutation(0.01 * p.variables.size());
			eoSGA<Indi> gga(select, xover, 0.6, mutation, 0.05, funccounter, continuator);
			ga.push_back(gga);
			pop_sizes.push_back(50);
		}

		//GA #2 - Winner #2
		{
			static eoStochTournamentSelect<Indi> select(0.7);
			static eoNPtsBitXover<Indi> xover;
			static eoBitMutation<Indi> mutation(0.01);
			eoSGA<Indi> gga(select, xover, 0.8, mutation, 0.9, funccounter, continuator);
			ga.push_back(gga);
			pop_sizes.push_back(70);
		}

		//GA #3 - Exploratory
		{
			static eoStochTournamentSelect<Indi> select(0.6);
			static eo1PtBitXover<Indi> xover;
			static eoDetBitFlip<Indi> mutation(0.1 * p.variables.size());
			eoSGA<Indi> gga(select, xover, 0.6, mutation, 0.25, funccounter, continuator);
			ga.push_back(gga);
			pop_sizes.push_back(500);
		}

		//GA #4 - Exploiter
		{
			static eoDetTournamentSelect<Indi> select(5);
			static eo1PtBitXover<Indi> xover;
			static eoBitMutation<Indi> mutation(0.01);
			eoSGA<Indi> gga(select, xover, 0.8, mutation, 0.75, funccounter, continuator);
			ga.push_back(gga);
			pop_sizes.push_back(50);
		}
	}
	
	//Fills PSO's
	{
		// Initializer common
		static eoUniformGenerator<double> sgen(-1, 1);
		static eoVelocityInitFixedLength<Particle> velorandom(pops.vector_size, sgen);
		static eoFirstIsBestInit<Particle> localinit;

		static eoSigBinaryFlight <Particle> flight;
		static eoRealVectorBounds bnds(pops.vector_size, -1.5, 1.5);

		//PSO #1 - Winner
		{
			static eoLinearTopology<Particle> top(3);

			static eoStandardVelocity<Particle> velocity(top, 1.0, 1.7, 2.3, bnds);

			static eoInitializer<Particle> fullinit(psoeval,velorandom,localinit,top,pops.pso_pop);

			eoEasyPSO<Particle> ppso(fullinit, pso_continuator, pso_counter, velocity, flight);

			pso.push_back(ppso);
			pop_sizes.push_back(20);
		}

		//PSO #2 - Winner #2
		{
			static eoLinearTopology<Particle> top(5);

			static eoStandardVelocity<Particle> velocity(top, 1.0, 1.3, 1.9, bnds);

			static eoInitializer<Particle> fullinit(psoeval,velorandom,localinit,top,pops.pso_pop);

			eoEasyPSO<Particle> ppso(fullinit, pso_continuator, pso_counter, velocity, flight);

			pso.push_back(ppso);
			pop_sizes.push_back(50);
		}

		//PSO #3 - Exploratory
		{
			static eoRingTopology<Particle> top(2);

			static eoStandardVelocity<Particle> velocity(top, 1.0, 1.0, 1.0, bnds);

			static eoInitializer<Particle> fullinit(psoeval,velorandom,localinit,top,pops.pso_pop);

			eoEasyPSO<Particle> ppso(fullinit, pso_continuator, pso_counter, velocity, flight);

			pso.push_back(ppso);
			pop_sizes.push_back(250);
		}

		//PSO #4 - Exploiter
		{
			//static eoStarTopology<Particle> top; TODO: Investigate
			static eoLinearTopology<Particle> top(49);

			static eoStandardVelocity<Particle> velocity(top, 1.0, 2.0, 2.5, bnds);

			static eoInitializer<Particle> fullinit(psoeval,velorandom,localinit,top,pops.pso_pop);

			eoEasyPSO<Particle> ppso(fullinit, pso_continuator, pso_counter, velocity, flight);

			pso.push_back(ppso);
			pop_sizes.push_back(50);
		}
	}

	//Fills PBIL's
	{
		//EDA #1 - Winner
		{
			static eoPBILAdditive<Indi> du(0.1, 1, 0.01, 0.01, 0);
			eoSimpleEDA<Indi> eeda(du, funccounter, 50, continuator);

			eda.push_back(eeda);
			pop_sizes.push_back(50);
		}

		//EDA #2 - Winner #2
		{
			static eoPBILAdditive<Indi> du(0.05, 1, 0.05, 0.01, 0);
			eoSimpleEDA<Indi> eeda(du, funccounter, 50, continuator);

			eda.push_back(eeda);
			pop_sizes.push_back(50);
		}

		//EDA #3 - Exploratory
		{
			static eoPBILAdditive<Indi> du(0.1, 5, 0.30, 0.01, 5);
			eoSimpleEDA<Indi> eeda(du, funccounter, 200, continuator);

			eda.push_back(eeda);
			pop_sizes.push_back(200);
		}

		//EDA #4 - Exploiter
		{
			static eoPBILAdditive<Indi> du(0.1, 1, 0.01, 0.01, 0);
			eoSimpleEDA<Indi> eeda(du, funccounter, 30, continuator);

			eda.push_back(eeda);
			pop_sizes.push_back(30);
		}

	}
}

int mhController::operator()(unsigned char algo) {
	refill_vectors();
	static eoEvalFuncPtr<Indi, double, const Indi&> eval(ga_eval);
	static eoEvalFuncPtr<Particle, double, const Particle&> psoeval(pso_eval);
	pops.resize(pop_sizes[algo]);
	apply<Indi>(eval, pops.ga_pop);
	pops.ga_pop.sort();
	int initial = pops.ga_pop[0].fitness();
	int fin = -1;

	if (algo < ga.size()) { // Runs a GA
		ga[algo](pops.ga_pop);
		pops.update_from_GA();
		pops.ga_pop.sort();
		fin = pops.ga_pop[0].fitness();
	} else if ( algo < (ga.size() + pso.size()) ) { // Runs PSO - to test
		pso[algo-ga.size()](pops.pso_pop);
		pops.update_from_PSO();
		pops.pso_pop.sort();
		fin = pops.pso_pop[0].best();
	} else if (algo < (ga.size() + pso.size() + eda.size()) ) {
		eda[algo-ga.size()-pso.size()](pops.dist);
		pops.update_from_dist();
		apply<Indi>(eval, pops.ga_pop);
		pops.ga_pop.sort();
		fin = pops.ga_pop[0].fitness();
	} else {
		std::cout << "mhController::operator(): asked for an heuristic non-existent\n";
		exit(-1);
	}

	std::cout << "Initial: " << initial << " / Final: " << fin << " Pop=" << pops.ga_pop.size() << " " << pops.pso_pop.size() << "\n";
	//return initial-fin;
	return fin;
}

