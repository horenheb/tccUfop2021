#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <mysql.h>
#include <string>
#include <chrono>

#include "distance.h"
#include "route.h"
#include "population.h"
#include "genetic.h"

#include "annealing.h"




/* run this program using the console pauser or add your own getch, system("pause") or input loop */

std::vector<int> split(const std::string & s, char delimiter) {  
   
    std::vector<int> tokens;     
    std::string token;     
    std::istringstream tokenStream(s); 
	    
    while (getline(tokenStream, token, delimiter)) { 
	
        tokens.push_back(stoi(token));     
    }     
    return tokens;  
}

std::vector<int> readFromFile(std::string filePath){
	
	std::string line;
	std::ifstream myfile (filePath);
	std::string text; 
	std::vector<int> myVector;
	
	if (myfile.is_open()){
		
		while (getline(myfile,line)){
			text = text + line;		
		}
		
		myfile.close();
		
	} else {
		std::cout << "Unable to open file";	
	}
	
	myVector = split(text,' ');
	
	return myVector;
	
}

void dropTable(std::string tableName, MYSQL * connection){
	
	std::string query;
	const char * q;

	query = "DROP TABLE IF EXISTS " + tableName + ";";
	q = query.c_str();
	auto qstate = mysql_query(connection, q);
	if(qstate) std::cout << "FATAL ERROR: Drop table fail!" << std::endl;
	
}


std::string createTableResults(MYSQL * connection){
		
	dropTable("results", connection);
	
	std::string  query;

	query = 		"CREATE TABLE IF NOT EXISTS `results` ("; 
    query =	query + "`id` int not null primary key auto_increment, ";
	query =	query + "`step` bigint(20) DEFAULT NULL, ";
	query =	query + "`distance` bigint(20) DEFAULT NULL, ";
	query = query + "`comments` varchar(64) DEFAULT NULL) ";
	query = query + "ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;";
	
	return query;
}

std::string createTableResultsShort(MYSQL * connection){
		
	dropTable("resultsShort", connection);
	
	std::string  query;

	query = 		"CREATE TABLE IF NOT EXISTS `resultsShort` ("; 
    query =	query + "`id` int not null primary key auto_increment, ";
	query =	query + "`distance` bigint(20) DEFAULT NULL, ";
	query =	query + "`time` bigint(20) DEFAULT NULL, ";
	query = query + "`comments` varchar(64) DEFAULT NULL) ";
	query = query + "ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;";
	
	return query;
}

std::string createTableResultsShortAG(MYSQL * connection){
		
	dropTable("resultsShortAG", connection);
	
	std::string  query;

	query = 		"CREATE TABLE IF NOT EXISTS `resultsShortAG` ("; 
    query =	query + "`id` int not null primary key auto_increment, ";
	query =	query + "`distance` bigint(20) DEFAULT NULL, ";
	query =	query + "`time` bigint(20) DEFAULT NULL, ";
	query = query + "`comments` varchar(64) DEFAULT NULL) ";
	query = query + "ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;";
	
	return query;
}

void insertLineResults(MYSQL *connection, int step, int distance, std::string comments){

	std::string query;
	const char * q;
	
	query = "INSERT INTO results (step, distance, comments) VALUES (";
	query = query + std::to_string(step) + ",  ";
	query = query + std::to_string(distance) + ",  ";  
	query = query + "\"" + comments + "\"" + "); ";
	
	q = query.c_str();
	auto qstate = mysql_query(connection, q);
	
	if(qstate) std::cout << "FATAL ERROR: Loading fom CSV fail! " << mysql_error(connection) << std::endl;
	
}

void insertLineResultsShort(MYSQL *connection, int distance, int time, std::string comments){

	std::string query;
	const char * q;
	
	query = "INSERT INTO resultsShort (distance, time, comments) VALUES (";
	query = query + std::to_string(distance) + ",  ";  
	query = query + std::to_string(time) + ",  ";  
	query = query + "\"" + comments + "\"" + "); ";
	
	q = query.c_str();
	auto qstate = mysql_query(connection, q);
	
	if(qstate) std::cout << "FATAL ERROR: Loading fom CSV fail! " << mysql_error(connection) << std::endl;
	
}


std::string createTableResultsAG(MYSQL * connection){
		
	dropTable("resultsAG", connection);
	
	std::string  query;

	query = 		"CREATE TABLE IF NOT EXISTS `resultsAG` ("; 
    query =	query + "`id` int not null primary key auto_increment, ";
	query =	query + "`step` bigint(20) DEFAULT NULL, ";
	query =	query + "`distance` bigint(20) DEFAULT NULL, ";
	query = query + "`comments` varchar(64) DEFAULT NULL) ";
	query = query + "ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;";
	
	return query;
}

