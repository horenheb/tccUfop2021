#include "genetic.h"

Genetic::Genetic(int size, Population & population, Distance & distance, std::mt19937 & mt, std::uniform_real_distribution<double> & dist, std::uniform_int_distribution<int> & distInt, int start, double crossProb, double mutaProb)
	: _size(size), _population(population), _distance(distance),_dist(dist), _mt(mt), _distInt(distInt), _start(start), _crossProb(crossProb), _mutaProb(mutaProb)
	{
		
	}
	
std::vector<Route> Genetic::firstGeneration(){

	Route auxRoute(_distance._size, _distance._paths, _mt, _dist, _start);	
	std::vector<Route> routes;
		
	for(int i=0; i<_size; i++){
		auxRoute.shuffle();
		routes.push_back(auxRoute);
	}
	
	return routes;
	
}	

void Genetic::makeTwoChildren(){
	
	double randValue = _dist(_mt);
	
	Route parent1;
	Route parent2;

	do {
		
		parent1 = selectRoute();
		parent2 = selectRoute();
	
	} while(parent1._fitness == parent2._fitness);	
	
	if(randValue < _crossProb) {
		
		crossOverPath(parent1, parent2);
		
		
	} else {

		_nextGenRoutes.push_back(parent1);
		_nextGenRoutes.push_back(parent2);
		
	}
	
}

Route Genetic::nextGeneration(){
	
	Route bestRoute;
	
	if(_population._routes.size() == 0) {

		std::vector<Route> routes = Genetic::firstGeneration();	
		_population.setRoutes(routes);	
		bestRoute = _population._bestRoute;
		
	} else {
		
		do {
			
			makeTwoChildren();
			
		} while (_nextGenRoutes.size() != _population._size);
		
		_population._routes.clear();
		_population.setRoutes(_nextGenRoutes);
		
		if(_population._bestRoute._fitness > bestRoute._fitness){
			
			bestRoute = _population._bestRoute;
			
		}	
			
		_nextGenRoutes.clear();
		
	}
	
	return bestRoute;
	
}

Route Genetic::selectRoute(){
	
	double randValue = _dist(_mt);
	double fitnessTotal = _population.getFitnessTotal();
	double roll = randValue * fitnessTotal; 

	for(int i =0; i<_population._size; i++){
		
		if(roll < _population._routes.at(i)._fitness){
				
			return _population._routes.at(i);
				
		} else {
					
			roll = roll - _population._routes.at(i)._fitness;
		
		}

	}
	
}

void Genetic::crossOverPath(Route parentOne, Route parentTwo){
	
	std::vector<int> childrenOne;
	std::vector<int> childrenTwo;
	
	std::vector<int> pathOne = parentOne._path;
	std::vector<int> pathTwo = parentTwo._path;
	
	int r1;
	int r2;
	
	_distInt = std::uniform_int_distribution<int>(1, pathOne.size()-1);
					
	r1 = _distInt(_mt);
	r2 = _distInt(_mt);
	
	do {
		r1 = _distInt(_mt);
		r2 = _distInt(_mt);		
	} while(r1 >= r2);
	
	childrenOne = orderOneCrossover(r1, r2, pathOne, pathTwo);
	childrenTwo = orderOneCrossover(r1, r2, pathTwo, pathOne);

	parentOne.update(childrenOne);
	parentTwo.update(childrenTwo);
	
	if(_dist(_mt) < _mutaProb){
		parentOne.mutate(0.05, _mt, _dist);
		parentTwo.mutate(0.05, _mt, _dist);
	}	

		
	//push_back children 1: 	
	_nextGenRoutes.push_back(parentOne);
	
	//push_back children 2:
	_nextGenRoutes.push_back(parentTwo);

}

std::vector<int> Genetic::orderOneCrossover(int r1, int r2, std::vector<int> & parent1, std::vector<int> & parent2){
	
		
	//initalize the child
	std::vector<int> child;
	
	for(int i=0; i<parent1.size(); i++) {
		child.push_back(-1);
	}
	
	//copy elements between r1, r2 from parent1 into child 
	for(int i = r1; i<=r2; i++){
		child[i] = parent1[i];
	}
	
	//array to hold elements of parent1 which are not in child yet
	std::vector<int> y; 
	for(int i = 0; i < parent1.size(); i++){
		if(std::find(child.begin(), child.end(), parent1[i]) == child.end()){
			y.push_back(parent1[i]);
		}
	}

	//rotate parent2
	std::vector<int> copy = parent2;
	std::rotate(copy.begin(), copy.begin()+r2+1, copy.end());
	
	//now order the elements in y according to their order in parent2 
	std::vector<int> y1;
	for(int i = 0; i < parent1.size(); i++){
		if(std::find(y.begin(), y.end(), copy[i]) != y.end()){
			y1.push_back(copy[i]);
		}
	}
	
	//now copy the remaining elements (i.e. remaining in parent1) into child
	//according to their order in parent2... starting after r2!
	for(int i = 0; i < y1.size(); i++){
		int ci = (r2 + i + 1) % parent1.size();
		child[ci] = y1[i];
	}
	

	
	return child;
		
}


