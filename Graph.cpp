// ���� "Graph.cpp"
// �������� ���������� ���� �������, ��������� � ����� ���������

#include "stdafx.h"
#include "Graph.h"      // ���� ��������� ������ Graph
#include <fstream>      // �������� ����-�����
#include <iostream>     // ���������� ����-�����
#include <sstream>      // ��������� ����-�����
#include <limits>       // ��������� �����
#include <algorithm>    // ����������� ���������

/*
����������� ������ Graph. �������������� ��� ������-����� (����) ������
*/
Graph::Graph()
{
	// ���������� �������� �� ��������� ��� ���� ����� ������
	weighted = false;
	oriented = false;
	N = 0;
	M = 0;
}

/*
���������� ������ Graph
*/
Graph::~Graph()
{
}

/*
������� ������ ����� �� �����.
������� ���������� � 0.
*/
void Graph::readGraph(string fileName)
{
	// ������� ������ ����� � ���������� ������� ���� � ������ "fileName"
	ifstream file(fileName.c_str());

	// ���� �� ������� ������� ����, ������ ����������
	if (!file.is_open()) {
		cout << "Can't open file " << fileName << "\n";
		return;
	}

	cout << "Reading graph from file " << fileName << "...\t";

	// �������� ������� ���������� ������ ���������
	adjList.clear();
	// �������� ������� ���������� ������������������ ������ ���������
	adjListTransp.clear();

	// ������� ���������� ������
	file >> N;

	// ������� ��������� ����������������� �����
	file >> oriented;

	// ������� ��������� ������������ �����
	file >> weighted;

	// ���������� ������ ������ ����� �� ����� ������
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// ������� N ������ ������
	for (int from = 0; from < N; from++) {
		// ������ ��� �������� ������ ������ ������� �������
		string rowStr;

		// ������� ������� ������
		getline(file, rowStr);

		// ��������� ����� ����� (��� ��������� row �� ��������� ������)
		istringstream iss(rowStr);

		// ���������� ��� �������� ������ ������� �������
		set< pair<int, int> > row;

		// ���������� ��� �������� ������ �������
		int node;

		// ������� �������
		iss >> node;

		// ������ �� rowStr
		while (!iss.eof() && !iss.str().empty()) {
			// ���������� ��� �������� ������ �������� �������
			int to;

			// ���������� ��� �������� ���� �����
			int weight;

			// ������� �������� �������
			iss >> to;

			// ���� ������ ���������� ����
			if (weighted)
				// ������� ��� �����
				iss >> weight;

			// ��������� ��������� ����� � ������ ������
			row.insert(make_pair(to, weight));

		};
		// ������������ ���������� ������� � ������
		iss.ignore(128);

		// �������� ������ ������ ��������� ������� � ������ ���������
		adjList[node] = row; 
	}

	cout << "done\n";

	// ���������� ����
	printGraph();
}


void Graph::addNode()
{
	cout << "Adding node " << "...\t";

	//������� ����� �������
	adjList[N];

	// ��������� ������� ������
	N++;

	cout << "done\n";

	// ���������� ����
	printGraph();
}

void Graph::removeNode(int from)
{
	cout << "Removing node " << from << "...\t";

	// �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
	map< int, set< pair<int, int> > >::iterator fromIt;

	// �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
	set< pair<int, int> >::iterator toIt;

	// ����� � ������ ��������� ������ �����
	fromIt = adjList.find(from);

	// ���������� ��������� �����
	int count_edge = 0; 

	// ���� �������
	if (fromIt != adjList.end()) {
		// ������������, ������� ����� � ��������� �������
		count_edge = adjList.count(from);
		
		//������� ��
		adjList.erase(from);

		// ������ �� �������
		for (map< int, set< pair<int, int> > >::iterator It = adjList.begin(); It != adjList.end(); It++) {

			// ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
			toIt = find_if(It->second.begin(), It->second.end(), [&from](const pair<int, int>& edge) {
				if (edge.first == from) return true;
				return false;
			});

			// ���� ������� ���-�� �����
			if (toIt != It->second.end()) {
				// ������� ��������� �����
				It->second.erase(toIt);
				count_edge++;
			}
		}
	}
	else { //���� ��� ����� ������� 
		// �������� �� ������
		cout << "\nCan't removing node " << from << ", the node doesn't exists\n";
		return;
	}

	// ��������� ������� ������
	N--;

	// ��������� ������� �����
	M -= count_edge;

	cout << "Deleting edge from this node: " << count_edge << "\n";

	cout << "done\n";

	// ���������� ����
	printGraph();
}


