#ifndef ROUTE_H_
#define ROUTE_H_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "distance.h"

class Route {
	
	public:
		double _fitness;
		int _distance;
		int _size;
		int _start;
		std::vector<int> _path;
		std::mt19937 _mt;
		std::uniform_real_distribution<double> _dist;
		std::vector<int> _paths;
		
	public:
		Route(int, std::vector<int> &, std::mt19937 &, std::uniform_real_distribution<double> &, int);
		Route(std::vector<int>, std::vector<int> &, int);
		Route();
		Route(const Route &);
		int getPathValue(int, int);
		int calcDistance();
		std::vector<int> generateBasePath();
		double calcFitness();
		double randomNumber();
		void mutate(double, std::mt19937 &, std::uniform_real_distribution<double> &);
		void localSearch(double, std::mt19937 &, std::uniform_real_distribution<double> &);
		void shuffle();
		void print();
		void update(std::vector<int> path);
		void reload();
			
};

#endif

