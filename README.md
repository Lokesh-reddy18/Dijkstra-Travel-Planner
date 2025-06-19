# Dijkstra Travel Planner

A C++ implementation of a travel route planner developed as a Data Structures and Algorithms (DSA) project. This project demonstrates the practical application of graph algorithms, specifically Dijkstra's algorithm, to solve real-world routing problems.

## Project Overview

This travel planner uses Dijkstra's algorithm to find optimal travel routes between cities worldwide. The program can calculate both the fastest and cheapest routes between any two cities using various modes of transportation (planes, trains, buses).

## Technical Implementation

### Data Structures Used
- Graph representation using vectors of Location and Route objects
- Priority Queue (minHeap) for Dijkstra's algorithm implementation
- Vectors for storing cities and routes
- Stacks for path reconstruction
- Custom Location and Route classes for data management

### Algorithms
- Dijkstra's Algorithm for finding shortest paths
- Graph traversal using adjacency lists
- Path reconstruction using backtracking with stacks
- Priority queue-based optimization

## Features

- Find optimal travel routes between any two cities worldwide
- Support for multiple transportation modes (planes, trains, buses)
- Option to optimize for either fastest or cheapest route
- Interactive command-line interface
- Generates HTML output with visual route representation using Google Maps
- Comprehensive database of cities and routes

## Prerequisites

- C++ compiler (g++ recommended)
- Basic understanding of command-line operations
- Knowledge of graph algorithms and data structures

## Installation

1. Clone or download this repository
2. Ensure you have g++ installed on your system
3. Navigate to the project directory

## Compilation

Compile the program using g++:

```bash
g++ -o travel Main.cpp FileOperations.h Location.h Route.h GraphFunctions.h
```

## Usage

1. Run the compiled program:
   ```bash
   ./travel
   ```

2. Follow the prompts to:
   - Enter the cities file (default: cities.csv)
   - Enter the routes file (default: routes.csv)
   - Specify the output HTML file name
   - Enter the origin city
   - Enter the destination city
   - Choose your preference (fastest/cheapest)

3. The program will generate an HTML file containing:
   - A visual map of your route using Leaflet
   - Detailed information about each leg of the journey
   - Transportation mode, duration, and cost for each segment

## Input Files

- `cities.csv`: Contains city information including:
  - Country name
  - City name
  - Latitude
  - Longitude

- `routes.csv`: Contains route information including:
  - Origin city
  - Destination city
  - Transportation mode
  - Travel time (in hours)
  - Cost (note: plane costs are multiplied by 3)
  - Additional notes

## Example

```
Enter filename containing cities: cities.csv
Enter filename containing routes: routes.csv
Enter filename for output (.html): output.html
Origin: London
Destination: Paris
Enter a preference (fastest/cheapest): fastest
```

## Notes

- City names are case-sensitive and must match exactly with the entries in cities.csv
- The program supports both direct and indirect routes
- The generated HTML file can be opened in any web browser to view the route map
- Some routes may require multiple connections to reach the destination
- Plane routes have a cost multiplier of 3x compared to other transportation modes
- The program requires an internet connection to display the Google Maps visualization

## Project Structure

- `Main.cpp`: Entry point and user interface
- `GraphFunctions.h`: Implementation of Dijkstra's algorithm and graph operations
- `Location.h`: City data structure and operations
- `Route.h`: Route data structure and operations
- `FileOperations.h`: File I/O and data parsing
- `Parser.h`: CSV file parsing utilities
- `cities.csv`: Database of cities with coordinates
- `routes.csv`: Database of available routes
- `travel.make`: Makefile for compilation

## Learning Outcomes

This project demonstrates:
- Implementation of Dijkstra's algorithm
- Graph data structure implementation
- Priority queue usage for optimization
- File I/O operations in C++
- Command-line interface design
- Path finding and optimization techniques
- Integration with external APIs (Leaflet)