void Graph::addEdge(int from, int to, int weight)
{
	cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

	// �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
	map< int, set< pair<int, int> > >::iterator fromIt;

	// ����� � ������ ��������� ������ �����
	fromIt = adjList.find(from);

	// ���� �� �������
	if (fromIt == adjList.end()) {
		// �������� �� ������
		cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
		return;
	}

	// �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
	set< pair<int, int> >::iterator toIt;

	// ���� ���� ����������
	if (weighted)
		// ����� ������� ������� �������, ��������� ��������� ���
		toIt = fromIt->second.find(make_pair(to, weight));
	else    // // ���� ���� ������������
		// ����� ������� ������� �������, ��������� � �������� ���� 0
		toIt = fromIt->second.find(make_pair(to, 0));

	// ���� ������� ���-�� �����
	if (toIt != fromIt->second.end()) {
		// �������� �� ������
		cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
		return;
	}

	// ���� ���� ����������
	if (weighted)
		// �������� ����� �������� �������, ��������� ����� ��������� ���
		fromIt->second.insert(make_pair(to, weight));
	else    // ���� ���� ������������
		// �������� ����� �������� �������, ��������� ����� ��� 0
		fromIt->second.insert(make_pair(to, 0));

	// ���� ���� �����������������, �� �������� ����� ����� � �������� �����������
	if (!oriented) {
		// ���� ���� ����������
		if (weighted)
			// ������������ ��������� ���
			adjList[to].insert(make_pair(from, weight));
		else    // ���� ���� ������������
			// ������������ ��� 0
			adjList[to].insert(make_pair(from, 0));
	}

	// ��������� ������� �����
	M++;

	cout << "done\n";

	// ���������� ����
	printGraph();
}


void Graph::removeEdge(int from, int to)
{
	cout << "Removing edge from " << from << " to " << to << "...\t";

	// �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
	map< int, set< pair<int, int> > >::iterator fromIt;

	// �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
	set< pair<int, int> >::iterator toIt;

	// ����� � ������ ��������� ������ �����
	fromIt = adjList.find(from);

	// ���� �������
	if (fromIt != adjList.end()) {
		// ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&to](const pair<int, int>& edge) {
			if (edge.first == to) return true;
			return false;
		});

		// ���� ������� ���-�� �����
		if (toIt != fromIt->second.end())
			// ������� ��������� �����
			fromIt->second.erase(toIt);
	}

	// ���� ���� �����������������, �� ������� ����� ����� � �������� �����������
	if (!oriented) {
		// ����� � ������ ��������� ������ �����
		fromIt = adjList.find(to);

		// ���� �� �������
		if (fromIt == adjList.end())
			// ������� �� ����������, ������� ������
			return;

		// ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&from](const pair<int, int>& edge) {
			if (edge.first == from) return true;
			return false;
		});

		// ���� �� ������� ������ �����
		if (toIt == fromIt->second.end()) {
			// �������� �� ������ (� ����������������� ����� ����� ������ ���� � ����� ������������)
			cout << "\nCan't remove edge from " << to << " to " << from << ", the edge doesn't exists\n";
			return;
		}

		// ������� ��������� �����
		fromIt->second.erase(toIt);
	}

	// ��������� ������� �����
	M--;

	cout << "done\n";

	// ���������� ����
	printGraph();
}



