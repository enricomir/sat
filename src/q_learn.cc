#include "q_learn.hh"
#include <iostream>
#include <random>
#include <fstream>

void Q_Learn::choose() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0, 1);

	float f = dis(gen);
  
  if (f < sigma) { //Random choice
    std::uniform_int_distribution<> dis2(0,11);
    action = dis2(gen);
  } else { //Best Q Choice
    float max = -1;
    for (int i = 0; i < 12; ++i) {
      if (q_value[state][i] > max) {
        action = i;
        max = q_value[state][i];
      }
    }
  }
}

void Q_Learn::reward(float r, float next_value) {
	//std::cout << "Q_Learn::reward r: " << r << "\n";
	//std::cout << "Q_Learn::reward increased amount: " << lr*(r+next_value*discount-q_value[state][action]) << "\n";
	q_value[state][action] += lr*(r+next_value*discount-q_value[state][action]);
}

void Q_Learn::print() {
	for (int i = 0; i < 12; ++i)
		std::cout << q_value[state][i] << " ";
	std::cout << "\n";
}

void Q_Learn::save(std::string file) {
	std::ofstream of(file);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 12; ++j)
			of << q_value[i][j] << " ";
		of << "\n";
	}
}

void Q_Learn::load(std::string file) {
	std::ifstream of(file);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 12; ++j)
			of >> q_value[i][j];
	}
}

int Q_Learn::best_action() {
	float max = 0;
	float action = -1;
	for (int i = 0; i < 12; ++i) {
		if (q_value[state][i] > max) {
			max = q_value[state][i];
			action = i;
		}
	}
	return action;
}

float Q_Learn::max_reward() {
	float max = 0;
	for (int i = 0; i < 12; ++i) {
		if (q_value[state][i] > max) {
			max = q_value[state][i];
		}
	}
	return max;
}