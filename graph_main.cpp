//include <iostream>

#include "graph.h"
using namespace std;

int main()
{
	Graph graph;
	string file_name = "graphs.txt";
	graph.create_graph_adjacency(file_name);
	//graph.create_graph_incident(file_name);
	graph.adjacency_to_incident();
	//graph.incident_to_adjacency();
	graph.print_incident_matrix();
	cout << endl;
	graph.print_adjacency_matrix();
	cout << endl;
	graph.create_graph();

	cout << graph.bfs_adjacency() << endl;
	cout << graph.bfs() << endl;
	cout << graph.bfs_by_tops_number(0, 2) << endl;

	return 0;
}