int Graph::changeEdge(int from, int to, int newWeight)
{
	// ���� ���� ������������
	if (weighted == false) {
		// �������� �� ������ (� ����� ��� �����, �������������� ������)
		cout << "\tSorry, you can't modify an edge weight of non-weighted graph\n";
		return -1;
	}
	else {
		cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";
		int oldWeight = 0;

		// �������� ����������� - ��������� �� ������ ������ ������� � ������ ���������
		map< int, set< pair<int, int> > >::iterator fromIt;

		// ����� � ������ ��������� ������ �����
		fromIt = adjList.find(from);

		// ���� �� �������
		if (fromIt == adjList.end()) {
			// �������� �� ������
			cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
			return -1;
		}

		fromIt->second.insert(make_pair(1, 2));

		// �������� ��������� - ��������� �� �������� ������� � ������ ������ ���������
		set< pair<int, int> >::iterator toIt;

		// ����� ����� ������� �������, ���������� ������ �����, ������� ���������� ��������
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&to](const pair<int, int>& edge) {
			if (edge.first == to) return true;
			return false;
		});

		// ���� �� ������� ������ �����
		if (toIt == fromIt->second.end()) {
			// �������� �� ������
			cout << "\nCan't change edge from " << from << " to " << to << ", the edge doesn't' exists\n";
			return -1;
		}

		// ��������� ������� �������� ����
		oldWeight = toIt->second;

		// ��������� ����� ��������� ���
		pair<int, int> copy = *toIt;
		copy.second = newWeight;
		fromIt->second.erase(toIt);
		fromIt->second.insert(copy);

		// ���� ���� �����������������, �� �������� ����� ����� � �������� �����������
		if (!oriented) {
			// ����� � ������ ��������� ������ �����
			fromIt = adjList.find(to);

			// ���� �� �������
			if (fromIt == adjList.end())
				// ������� �� ����������, ������� ������
				return -1;

			// ����� ����� ������� �������, ���������� ������ �����, ������� ���������� �������
			toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&from](const pair<int, int>& edge) {
				if (edge.first == from) return true;
				return false;
			});

			// ���� �� ������� ������ �����
			if (toIt == fromIt->second.end()) {
				// �������� �� ������ (� ����������������� ����� ����� ������ ���� � ����� ������������)
				cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
				return -1;
			}

			// ��������� ����� ��������� ���
			pair<int, int> copy = *toIt;
			copy.second = newWeight;
			fromIt->second.erase(toIt);
			fromIt->second.insert(copy);
		}

		cout << "done\n";

		// ���������� ����
		printGraph();

		// ������� ������� �������� ����
		return oldWeight;
	}
}

bool Graph::is_weighted()
{
	return weighted;
}


void Graph::printGraph()
{
	cout << "Printing graph (";

	// ���������� ���� �����������������
	if (oriented)
		cout << "oriented, ";
	else
		cout << "not oriented, ";

	// ���������� ���� ������������
	if (weighted)
		cout << "weighted, ";
	else
		cout << "not weighted, ";

	cout << "adjacency list)...\t\n";

	// ������ �� �������
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		cout << "node " << fromIt->first << ": ";
		// ������ �� ��������
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) {
			// ���������� ����� �������� �����
			cout << "to " << toIt->first;

			// ���� ���� ����������
			if (weighted)
				// ���������� ��� �������� �����
				cout << " weight: " << toIt->second << ", ";
			cout << " ";
		}
		cout << "\n";
	}
}


void Graph::writeGraph(string fileName)
{
	// ������� ������ ����� � ���������� ������� ���� � ������ "fileName"
	ofstream file(fileName.c_str());

	// ���� �� ������� ������� ����, ������ ����������
	if (!file.is_open()) {
		cout << "Can't open file " << fileName << "\n";
		return;
	}

	cout << "Writing graph to file " << fileName << "...\t";

	// ��������� ���������� ������
	file << N << "\n";

	// ��������� ���� ����������������� �����
	file << oriented << " ";

	// ��������� ���� ������������ �����
	file << weighted << "\n";

	// ������ �� �������
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		file << fromIt->first << " ";
		// ������ �� ��������
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) {
			// ���������� ����� �������� �����
			file << toIt->first;

			// ���� ���� ����������
			if (weighted)
				// ���������� ��� �������� �����
				file << " " << toIt->second;
			file << " ";
		}
		file << "\n";
	}
	cout << "done\n";
}

