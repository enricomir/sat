#ifndef Q_LEARN_HH
#define Q_LEARN_HH

#include <vector>
#include <string>
#include <algorithm>

class Q_Learn {
	public:
		const float lr = 0.1;
    const float discount = 0.9;
		float sigma = 0.0;

		Q_Learn() {
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 12; j++)
					q_value[i][j] = 0;
		}

		int state;
		int action;

		void choose();
		void reward(float r, float next_value);

		void print();

		void save(std::string file);
		void load(std::string file);
		int best_action();
		float max_reward();

		float q_value[10][12]; //state x action
};

#endif
