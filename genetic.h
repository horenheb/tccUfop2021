#ifndef GENETIC_H_
#define GENETIC_H_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "route.h"
#include "distance.h"
#include "population.h"

class Genetic {
	
	public:
		int _generation;
		int _start;
		int _size;
		double _crossProb;
		double _mutaProb;
		Population _population;
		Distance _distance;
		std::mt19937 _mt;
		std::uniform_real_distribution<double> _dist;
		std::uniform_int_distribution<int> _distInt;
		std::vector<Route> _nextGenRoutes;
				
	public:
		Genetic(int, Population &, Distance &, std::mt19937 &, std::uniform_real_distribution<double> &, std::uniform_int_distribution<int> &, int, double, double);
		Route selectRoute();
		void crossOverPath(Route, Route);
		std::vector<int> orderOneCrossover(int, int, std::vector<int> &, std::vector<int> &);
		void makeTwoChildren();
		
		//work
		Route nextGeneration();
		std::vector<Route> firstGeneration();
	
};


#endif

