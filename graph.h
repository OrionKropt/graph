#pragma once

#include <list>
#include <queue>
#include <string>
#include <iostream>

#include <fstream>

struct Top
{
	Top() : id(0), inc_row(0), inc_col(0), adj_row(0), adj_col(0), prev(0) {}
	int id;
	int inc_row;
	int inc_col;
	int adj_row;
	int adj_col;
	int prev;
};

class Graph
{
public:
	Graph();


	auto create_graph_adjacency(bool** matrix, const int& size) -> void; 
	auto create_graph_adjacency(const std::string& file_name) -> void;
	auto create_graph_incident(bool** matrix, const int rows, const int cols) -> void;
	auto create_graph_incident(const std::string& file_name) -> void;

	auto create_graph() -> void;

	auto print_adjacency_matrix() -> void;
	auto print_incident_matrix() -> void;

	auto incident_to_adjacency() -> bool;
	auto adjacency_to_incident() -> bool;

	auto bfs() -> int;
	auto bfs(const int& first, bool is_farthest) -> int;
	auto bfs_by_tops_number(const int& first, const int& second) -> int;

	auto bfs_adjacency() -> int;
	auto bfs_adjacency(const int& I, bool first) -> int;

	auto has_adjacency_matrix() -> bool;
	auto has_incident_matrix() -> bool;
	auto has_graph() -> bool;

	~Graph();
private:
	
	// adjacency matrix
	int _adj_size;
	bool** _adj_matrix;
	
	// incident matrix
	int _inc_rows;
	int _inc_cols;
	bool** _inc_matrix;

	// graph in lists
	int _size;
	std::list<Top>* _graph;


};