void Graph::task_1a()
{
	if (oriented) {
		cout << "Enter the node: ";
		int from = 0;
		cin >> from;
		map< int, set< pair<int, int> > >::iterator fromIt;
		// ����� � ������ ��������� �������
		fromIt = adjList.find(from);
		// ���� ����� 
		if (fromIt != adjList.end()) {
			cout << "Outgoing nodes: ";
			if (fromIt->second.size() == 0) 
				cout << "This node has no neighbors\n";
			else {
				// ������ �� ��������
				for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) 
					// ���������� ��������� �������
					cout << toIt->first << " ";
			}
		}
		else
			cout << "\nThis node doesn't exists\n";
	}
	else
		cout << "\nThe task is impossible. Graph is non-oriented\n";
}

void Graph::task_1b()
{
	Graph graph2;
	cout << "Reading graph...\n";

	string fileName;

	cout << "\tEnter file name > ";
	cin >> fileName;
	graph2.readGraph(fileName); 
	if (adjList.size() == graph2.adjList.size() && adjList == graph2.adjList && oriented == graph2.oriented && weighted == graph2.weighted)
		cout << "Graphs match\n";
	else
		cout << "Graphs do not match\n";
}

vector<char> used; //������ �������  
vector<int> order, component;

void Graph::dfs1(int v, map< int, set< pair<int, int> > >::iterator fromIt) {
	used[v] = true;
	/*map< int, set< pair<int, int> > >::iterator fromIt = adjList.find(v);*/
	for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
		if (!used[toIt->first])
			dfs1(toIt->first, fromIt);  
	order.push_back(v);
}
void Graph::dfs2(int v) {
	used[v] = true;
	component.push_back(v);
	map< int, set< pair<int, int> > >::iterator fromIt = adjListTransp.find(v);
	for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
		if (!used[toIt->first])
			dfs2(toIt->first);
}

void Graph::task_2a()
{
	// ���������������� ������ ��������� 
	// ������� ��������� ��� � ����������������� ������ �����
	edgListTransp.clear();
	for (map< int, set< pair< int, int > > >::iterator i = adjList.begin(); i != adjList.end(); i++) {
		for (set< pair< int, int > >::iterator j = i->second.begin(); j != i->second.end(); j++) {
			//edgList.insert(make_tuple(i->first, j->first, j->second));
			edgListTransp.insert(make_tuple(j->first, i->first, j->second));
		}
	}
	//����������������� ������ ����� ��������� ������� � ������ ��������� 
	for (set< tuple<int, int, int> >::iterator it = edgListTransp.begin(); it != edgListTransp.end(); it++) {
		adjListTransp[get<0>(*it)].insert(make_pair(get<1>(*it), get<2>(*it)));
	}
	edgListTransp.clear();

	// ������ �� �������
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjListTransp.begin(); fromIt != adjListTransp.end(); fromIt++) {
		cout << "node " << fromIt->first << ": ";
		// ������ �� ��������
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
			// ���������� ����� �������� �����
			cout << "to " << toIt->first;
		cout << "\n";
	}

	used.assign(100, false);
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		if (!used[fromIt->first]) 
			dfs1(fromIt->first, fromIt);
	}


	for (int i = 0; i < order.size(); i++)
		cout << order[i];
	cout << " ";

	used.assign(100, false);
	for (int i = 0; i < order.size(); i++) {
		int v = order.back();
		if (!used[v]) {
			dfs2(v);
			for (int i = 0; i < component.size(); i++)
				cout << component[i];
			component.clear();
			cout << " ";
		}
		order.pop_back();
	}
}