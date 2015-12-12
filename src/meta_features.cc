#include "meta_features.hh"
#include "SatProblem.hh"
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <cmath>

using std::to_string;

struct Statistic {
	double avg;
	double stddev;
	double vc;
	double min;
	double max;
	double entropy;
};
Statistic calculate(std::vector<double> v);
using my_size = unsigned long int;

void problem_size(std::string category, std::string problem) {
	std::string filename = "dat/cnf/";
	filename += category;
	filename += "/";
	filename += problem;

	SatProblem p(filename);
	
	unsigned int c = p.clauses.size();
	unsigned int v = p.variables.size();

	std::string query = "insert into meta_problem_size (category, problem, clauses, variables) values (\'";
	query += (category + "\',");
	query += ("\'" + problem + "\',");
	query += (to_string(c)+",");
	query += (to_string(v)+")");

	std::cout << query << "\n";
	
	sqlite3* db;
	while (sqlite3_open("results/results.db", &db)!=SQLITE_OK) ;

	sqlite3_stmt* stmt;
	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);
	while (sqlite3_step(stmt)!=SQLITE_DONE);
	sqlite3_finalize(stmt);
	
/*	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);
	while (sqlite3_step(stmt)!=SQLITE_DONE);
	sqlite3_finalize(stmt);*/

	sqlite3_close(db);
}

void balance(std::string category, std::string problem) {
	std::string filename = "dat/cnf/";
	filename += category;
	filename += "/";
	filename += problem;
	SatProblem p(filename);

	std::cout << "Balance: open file " << filename << "\n";

	// Positive to negative ratio
	std::vector<double> pos_to_neg_ratio;
	
	int i = 0;
	for (auto c: p.clauses) {
		double positive = 0.0, negative = 0.0;
		for (auto i: c) {
			if (i>0) {
				positive++;
			} else {
				negative++;
			}
		}
		pos_to_neg_ratio.push_back(positive/(positive+negative));
	}

	std::cout 
		<< "Read pos to negative clauses (size " << pos_to_neg_ratio.size() <<
		"), start statistics.\n";
	Statistic pos_to_neg = calculate(pos_to_neg_ratio);

	//Variable positive and negative statistics
	std::cout << "Start variable balance\n";
	std::vector<my_size> positive_var, negative_var;
	for (auto i: p.variables) {
		positive_var.push_back(0);
		negative_var.push_back(0);
	}
	std::cout << "  Finish setting up\n";

	for (auto c: p.clauses) {
		for (auto i: c) {
			if (i>0) {
				positive_var[i  -1]++;
			} else {
				negative_var[-i -1]++;
			}
		}
	}

	std::cout << "  Read, starting to create ratio\n";

	std::vector<double> var_ratio;
	for (unsigned long int i = 0; i < p.variables.size(); ++i) {
		//Divide by 0 error
		if (negative_var[i] == 0)
			negative_var[i] = 100000;

		var_ratio.push_back(static_cast<double>(positive_var[i])/negative_var[i]);
	}

	std::cout << "  Finish ratio, start statistics\n";
	Statistic  var = calculate(var_ratio);

	//Unary, binary, ternary
	double unary, binary, ternary;
	for (auto c: p.clauses) {
		if (c.size() == 1)
			unary++;
		else if (c.size() == 2)
			binary++;
		else if (c.size() == 3)
			ternary++;
	}
	std::cout << "Finish unary binary ternary\n";

	unary   =   unary/p.clauses.size();
	binary  =  binary/p.clauses.size();
	ternary = ternary/p.clauses.size();

	//Add to database
	std::string query = 
		"insert into meta_balance values(";

	query += ("\'" + category + "\',");
	query += ("\'" + problem  + "\',");

	query += (to_string(pos_to_neg.avg) + ",");
	query += (to_string(pos_to_neg.stddev) + ",");
	query += (to_string(pos_to_neg.vc) + ",");
	query += (to_string(pos_to_neg.min) + ",");
	query += (to_string(pos_to_neg.max) + ",");
	query += (to_string(pos_to_neg.entropy) + ",");

	query += (to_string(var.avg) + ",");
	query += (to_string(var.stddev) + ",");
	query += (to_string(var.vc) + ",");
	query += (to_string(var.min) + ",");
	query += (to_string(var.max) + ",");
	query += (to_string(var.entropy) + ",");

	query += (to_string(unary) + ",");
	query += (to_string(binary) + ",");
	query += (to_string(ternary) + ")");

	std::cout << "Opening DB\n";
	sqlite3* db;
	while (sqlite3_open("results/results.db", &db)!=SQLITE_OK) ;
	std::cout << "  DB Open\n";
	std::cout << query << "\n";

	sqlite3_stmt* stmt;
	while (sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL)!=SQLITE_OK);
	std::cout << "  Statement prepared\n";
	while (sqlite3_step(stmt)!=SQLITE_DONE);
	std::cout << "  Step statement\n";
	sqlite3_finalize(stmt);
	std::cout << "  Finalize statement\n";

	sqlite3_close(db);
}

Statistic calculate(std::vector<double> v) {
	Statistic ret;
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	ret.avg = sum/v.size();
	std::cout << "  Finish average " << ret.avg << "\n";

	double acc = 0.0;
	std::for_each(v.begin(), v.end(), 
			[&acc,ret](const double d) { acc += (d-ret.avg)*(d-ret.avg); } );
	ret.stddev = std::sqrt(acc / v.size());
	std::cout << "  Finish stddev " << ret.stddev << "\n";

	ret.vc = ret.stddev / ret.avg;
	std::cout << "  Finish vc " << ret.vc << "\n";

	ret.min = v[0];
	std::for_each(v.begin(), v.end(),
			[&ret](const double d) { ret.min = (ret.min > d? d : ret.min); } );
	std::cout << "  Finish min " << ret.min << "\n";

	ret.max = v[0];
	std::for_each(v.begin(), v.end(),
			[&ret](const double d) { ret.max = (d > ret.max ? d : ret.max); } );
	std::cout << "  Finish max " << ret.max << "\n";

	ret.entropy = 0.0;
	std::for_each(v.begin(), v.end(),
			[&ret,sum](const double d) { 
				double p_i = d/sum;
				if (p_i != 0)
					ret.entropy += p_i*std::log(p_i);
			} );
	std::cout << "  Finish entropy " << ret.entropy << "\n";

	return ret;
}

