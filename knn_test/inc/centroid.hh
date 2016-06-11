#ifndef CENTROID_HH
#define CENTROID_HH
#include <string>
#include <sstream>
#include <cmath>
#include "point.hh"

class Centroid {
	public:
		float data[52];

		void read_from(std::string line) {
			std::stringstream ss(line);
			for (int i = 0; i < 52; ++i)
				ss >> data[i];
		}

		float getDist(Point p) {
			float double_dist = 0.0;
			for (int i = 0; i < 52; ++i) {
				float d = data[i] - p.data[i];
				double_dist += (d*d);
			}

			return std::sqrt(double_dist);
		}
};

#endif
