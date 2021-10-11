#include "route.h"

double Route::calcFitness(){
	return (double) 1 / _distance; 
}

int Route::getPathValue(int row, int col){
	
	int a = _paths.at((row * _size) + col);
	int b = _paths.at((col * _size) + row);
	
	//(a < b) ? return a : return b;
	
	return (a < b) ? a : b;
	
}

void Route::update(std::vector<int> path){
	_path = path;
	_distance = calcDistance();
	_fitness = calcFitness();	
}

void Route::reload(){
	_distance = calcDistance();
	_fitness = calcFitness();
}

Route::Route(std::vector<int> path, std::vector<int> & paths, int start) {
	_path = path;
	_paths = paths;
	_start = start;
	_size = path.size();
	_distance = calcDistance();
	_fitness = calcFitness();
}

Route::Route() {
	_path = {};
	_paths = {};
	_start = 0;
	_size = 0;
	_distance = 0;
	_fitness = 0;
}

int Route::calcDistance(){

	int i;
	int row = _start;
	int column = _path.at(0);
	int accum = getPathValue(row,column);

	for(i=1; i < _path.size(); i++){
		
		row = _path.at(i-1);
		column = _path.at(i);
		accum = accum + getPathValue(row,column);
	
	}
		
	row = _path.at(_path.size()-1);
	column = _start;
	accum = accum + getPathValue(row,column);
	
	return accum;
		
}

double Route::randomNumber(){
	return _dist(_mt);
}

void Route::mutate(double amount, std::mt19937 & mt2, std::uniform_real_distribution<double> & dist2){
	
	int loopSize = amount * _size;
	std::vector<int> memory;
	int randInd;

	for (int i = 0; i < loopSize; i++) {
		
		double randomValue;
		
		do {
			
			randomValue = dist2(mt2);
			randInd = floor(randomValue * _path.size());
			
		} while (std::find(memory.begin(), memory.end(), randInd) != memory.end());
		
		memory.push_back(randInd);
			
		int tmp = _path[randInd];
		_path[randInd] = _path[i];
		_path[i] = tmp;
			
	}
	
}

void Route::shuffle(){
	
	std::shuffle(_path.begin(), _path.end(), _mt);	
	_distance = calcDistance();
	_fitness = calcFitness();
	
}

void Route::print(){
	
	for(int i=0; i<_path.size(); i++){
		std::cout << _path.at(i) << " ";
	}
	std::cout <<std::endl;
	
}

std::vector<int> Route::generateBasePath(){
	
	std::vector<int> tmpPath;
	
	for(int i=0; i<_size; i++){
		if(i!=_start){
			tmpPath.push_back(i);
		}
	}
	
	return tmpPath;
}

Route::Route(int size, std::vector<int> & paths, std::mt19937 & mt, std::uniform_real_distribution<double> & dist, int start)
	
	: _paths(paths), _start(start), _mt(mt), _dist(dist), _size(size){
			
		_path = generateBasePath();
		_distance = calcDistance();
		_fitness = calcFitness();

	}

Route::Route(const Route & route){
	
	_fitness = route._fitness;
	_distance = route._distance;
	_size = route._size;
	_start = route._start;
	_path = route._path;
	_mt = route._mt;
	_dist = route._dist;
	_paths = route._paths;
	
}

void Route::localSearch(double amount, std::mt19937 & mt2, std::uniform_real_distribution<double> & dist2){
		
	int loopSize = amount * _size;
	std::vector<int> memory;
	int randInd;

	for (int i = 0; i < loopSize; i++) {
		
		double randomValue;
		
		do {
			
			randomValue = dist2(mt2);
			randInd = floor(randomValue * _path.size());
			
		} while (std::find(memory.begin(), memory.end(), randInd) != memory.end());
		
		memory.push_back(randInd);
			
		int tmp = _path[randInd];
		_path[randInd] = _path[i];
		_path[i] = tmp;
			
	}
	
	
}




