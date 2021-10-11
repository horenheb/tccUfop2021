#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <iostream>
#include <vector>
#include <random>
#include <numeric>  
#include <cmath>

class Distance {
	
	public:
		
		int _size;
		int _total;
		std::vector<int> _paths;
		std::mt19937 _mt;
		std::uniform_int_distribution<int> _dist;
		
	public:
		
		Distance(int, std::mt19937 &, std::uniform_int_distribution<int>);
		Distance(const Distance &);
		Distance(std::vector<int>);
		void makePaths();
		void print();
		int getTotal();
		int getPathValue(int, int);
		void setPathValue(int, int, int);
		void printAsMatrix();
		void makeSymmetric();
		
};

#endif
