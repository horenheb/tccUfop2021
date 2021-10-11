#include "population.h"

void Population::setRoutes(std::vector<Route> routes){
	
	_routes = routes;
	_size = routes.size();
	_fitness = getFitnessTotal();
	_bestRoute = getBestRoute();
	
}

double Population::getFitnessTotal(){
	
	double accum = 0;	
	for(int i=0; i<_size; i++){
		accum = accum + _routes.at(i)._fitness;
	}
	return accum;
		
}


Route Population::getBestRoute(){
	
	double maxFit = _routes.at(0)._fitness;


	Route bestRoute;
	
		
	
	for(int i=1; i<_routes.size(); i++){
		
		if(_routes.at(i)._fitness > maxFit){
			
			//std::cout << maxFit << std::endl;
			maxFit = _routes.at(i)._fitness;
			bestRoute = _routes.at(i);
			
		}
		
	}
	
	return bestRoute;
}

