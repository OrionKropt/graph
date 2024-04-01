#include "graph.h"

Graph::Graph() :	
	_adj_size(0),
	_adj_matrix(nullptr),
	_inc_cols(0),
	_inc_rows(0),
	_inc_matrix(nullptr),
	_size(0),
	_graph(nullptr)
{}

auto Graph::create_graph_adjacency(bool** matrix, const int& size) -> void
{
	_adj_matrix = new bool* [size];
	int size_byte = sizeof(bool) * size;
	for (int i = 0; i < size; ++i)
	{
		_adj_matrix[i] = new bool[size] {0};
		memcpy_s(_adj_matrix[i], size_byte, matrix[i], size_byte);
	}
	_adj_size = size;
}

auto Graph::create_graph_adjacency(const std::string& file_name) -> void
{
	std::ifstream fin(file_name);
	fin >> _adj_size;
	_adj_matrix = new bool* [_adj_size];
	for (int i = 0; i < _adj_size; ++i)
		_adj_matrix[i] = new bool[_adj_size] {0};
	for (int i = 0; i < _adj_size; ++i)
		for (int j = 0; j < _adj_size; ++j)
			fin >> _adj_matrix[i][j];
	fin.close();
}

auto Graph::create_graph_incident(bool** matrix, const int rows, const int cols) -> void
{
	_inc_matrix = new bool* [rows];
	int size_byte = sizeof(bool) * cols;
	for (int i = 0; i < rows; ++i)
	{
		_inc_matrix[i] = new bool[cols];
		memcpy_s(_inc_matrix[i], size_byte, matrix[i], size_byte);
	}
	_inc_cols = cols;
	_inc_rows = rows;
}

auto Graph::create_graph_incident(const std::string& file_name) -> void
{
	std::ifstream fin(file_name);
	fin >> _inc_rows >> _inc_cols;
	_inc_matrix = new bool* [_inc_rows];
	for (int i = 0; i < _inc_rows; ++i)
		_inc_matrix[i] = new bool[_inc_cols] {0};
	for (int i = 0; i < _inc_rows; ++i)
		for (int j = 0; j < _inc_cols; ++j)
			fin >> _inc_matrix[i][j];
	fin.close();
}

auto Graph::create_graph() -> void
{
	if (has_incident_matrix())
	{
		_graph = new std::list<Top>[_inc_rows];
		_size = _inc_rows;
		for (int i = 0; i < _inc_cols; ++i)
		{
			int j = -1, index = 0, num = 0;
			bool flag = false;
			while (j < _inc_rows && num < 2)
			{
				++j;
				if (_inc_matrix[j][i])
				{
					++num;
					if (!flag) index = j;
					flag = true;
				}
			}
			Top top1;
			Top top2;
			top1.inc_col = j;
			top1.inc_row = top1.inc_row = i;
			top1.id = j;
			top2.inc_col = index;
			top2.id = index;
			_graph[index].emplace_back(top1);
			_graph[j].emplace_back(top2);
		}
	}

	else if (has_adjacency_matrix())
	{
		_graph = new std::list<Top>[_adj_size];
		_size = _adj_size;
		for (int i = 0; i < _adj_size; ++i)
		{
			for (int j = 0; j < _adj_size; ++j)
			{
				if (_adj_matrix[i][j])
				{
					Top top;
					top.adj_col = i;
					top.adj_row = j;
					top.id = j;
					_graph[i].emplace_back(top);
				}
			}
		}
	}
}

auto Graph::print_adjacency_matrix() -> void
{
	if (!has_adjacency_matrix()) return;
	for (int i = 0; i < _adj_size; ++i)
	{
		for (int j = 0; j < _adj_size; ++j)
			std::cout << _adj_matrix[i][j] << ' ';
		std::cout << std::endl;
	}
}

auto Graph::print_incident_matrix() -> void
{
	if (!has_incident_matrix()) return;
	for (int i = 0; i < _inc_rows; ++i)
	{
		for (int j = 0; j < _inc_cols; ++j)
			std::cout << _inc_matrix[i][j] << ' ';
		std::cout << std::endl;
	}
}

