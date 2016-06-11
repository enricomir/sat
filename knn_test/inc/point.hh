#ifndef POINT_HH
#define POINT_HH

#include <string>
#include <sstream>

class Point {
	public:
		float data[52];
		int centroid;

		void read_from(std::string line) {
			std::stringstream ss(line);
			for (int i = 0; i < 52; ++i)
				ss >> data[i];
			ss >> centroid;
		}
};

#endif
