#include <iostream>
#include <chrono>
#include <stdexcept>
#include "SatProblem.hh"
#include "BnB.hh"
#include "bitga.h"
#include "binarypso.h"
#include "meta_features.hh"
#include "eda.hh"
#include "popcontroller.hh"

int main(int argc, char** argv) {
	try {
		//ga_main_function(argc, argv);
		//pso_main_function(argc, argv);
		//problem_size(argv[1], argv[2]);
		//balance(argv[1], argv[2]);
		//eda_main_function(argc, argv);
		
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << '\n';
	}
	return 0;
}

void test_popcontroller() {
		popController p(10);
		std::cout << "Created\n" << std::flush;

		std::cout << "Dist: ";
		for (int i = 0; i < p.dist.size(); ++i) {
			std::cout << p.dist.value()[i] << " ";
		}
		std::cout << "\n";

		p.resize(10);
		std::cout << "Resized\n" << std::flush;
		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				std::cout << p.pso_pop[i][j] << "" << p.ga_pop[i][j] << " ";
			}
			std::cout << "\n";
		}


		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				p.pso_pop[i][j] = (i+j)%2;
			}
		}

		std::cout << "Changed PSO pop\n";
		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				std::cout << p.pso_pop[i][j] << "" << p.ga_pop[i][j] << " ";
			}
			std::cout << "\n";
		}

		p.update_from_PSO();

		std::cout << "\n\nFix Pop:\n";
		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				std::cout << p.pso_pop[i][j] << "" << p.ga_pop[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "Dist: ";
		for (int i = 0; i < p.dist.size(); ++i) {
			std::cout << p.dist.value()[i] << " ";
		}
		std::cout << "\n";


		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				p.ga_pop[i][j] = 1-(i+j)%2;
			}
		}

		std::cout << "Changed GA pop\n";
		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				std::cout << p.pso_pop[i][j] << "" << p.ga_pop[i][j] << " ";
			}
			std::cout << "\n";
		}

		p.update_from_GA();

		std::cout << "\n\nFix Pop:\n";
		for (int i = 0; i < p.pso_pop.size(); ++i) {
			for (int j = 0; j < p.pso_pop[i].size(); ++j) {
				std::cout << p.pso_pop[i][j] << "" << p.ga_pop[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "Dist: ";
		for (int i = 0; i < p.dist.size(); ++i) {
			std::cout << p.dist.value()[i] << " ";
		}
		std::cout << "\n\n\n";

		std::cout << p.dist << "\n\n\n" << p.pso_pop << "\n\n\n" << p.ga_pop;
}


int bnb_main(int argc, char* argv[]) {
	if (argc==1) {
		std::cerr << "Error! Usage: ./sat.app <filename>" << std::endl;
		return 1;
	}

	SatProblem p(argv[1]);
	p.printClauses();

	BnB c(p);

	c.solve(std::chrono::seconds(5));
	c.best_so_far.eval();
	std::cout << "c BestVars \n";
	c.best_so_far.printVars();
	std::cout << "c BestMax: " << c.best_so_far.true_clauses << "\n";
	std::cout << "c BestMin: " << c.best_so_far.false_clauses << "\n";
	c.best_so_far.printClauses();
	return 0;
}