auto Graph::incident_to_adjacency() -> bool
{
	if (_adj_matrix != nullptr || _inc_matrix == nullptr) return 0;
	_adj_size = _inc_rows;
	_adj_matrix = new bool* [_adj_size];
	for (int i = 0; i < _adj_size; ++i)
		_adj_matrix[i] = new bool[_adj_size] {0};

	for (int i = 0; i < _inc_cols; ++i)
	{
		int num = 0, j = -1, index = 0;
		bool flag = false;
		while (j < _inc_rows && num < 2)
		{
			++j;
			if (_inc_matrix[j][i])
			{
				if (!flag) index = j;
				++num;
				flag = true;
			}
		}
		_adj_matrix[index][j] = 1;
		_adj_matrix[j][index] = 1;
	}

	return 1;

}

auto Graph::adjacency_to_incident() -> bool
{
	if (_adj_matrix == nullptr || _inc_matrix != nullptr) return 0;
	_inc_rows = _adj_size;
	for (int i = 0; i < _adj_size; ++i)
		for (int j = i; j < _adj_size; ++j)
			if (_adj_matrix[i][j]) ++_inc_cols;

	_inc_matrix = new bool* [_inc_rows];
	for (int i = 0; i < _inc_rows; ++i)
		_inc_matrix[i] = new bool[_inc_cols] {0};

	int num = 0;
	for (int i = 0; i < _adj_size; ++i)
	{
		for (int j = i; j < _adj_size; ++j)
		{
			if (_adj_matrix[i][j])
			{
				_inc_matrix[j][num] = 1;
				_inc_matrix[i][num++] = 1;
			}
		}
	}

	
}

auto Graph::bfs() -> int
{
	return (_graph == nullptr) ? (bfs_adjacency(0, 0)) : (bfs(0, 0));
}

auto Graph::bfs(const int& first, bool is_farthest) -> int
{
	std::queue<int> q;
	bool* used = new bool[_size] {0};
	int* dis = new int[_size] {0};
	q.push(first);
	used[first] = true;
	int Max = 0;
	int temp;
	while (!q.empty())
	{
		temp = q.front();
		q.pop();
		for (auto a : _graph[temp])
		{
			if (!used[a.id])
			{
				dis[a.id] = dis[temp] + 1;
				used[a.id] = true;
				a.prev = temp;
				Max = std::max(Max, dis[a.id]);
				q.push(a.id);
			}
		}
	}
	
	if (!is_farthest) Max = bfs(temp, 1);

	delete[]used;
	delete[] dis;

	return Max;
}

auto Graph::bfs_by_tops_number(const int& first, const int& second) -> int
{
	if (second >= _size || first >= _size || second < 0 || first < 0) return -1;
	if (_graph == nullptr) return -1;
	std::queue<int> q;
	bool* used = new bool[_size] {0};
	int* dis = new int[_size] {0};
	q.push(first);
	used[first] = true;
	int temp;
	while (!q.empty())
	{
		temp = q.front();
		if (temp == second) 
		{
			
			int out = dis[temp];
			delete[]used;
			delete[] dis;
			return out;
		}
		q.pop();
		for (auto a : _graph[temp])
		{
			if (!used[a.id])
			{
				dis[a.id] = dis[temp] + 1;
				used[a.id] = true;
				a.prev = temp;
				q.push(a.id);
			}
			
		}
	}

}





auto Graph::bfs_adjacency() -> int
{
	if (has_adjacency_matrix())
		return bfs_adjacency(0, 0);
}

auto Graph::bfs_adjacency(const int& I, bool first) -> int
{
	int Max = 0;
	std::queue<int> q;
	bool* used = new bool [_adj_size] {0};
	int* dis = new int [_adj_size] {0};

	q.push(I);
	used[I] = 1;
	int temp;
	while (!q.empty())
	{
		temp = q.front();
		q.pop();
		for (int j = 0; j < _adj_size; ++j)
		{
			if (_adj_matrix[temp][j] && !used[j])
			{
				q.push(j);
				used[j] = 1;
				dis[j] = dis[temp] + 1;
				if (Max < dis[j]) Max = dis[j];
			}
		}
	}
	if (!first) Max = bfs_adjacency(temp, 1);
	return Max;
}



auto Graph::has_adjacency_matrix() -> bool
{
	return _adj_matrix != nullptr;
}

auto Graph::has_incident_matrix() -> bool
{
	return _inc_matrix != nullptr;
}

auto Graph::has_graph() -> bool
{
	return _graph != nullptr;
}

Graph::~Graph()
{
	for (int i = 0; i < _adj_size; ++i)
		delete[] _adj_matrix[i];
	delete[] _adj_matrix;

	for (int i = 0; i < _inc_rows; ++i)
		delete[] _inc_matrix[i];
	delete[] _inc_matrix;

	delete[] _graph;

}
