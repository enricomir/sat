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
		mhController(SatProblem p);

		popController pops;
		SatProblem p;

			bool save;

		int operator()(unsigned char algo);
};

#endif
