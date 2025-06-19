#ifndef GRAPHFUCNTIONS_H
#define GRAPHFUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <stack>
#include <cmath>

#include "Location.h"
#include "Route.h"
#include "FileOperations.h"

using namespace std;

class Graph{
public:
	vector<Location*> cities;
	vector<Route*> routes;

	int numExists;

	Graph(string nodesFile, string edgesFile);

	int getCityIndex(string key);
	Location* getCity(string key);

	float getWeight(string startS, string endS, bool costOrTime);
	float getWeight(Location* start, Location* end, bool costOrTime);

	void Dijkstras(string startS, bool CostOrTime);

	vector<Location*>* adjacentLocations(Location* city);
	Route* getRoute(Location* start, bool costOrTime, float totalDistance);

	vector<Route*>* adjacentRoutes(Location* city);

	stack<Location*>cityStacker(string destinationS);
	stack<Route*> routeStacker(string destinationS, bool costOrTime);

//-----------------------------------------------------------------------
/*	SCRAPPED METHODS

	Location* getSmallest();
	void printShortestRoute(string destinationS);
	void printOutRoutes();
*/
//-----------------------------------------------------------------------
};

Graph::Graph(string nodesFile, string edgesFile){
	routes = routeParser(edgesFile);
	cities = locationParser(nodesFile, routes);

	numExists = cities.size();

}

int Graph::getCityIndex(string key){
	int output = -1;
	for(int i = 0; i < cities.size(); i++){
		if(cities[i] -> capital.compare(key) == 0){
			output = i;
			break;
		}
	}
	return output;
}

Location* Graph::getCity(string key){
	int i = getCityIndex(key);
	if(i != -1){
		return cities.at(i);
	}
	else{
		return NULL;
	}
}


float Graph::getWeight(string startS, string endS, bool costOrTime){
	Location* start = getCity(startS);
	Location* end = getCity(endS);

	for(int i = 0; i < routes.size(); i++){
		if(routes[i] -> doesConnect(start, end)){
			if(costOrTime){
				return routes[i] -> cost;
			}
			else{
				return routes[i] -> time;
			}
		}
	}

	return -1;
}

float Graph::getWeight(Location* start, Location* end, bool costOrTime){

	for(int i = 0; i < routes.size(); i++){
		if(routes[i] -> doesConnect(start, end)){
			if(costOrTime){
				return routes[i] -> cost;
			}
			else{
				return routes[i] -> time;
			}
		}
	}

	return -1;
}


void Graph::Dijkstras(string startS, bool costOrTime){

	Location* start = getCity(startS);
	start -> lengthFromStart = 0;

	// Reset all cities to unvisited state
	for(int i = 0; i < cities.size(); i++){
		cities[i] -> exists = true;
		if(cities[i] != start) {
			cities[i] -> lengthFromStart = 999999;
			cities[i] -> previous = NULL;
		}
	}

	priority_queue<Location*,vector<Location*>,compareLocation> minHeap;

	for(int i = 0; i < cities.size(); i++){
		minHeap.push(cities[i]);
	}

	while(!minHeap.empty()){

		// Find the unvisited node with minimum distance
		Location* smallest = NULL;
		priority_queue<Location*,vector<Location*>,compareLocation> tempHeap;

		while(!minHeap.empty()){
			Location* current = minHeap.top();
			minHeap.pop();

			if(current -> exists) {
				smallest = current;
				// Put remaining elements back
				while(!minHeap.empty()) {
					tempHeap.push(minHeap.top());
					minHeap.pop();
				}
				minHeap = tempHeap;
				break;
			}
		}

		if(smallest == NULL) {
			return;
		}

		smallest -> exists = false;

		vector<Location*>* adjacentCities = adjacentLocations(smallest);	

		for(int i = 0; i < adjacentCities -> size(); i++){

			Location* adjacent = adjacentCities -> at(i);

			if(adjacent -> exists == true){

				float distance = getWeight(smallest, adjacent, costOrTime) + smallest -> lengthFromStart;

				if(distance < adjacent -> lengthFromStart){
					adjacent -> lengthFromStart = distance;
					adjacent -> previous = smallest;
					minHeap.push(adjacent);
				}
			}
		}

		delete adjacentCities;
	}
}

