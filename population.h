#ifndef POPULATION_H_
#define POPULATION_H_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "route.h"
#include "distance.h"

class Population {
	
	public:
		int _size;
		double _fitness;
		std::vector<Route> _routes;
		Route _bestRoute;

	public:
		double getFitnessTotal();
		void setRoutes(std::vector<Route>);
		Route getBestRoute();
		
	
};

#endif

