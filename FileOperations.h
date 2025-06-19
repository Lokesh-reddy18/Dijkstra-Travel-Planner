#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "Location.h"

using namespace std;

vector<Location*> locationParser(string filename, vector<Route*> routes){
	fstream locations(filename.c_str());

	string country;
	string city;
	string latitude;
	string longitude;

	vector<Location*> cities;
	Location* node;

	while(locations.good()){
		getline(locations, country, ',');
		getline(locations, city, ',');
		getline(locations, latitude, ',');
		getline(locations, longitude);
		//cout << "Country:" << country << " City:" << city << " Lat:" << latitude << " Lon:" << longitude << endl << endl << endl;

		node = new Location(country, city, atof(latitude.c_str()), atof(longitude.c_str()));

		vector<Route*>::iterator it = routes.begin();

		while(it != routes.end()){
			if((*it) -> originS.compare(node -> capital) == 0){
				(*it) -> origin = node;
				node -> routes.push_back((*it));
			}
			else if((*it) -> destinationS.compare(node -> capital) == 0){
				(*it) -> destination = node;
			}
			++it;
		}

		cities.push_back(node);

	}
	cout << "Cities Parsed from: " << filename << endl;
	return cities;
}


/*
Parses and builds Route objects from file routes.csv
*/

vector<Route*> routeParser(string filename){

	fstream routes(filename.c_str());

	string originS;
	string destinationS;

	Location* origin = new Location();
	Location* destination = new Location();

	string type;
	string time;
	string cost;
	string note;

	vector<Route*> allRoutes;
	Route* edge;

	while(routes.good()){
		getline(routes, originS, ',');
		getline(routes, destinationS, ',');
		getline(routes, type, ',');
		getline(routes, time, ',');
		getline(routes, cost, ',');
		getline(routes, note);

		//cout << "Origin: " << originS << " Destination: " << destinationS << "---" << type << " " << time << " " << cost << " " << endl; //debug

		edge = new Route(origin, destination, type, atof(time.c_str()), atof(cost.c_str()), note);
		edge -> destinationS = destinationS;
		edge -> originS = originS;

		allRoutes.push_back(edge);
	}

	cout << "Routes Parsed from: " << filename << endl;
	return allRoutes;
}



/*
Generates an appropriately formatted html file that displays the route information passed in by the stack fo cities and routes
*/
void outputGenerator(string filename, stack<Location*> cities, stack<Route*> routes, bool costOrTime){
	// Copy stacks to vectors for easier processing
	vector<Location*> citiesOrdered;
	vector<Route*> routesOrdered;
	stack<Location*> tempCityStack = cities;
	stack<Route*> tempRouteStack = routes;
	while (!tempCityStack.empty()) {
		citiesOrdered.push_back(tempCityStack.top());
		tempCityStack.pop();
	}
	while (!tempRouteStack.empty()) {
		routesOrdered.push_back(tempRouteStack.top());
		tempRouteStack.pop();
	}
	// Defensive: routesOrdered.size() should be one less than citiesOrdered.size()
	size_t numLegs = std::min(routesOrdered.size(), (citiesOrdered.size() > 1 ? citiesOrdered.size() - 1 : 0));

	// Calculate totals
	float totalTime = 0.0f;
	float totalCost = 0.0f;
	for (size_t i = 0; i < numLegs; ++i) {
		Route* r = routesOrdered[i];
		float cost = r->cost;
		if (r->transport == "plane") cost = cost / MULTI;
		totalTime += r->time;
		totalCost += cost;
	}

	ofstream output(filename.c_str());
	output << "<HTML><HEAD><TITLE>Travel Route Summary</TITLE>";
	// Bootstrap and custom styles
	output << "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet'>";
	output << "<link rel='stylesheet' href='https://unpkg.com/leaflet@1.9.4/dist/leaflet.css' />";
	output << "<style>body{background:#f8f9fa;}#summary,#details{background:#fff;border-radius:10px;box-shadow:0 2px 8px #0001;padding:20px;}#details{margin-top:20px;}#map{border-radius:10px;box-shadow:0 2px 8px #0001;}details summary{font-size:1.1em;}li{margin-bottom:4px;}.leg-icon{font-size:1.2em;margin-right:8px;}#summary{position:sticky;top:0;z-index:100;}table{margin-bottom:0;} .leaflet-container { height: 400px; width: 100%; margin-top: 20px; }</style>";
	output << "</HEAD>";
	output << "<body><div class='container my-4'>";
	// --- Summary Table ---
	output << "<div id='summary' class='mb-4'><h2 class='mb-3'>Route Summary</h2>";
	output << "<table class='table table-bordered'><tr><th>Total Time</th><th>Total Cost</th><th>Stops</th></tr><tr>";
	output << "<td>" << totalTime << " hours</td>";
	output << "<td>$" << totalCost << "</td>";
	output << "<td>";
	for (size_t i = 0; i < citiesOrdered.size(); ++i) {
		output << citiesOrdered[i]->capital;
		if (i != citiesOrdered.size() - 1) output << " &rarr; ";
	}
	output << "</td></tr></table></div>";

	// --- Map ---
	output << "<div id='map'></div>";
	output << "<script src='https://unpkg.com/leaflet@1.9.4/dist/leaflet.js'></script>\n";
	output << "<script>\n";
	// Center map on first city or default
	double centerLat = 0.0, centerLon = 0.0;
	if (!citiesOrdered.empty()) {
		centerLat = citiesOrdered[0]->lat;
		centerLon = citiesOrdered[0]->lon;
	}
	// Only street layer
	output << "var street = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { attribution: '© OpenStreetMap contributors' });\n";
	output << "var map = L.map('map', { center: [" << centerLat << ", " << centerLon << "], zoom: 4, layers: [street] });\n";
	// Add markers with popups for leg details
	for (size_t i = 0; i < citiesOrdered.size(); ++i) {
		std::string popup;
		if (i == 0) {
			popup = "Start of route";
		} else if (i == citiesOrdered.size() - 1) {
			popup = "End of route";
		} else {
			Route* r = routesOrdered[i-1];
			float cost = r->cost;
			if (r->transport == "plane") cost = cost / MULTI;
			popup = "Arrived from <b>" + citiesOrdered[i-1]->capital + "</b><br>";
			popup += "<b>Mode:</b> " + r->transport + "<br>";
			popup += "<b>Time:</b> " + std::to_string(r->time) + " hours<br>";
			popup += "<b>Cost:</b> $" + std::to_string(cost) + "<br>";
			if (!r->note.empty()) popup += "<b>Notes:</b> " + r->note + "<br>";
		}
		popup = citiesOrdered[i]->capital + ", " + citiesOrdered[i]->country + "<br>" + popup;
		output << "L.marker([" << citiesOrdered[i]->lat << ", " << citiesOrdered[i]->lon << "]).addTo(map).bindPopup('" << popup << "');\n";
	}
	// Draw route polyline
	output << "var routeLine = L.polyline([";
	for (size_t i = 0; i < citiesOrdered.size(); ++i) {
		output << "[" << citiesOrdered[i]->lat << ", " << citiesOrdered[i]->lon << "]";
		if (i != citiesOrdered.size() - 1) output << ", ";
	}
	output << "], {color: 'blue', weight: 4, opacity: 0.8}).addTo(map);\n";
	// Fit map to route
	output << "map.fitBounds(routeLine.getBounds());\n";
	output << "</script>";
	output << "</div></body></HTML>";
	output.close();

	cout << "Output File Generated: " << filename << endl;
}


#endif
