#include <iostream>
#include <fstream>
#include "point.hh"
#include "centroid.hh"
#include <vector>

void calc(std::string clusters, std::string out) {
	std::ifstream iclusters(clusters);
	Centroid c;
	std::vector<Centroid> centroids;
	std::string str;
	while (std::getline(iclusters, str)) {
		c.read_from(str);
		centroids.push_back(c);
	}

	double dists = 0.0;
	unsigned int n = 0;

	std::ifstream iout(out);
	Point p;
	while (std::getline(iout, str)) {
		p.read_from(str);
		n++;
		dists += centroids[p.centroid].getDist(p);
	}

	std::cout << dists/n;
}

int main(int argc, char** argv) {
	std::cout << argv[3];
	calc(argv[1], argv[2]);
	std::cout << "\n";
	return 0;
}