void insertLineResultsShortAG(MYSQL *connection, int distance, int time, std::string comments){

	std::string query;
	const char * q;
	
	query = "INSERT INTO resultsShortAG (distance, time, comments) VALUES (";
	query = query + std::to_string(distance) + ",  ";  
	query = query + std::to_string(time) + ",  ";  
	query = query + "\"" + comments + "\"" + "); ";
	
	q = query.c_str();
	auto qstate = mysql_query(connection, q);
	
	if(qstate) std::cout << "FATAL ERROR: Loading fom CSV fail! " << mysql_error(connection) << std::endl;
	
}


void insertLineResultsAG(MYSQL *connection, int step, int distance, std::string comments){

	std::string query;
	const char * q;
	
	query = "INSERT INTO resultsAG (step, distance, comments) VALUES (";
	query = query + std::to_string(step) + ",  ";
	query = query + std::to_string(distance) + ",  ";  
	query = query + "\"" + comments + "\"" + "); ";
	
	q = query.c_str();
	auto qstate = mysql_query(connection, q);
	
	if(qstate) std::cout << "FATAL ERROR: Loading fom CSV fail! " << mysql_error(connection) << std::endl;
	
}


int main(int argc, char** argv) {
	
	MYSQL * connection; 
	MYSQL * response; 
	MYSQL_ROW row;
	
	std::string query;
	const char * q;
	
	connection = mysql_init(0);
	connection = mysql_real_connect(connection, "localhost", "root", "", "algoresults", 0, NULL, 0);
	
	if(!connection) {
		std::cout << "Connection failed.\n";
		return 0;
	}
	
	//Random seed original: 2 e 1000.
	
	
 	int MATRIX_MIN 			= 1;
 	int MATRIX_MAX 			= 100;
 	int RANDOM_SEED 		= 187;	//187, 234, 999, 763, 566, 288843, 4231, 7432, 1234, 766622, para AG, selecionar o seed igual a 2 para melhorar o resultado.
 	
 	std::vector<int> randomSeed;
 	
 	randomSeed.push_back(1000);
 	randomSeed.push_back(234);
 	randomSeed.push_back(999);
 	randomSeed.push_back(763);
 	randomSeed.push_back(566);
 	randomSeed.push_back(288);
 	randomSeed.push_back(423);
 	randomSeed.push_back(766);
 	randomSeed.push_back(872);
 	randomSeed.push_back(222);
 	
 	
 	for(int wed=0; wed < randomSeed.size(); wed++){
	 	
	 	
	 	
	 	double MUTATION_PROB 	= 0.2;
	 	double MUTATION_SIZE	= 0.2;
	 	
	 	int POPULATION_SIZE 	= 300;
	 	
	 	int MATRIX_SIZE 		= 100;
	 	
	 	std::string instance = "FST_" +  std::to_string(randomSeed.at(wed)) + "xF";
	 	
		std::mt19937 								GENERATOR(randomSeed.at(wed));
		std::uniform_int_distribution<int> 			INT_DISTRIBUITION(MATRIX_MIN, MATRIX_MAX);
		std::uniform_real_distribution<double> 		REAL_DISTRIBUITION(0, 1);
		
		//std::vector<int> 							pathDistanceVector; 
		//pathDistanceVector = readFromFile("geleia.txt");
		//Distance distance(pathDistanceVector);
	
		Distance distance(MATRIX_SIZE, GENERATOR, INT_DISTRIBUITION);
		
	
		
		// Create table results AG. 
		//query = createTableResultsAG(connection);
		//q = query.c_str();
		//auto qstate = mysql_query(connection, q);
		
		// Create table results AG short. 
		//query = createTableResultsShortAG(connection);
		//q = query.c_str();
		//qstate = mysql_query(connection, q);
		
		//if(qstate) {
			//std::cout << mysql_error(connection) << std::endl;
			//std::cout << "Error in creating MySql Table: results." << std::endl;
			//return 0;
		//}
		
		std::vector<double> crossProbAlpha;
		std::vector<double> mutationAlpha;
		std::vector<int> populationAlpha;
		
		crossProbAlpha.push_back(0.20);
		crossProbAlpha.push_back(0.22);
		crossProbAlpha.push_back(0.24);
	
		mutationAlpha.push_back(0.01);
		mutationAlpha.push_back(0.02);
		mutationAlpha.push_back(0.03);
		
		populationAlpha.push_back(300);
			
		for(int w=0; w<crossProbAlpha.size(); w++){
			
			std::mt19937 								GENERATOR(randomSeed.at(wed));
			std::uniform_int_distribution<int> 			INT_DISTRIBUITION(MATRIX_MIN, MATRIX_MAX);
			std::uniform_real_distribution<double> 		REAL_DISTRIBUITION(0, 1); 
		
			std::cout << "AG - Etapa " << w + 1 << " de " << crossProbAlpha.size() << "." << std::endl;
			
			for(int b=0; b<populationAlpha.size(); b++){
				
				for(int f=0; f<mutationAlpha.size(); f++){
			
					Population population;
					Genetic genetic(populationAlpha.at(b), population, distance, GENERATOR, REAL_DISTRIBUITION, INT_DISTRIBUITION, 0, crossProbAlpha.at(w), mutationAlpha.at(f));
				
					Route TheBestGA;
					Route CurrentGA;
					
					auto start = std::chrono::high_resolution_clock::now();
		
					for(int i=0; i<2000; i++){
						
						if(i==0){
							CurrentGA = TheBestGA;		
						}
						CurrentGA = genetic.nextGeneration();
						
						if(CurrentGA._fitness > TheBestGA._fitness){
							TheBestGA = CurrentGA;
						}
						
						//detailed version
						//std::string comments = std::to_string(crossProbAlpha.at(w)) + "|" + std::to_string(mutationAlpha.at(f))+ "|" + std::to_string(populationAlpha.at(b)) + "|" + instance;
						//insertLineResultsAG(connection, i, TheBestGA._distance, comments);
						
					}
					
					auto end = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);	
					int ms = duration.count();	
					
	
					std::string comments = std::to_string(crossProbAlpha.at(w)) + "|" + std::to_string(mutationAlpha.at(f))+ "|" + std::to_string(populationAlpha.at(b)) + "|" + instance;	
					insertLineResultsShortAG(connection, TheBestGA._distance, ms, comments);
					
				}
			}
		
		}
		
		
		
		
		
		
		
		// Create table results. 
		//query = createTableResults(connection);
		//q = query.c_str();
		//qstate = mysql_query(connection, q);
		
		// Create table results short.
		//query = createTableResultsShort(connection);
		//q = query.c_str();
		//qstate = mysql_query(connection, q);
	
		std::vector<double> alphaArray;	
		std::vector<double> temperatureArray;
		std::vector<int> nArray;	
		
		alphaArray.push_back(0.99);
		alphaArray.push_back(0.98);
		alphaArray.push_back(0.97);
		
		nArray.push_back(2500);
		
		temperatureArray.push_back(1.00);
		temperatureArray.push_back(0.50);
		temperatureArray.push_back(10.00);
			
		for(int k=0; k<alphaArray.size(); k++){
			
			std::mt19937 								GENERATOR(randomSeed.at(wed));
			std::uniform_int_distribution<int> 			INT_DISTRIBUITION(MATRIX_MIN, MATRIX_MAX);
			std::uniform_real_distribution<double> 		REAL_DISTRIBUITION(0, 1);
			
			std::cout << "RS - Etapa " << k + 1 << " de " << alphaArray.size() << "." << std::endl;
	
			double delta;
			
			Route nextRoute;
			Route TheBest;
			Route Current;
			
			for(int w=0; w<temperatureArray.size(); w++){
	
				double temperature = temperatureArray.at(w);
				Route myRoute(distance._size, distance._paths, GENERATOR, REAL_DISTRIBUITION, 0);
				myRoute.shuffle();
				TheBest = myRoute;
		
				for(int p=0; p<nArray.size(); p++){
					
					auto start = std::chrono::high_resolution_clock::now();
					
					for(int j =0; j<2000; j++){ // AQUI 2000
						
						for(int i=0; i<(nArray.at(p)); i++){
							
							nextRoute = myRoute;			
							nextRoute.localSearch(0.15, GENERATOR, REAL_DISTRIBUITION);
							nextRoute.reload();
							
							Current = nextRoute;
								
							delta = nextRoute._distance - myRoute._distance;
							
							if(delta<0){
								
								myRoute = nextRoute;
								
								if(Current._distance < TheBest._distance){
									TheBest = Current;					
								}	
								
							} else {
								
								if(REAL_DISTRIBUITION(GENERATOR) < exp((-(1/delta))/temperature)){
									myRoute = nextRoute;
									
								} 
								
							}
							
						}
						
						temperature = temperature * alphaArray.at(k);
						
						//detailed version
						//std::string comments = std::to_string(alphaArray.at(k)) + "|" + std::to_string(temperatureArray.at(w)) + "|" + std::to_string(nArray.at(p)) + "|" + instance;
						//insertLineResults(connection, j, TheBest._distance, comments);
					
					}
					
					std::string comments = std::to_string(alphaArray.at(k)) + "|" + std::to_string(temperatureArray.at(w)) + "|" + std::to_string(nArray.at(p)) + "|" + instance;
					auto end = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					int ms = duration.count();		
					
					insertLineResultsShort(connection, TheBest._distance, ms, comments);
	
				}
			
			}
			
				
		}
	
	
	
	}
	
	
	
	return 0;                
}