vector<Location*>* Graph::adjacentLocations(Location* city){

	vector<Location*>* output = new vector<Location*>();

	for(int i = 0; i < city -> routes.size(); i++){
		if(city -> routes[i] -> destination -> exists == true){
			output -> push_back(city -> routes[i] -> destination);
			//cout << i <<" Adjacent: " << city -> routes[i] -> destination -> exists << endl;	//debug
		}
	}

	return output;
}

vector<Route*>* Graph::adjacentRoutes(Location* city){
	vector<Route*>* output = new vector<Route*>();

	for(int i = 0 ; i < routes.size(); i++){
		if(routes[i] -> origin -> capital.compare(city -> capital) == 0){
			output -> push_back(routes[i]);
		}
	}
	return output;
}

Route* Graph::getRoute(Location* start, bool costOrTime, float totalDistance){

	vector<Route*>* routes = adjacentRoutes(start);

	float epsilon = 1e-5;

	for(int i = 0; i < routes -> size(); i++){

			if(costOrTime == true){
				if(fabs((totalDistance - routes -> at(i) -> cost) - routes -> at(i) -> origin -> lengthFromStart) < epsilon){
					return routes -> at(i);
				}
			}

			else if(costOrTime == false){
				if(fabs((totalDistance - routes -> at(i) -> time) - routes -> at(i) -> origin -> lengthFromStart) < epsilon){
					return routes -> at(i);
				}
			}

	}
	return NULL;
}

stack<Location*> Graph::cityStacker(string destinationS){

	Location* destination = getCity(destinationS);
	stack<Location*> output;

	while(destination != NULL){
		output.push(destination);
		destination = destination -> previous;
	}

	return output;

}

stack<Route*> Graph::routeStacker(string destinationS, bool costOrTime){

	stack<Route*> output;
	Location* destination = getCity(destinationS);
	float totalDistance = destination -> lengthFromStart;

	while(destination -> previous != NULL){
		output.push(getRoute(destination -> previous, costOrTime, totalDistance));
		destination = destination -> previous;

		totalDistance = destination -> lengthFromStart;
	}

	return output;
}

/*
Location* Graph::getSmallest(){
	if(cities.size() == 0){
		return NULL;
	}
	int pos = 0;
	Location* output = cities[0];
	for(int i = 1; i < cities.size(); i++){
		Location* current = cities[i];
		if(current -> lengthFromStart < output -> lengthFromStart){
			output = current;
			pos = i;
		}
	}

	cout << "Pos: " << pos << endl;	//debug
	cities[pos + 1] -> exists = false;
	numExists--;
	return output;
}

void Graph::printShortestRoute(string destinationS){

	Location* destination = getCity(destinationS);
	Location* previous = destination;
	float distFromStart = 0;
	while (previous != NULL)
	{
		cout << previous -> capital << " <- ";
		distFromStart += previous -> lengthFromStart;

		previous = previous -> previous;
	}
	cout << endl;
	cout << "Distance from start: " << distFromStart << endl;
}


void Graph::printOutRoutes(){
	for(int i = 0; i < cities.size(); i++){
			cout << "Routes from: " << cities[i] -> capital << endl;
		for(int j = 0; j < cities[i] -> routes.size(); j++){
			cout << "	" << j << ": " << cities[i] -> routes[j] -> destination -> capital << " Type: " << cities[i] -> routes[j] -> transport << " Cost: " << cities[i] -> routes[j] -> cost << " Time: " <<  cities[i] -> routes[j] -> time <<endl;
		}
	}
}
*/

#endif