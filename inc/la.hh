#ifndef LA_HH
#define LA_HH

#include <vector>
#include <string>

class LA {
	public:
		const float lr = 0.01;

		LA() {
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 12; j++)
					probabilities[i][j] = 1.0/12;
		}

		int state;
		int action;

		void choose();
		void reward(unsigned int r);

		void print();

		void save(std::string file);
		void load(std::string file);

		float probabilities[10][12]; //state x action

		float get_convergence();
};



#endif
