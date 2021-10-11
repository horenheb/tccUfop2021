#include "distance.h"

int Distance::getTotal(){
	
	return std::accumulate(_paths.begin(), _paths.end(), 0);
	
}

Distance::Distance(int size, std::mt19937 & mt, std::uniform_int_distribution<int> dist)
	: _size(size), _mt(mt), _dist(dist){
		makePaths();
		_total = getTotal();
	}
	
Distance::Distance(std::vector<int> paths) {
	
	_paths = paths;
	_size = sqrt(paths.size());
	_total = getTotal();
} 
		
Distance::Distance(const Distance & distance) {
	_size = distance._size;
	_mt = distance._mt;
	_dist = distance._dist;
	_paths = distance._paths;
	_total = distance._total;
}

void Distance::makePaths(){
		
	for(int i=0, aux=0; i<(_size*_size); i++){
		
		if(i!=((_size*aux)+aux) && i != 0){
			_paths.push_back(_dist(_mt));
		} else {
			_paths.push_back(0);
			aux++;
		}

	}
	
	makeSymmetric();
	
}

void Distance::print(){
	
	for(int i=0; i<_paths.size(); i++){		
		std::cout << _paths.at(i);
	}
	
}


int Distance::getPathValue(int row, int col){
	return _paths.at((row * _size) + col);
}

void Distance::setPathValue(int row, int col, int value){
	_paths.at((row * _size) + col) = value;
}

void Distance::printAsMatrix(){
	
	for(int i=0; i<_size; i++){		
		for(int j=0; j<_size; j++){		
			std::cout << getPathValue(i,j) << " ";
		}
		std::cout << std::endl;
	}
	
}

void Distance::makeSymmetric(){
	
	for(int i =0; i<_size; i++){
		for(int j=0; j<_size;j++){	
			if(i<j){
				setPathValue(i,j, getPathValue(j,i));	
			}
		}
	}
	
}




/*
void lower(int matrix[3][3], int row, int col)
{
    int i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            if (i < j)
            {
                cout << "0" << " ";
            }
            else
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}*/


