#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <stack>
#include <algorithm>

#include "FileOperations.h"
#include "Route.h"
#include "GraphFunctions.h"

using namespace std;

int main(int argc, char* argv[]){

	string citiesFilename;
	string routesFilename;
	string outputFilename;
	string origin;
	string destination;
	string preference;
	bool biPreference;

	if(argc > 1){
		citiesFilename = argv[1];
	}

	else{
		cout << "Enter filename containing cities: ";
		cin >> citiesFilename;
	}

	if(argc > 2){
		routesFilename  = argv[2];
	}
	
	else{
		cout << "Enter filename containing routes: ";
		cin >> routesFilename;
	}

	if(argc > 3){
		outputFilename = argv[3];
	}

	else{
		cout << "Enter filename for output (.html): ";
		cin >> outputFilename;
	}

	if(argc > 4){
		origin = argv[4];
	}

	else{
		cout << "Origin: ";
		cin >> origin;
	}

	if(argc > 5){
		destination = argv[5];
	}

	else{
		cout << "Destination: ";
		cin >> destination;
	}

	if(argc > 6){
		preference = argv[6];
	}

	else{
		cout << "Enter a preference (fastest/cheapest): ";
		cin >> preference;
	}

	if(preference.compare("cheapest") == 0){
		biPreference = true;
	}
	else if(preference.compare("fastest") == 0){
		biPreference = false;
	}

	else{
		cout << "Invalid entry";
		return 0;
	}

	Graph graph(citiesFilename, routesFilename);

	if(graph.getCity(origin) == NULL || graph.getCity(destination) == NULL){
		cout << "Invalid entry" << endl;
		return 0;
	}


	graph.Dijkstras(origin,biPreference);

	stack<Location*> cityStack = graph.cityStacker(destination);
	stack<Route*> routeStack = graph.routeStacker(destination, biPreference);

	vector<Location*> citiesOrdered;
	vector<Route*> routesOrdered;
	stack<Location*> tempCityStack = cityStack;
	stack<Route*> tempRouteStack = routeStack;
	while (!tempCityStack.empty()) {
		citiesOrdered.push_back(tempCityStack.top());
		tempCityStack.pop();
	}
	while (!tempRouteStack.empty()) {
		routesOrdered.push_back(tempRouteStack.top());
		tempRouteStack.pop();
	}
	// Reverse to get correct order (start to end)
	(citiesOrdered.begin(), citiesOrdered.end());

	// Print all stops
	cout << "\nStops: ";
	for (size_t i = 0; i < citiesOrdered.size(); ++i) {
		cout << citiesOrdered[i]->capital;
		if (i != citiesOrdered.size() - 1) cout << " -> ";
	}
	cout << endl;

	// Print each leg
	float totalTime = 0.0f;
	float totalCost = 0.0f;
	for (size_t i = 0; i < routesOrdered.size(); ++i) {
		Route* r = routesOrdered[i];
		float cost = r->cost;
		if (r->transport == "plane") cost = cost / MULTI;
		totalTime += r->time;
		totalCost += cost;
	}
	cout << "\nTotal time: " << totalTime << " hours" << endl;
	cout << "Total cost: $" << totalCost << endl;

	outputGenerator(outputFilename.c_str(), cityStack, routeStack, biPreference);

	return 0;
}
