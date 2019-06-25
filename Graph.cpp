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


/*----------------------------------------------���������� �����----------------------------------------------------*/


/*��� ������ ������� ������� ������� ��� "���������" �������*/
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


/*��������, ��������� �� ��� �������� �����*/
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



/*��������, �������� �� ������ ������ �������*/

vector<char> used; //������ �������  
vector<int> order, component;

void Graph::dfs1(int v) {
	used[v] = true;
	map< int, set< pair<int, int> > >::iterator fromIt = adjList.find(v);
	for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); ++toIt)
		if (!used[toIt->first])
			dfs1(toIt->first);
	order.push_back(v);
}
void Graph::dfs2(int v) {
	used[v] = true;
	component.push_back(v);
	map< int, set< pair<int, int> > >::iterator fromIt = adjListTransp.find(v);
	for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); ++toIt)
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
			edgListTransp.insert(make_tuple(j->first, i->first, j->second));
		}
	}
	//����������������� ������ ����� ��������� ������� � ������ ��������� 
	for (set< tuple<int, int, int> >::iterator it = edgListTransp.begin(); it != edgListTransp.end(); it++) {
		adjListTransp[get<0>(*it)].insert(make_pair(get<1>(*it), get<2>(*it)));
	}
	edgListTransp.clear();

	used.assign(100, false);
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		if (!used[fromIt->first])
			dfs1(fromIt->first);
	}

	used.assign(100, false);
	int col = 0;
	while (!order.empty()) {
		int v = order.back();
		if (!used[v]) {
			dfs2(v);
			col++;
		}
		order.pop_back();
	}
	if (col == 1)
		cout << "Graph is strongly connected\n";
	else
		cout << "Graph is not strongly connected\n";
}


/*������� ���������� ���� �� �������� ������� �� ��� ���������*/

void Graph::task_2b()
{
	const int INF = 1000000000;
	int s; //��������� ������� 
	cout << "Enter the start node: \n";
	cin >> s;
	vector<int> d(adjList.size() + 1, INF), p(adjList.size() + 1);
	d[s] = 0;
	vector<char> u(adjList.size() + 1);
	for (map< int, set< pair<int, int> > >::iterator i = adjList.begin(); i != adjList.end(); ++i) {
		int v = -1;
		for (map< int, set< pair<int, int> > >::iterator j = adjList.begin(); j != adjList.end(); ++j)
			if (!u[j->first] && (v == -1 || d[j->first] < d[v]))
				v = j->first;
		if (d[v] == INF)
			break;
		u[v] = true;

		map< int, set< pair<int, int> > >::iterator fromIt = adjList.find(v);
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) {
			int to = toIt->first,
				len = toIt->second;
			if (d[v] + len < d[to]) {
				d[to] = d[v] + len;
				p[to] = v;
			}
		}
	}
	vector<int> path;
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); ++fromIt) {
		if (fromIt->first == s)
			continue;
		else {
			for (int v = fromIt->first; v != s; v = p[v])
				path.push_back(v);
			path.push_back(s);
			reverse(path.begin(), path.end());
			cout << "to " << fromIt->first << ": ";
			for (int j = 0; j < path.size(); j++)
			{
				cout << path[j];
				cout << " ";
			}
			cout << endl;
		}
		path.clear();
	}
}


/*�������� �������� ��� ��������� ������������ ��������� ������*/

void Graph::task_3()
{
	for (map< int, set< pair< int, int > > >::iterator i = adjList.begin(); i != adjList.end(); i++) {
		for (set< pair< int, int > >::iterator j = i->second.begin(); j != i->second.end(); j++) {
			listWeight.push_back(make_pair(j->second, make_pair(i->first, j->first)));
		}
	}
	int cost = 0;
	vector < pair<int, int> > res;
	sort(listWeight.begin(), listWeight.end());
	vector<int> tree_id(adjList.size() + 1);
	for (int i = 0; i < adjList.size() + 1; ++i)
		tree_id[i] = i;
	for (int i = 0; i < listWeight.size(); ++i)
	{
		int a = listWeight[i].second.first, b = listWeight[i].second.second, l = listWeight[i].first;
		if (tree_id[a] != tree_id[b])
		{
			cost += l;
			res.push_back(make_pair(a, b));
			int old_id = tree_id[b], new_id = tree_id[a];
			for (int j = 0; j < adjList.size() + 1; ++j)
				if (tree_id[j] == old_id)
					tree_id[j] = new_id;
		}
	}
	cout << "Minimum spanning tree: \n";
	for (vector < pair<int, int> >::iterator it = res.begin(); it != res.end(); ++it)
	{
		cout << "from " << it->first << " to " << it->second << " \n";
	}
}


/*�������������� ������� � ������������ ���������� �� ���� ����������� ���������� �� ������ ������ �� ������ �������. 
������ ����� � ����������� �� ���������������� ��� ������. 
����� ����� ����� � ��������� ������, ��������������� ������� ����� ������� �����*/

void Graph::task_4a()
{
	const int INF = 1000000000;
	vector< pair<int, int> > excentr;
	for (map< int, set< pair< int, int > > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		vector<int> d(adjList.size() + 1, INF);
		d[fromIt->first] = 0;
		int max = 0;
		int vertex;
		vector<char> u(adjList.size() + 1);
		for (map< int, set< pair<int, int> > >::iterator i = adjList.begin(); i != adjList.end(); ++i) {
			int v = -1;
			for (map< int, set< pair<int, int> > >::iterator j = adjList.begin(); j != adjList.end(); ++j)
				if (!u[j->first] && (v == -1 || d[j->first] < d[v]))
					v = j->first;
			if (d[v] == INF)
				break;
			u[v] = true;
			
			map< int, set< pair<int, int> > >::iterator from = adjList.find(v);
			for (set< pair<int, int> >::iterator toIt = from->second.begin(); toIt != from->second.end(); toIt++) {
				int to = toIt->first,
					len = toIt->second;
				if (d[v] + len < d[to]) 
					d[to] = d[v] + len;
				if (d[v] > max && d[v] != 1000000000) {
					max = d[v];
					vertex = fromIt->first;
				}
			}
		}
		excentr.push_back(make_pair(max, vertex));
		d.clear();
	}
	int rad = INF;
	cout << "Excentricity - " << "Vertex\n";
	for (int j = 0; j < excentr.size(); j++) {
		cout << excentr[j].first << " " << excentr[j].second;
		cout << "\n";
		if (excentr[j].first < rad)
			rad = excentr[j].first;
	}
	cout << "\nRadius "<< rad;
	vector<int> center;
	for (int j = 0; j < excentr.size(); j++)
	{
		if (excentr[j].first == rad)
			center.push_back(excentr[j].second);
	}
	cout << "\nCenter ";
	for (int j = 0; j < center.size(); j++)
	{
		cout << center[j] << " ";
	}
}

void Graph::task_4b()
{
	const int INF = 1000000000;
	vector< pair<int, int> > excentr;
	for (map< int, set< pair< int, int > > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		vector<int> d(adjList.size() + 1, INF);
		d[fromIt->first] = 0;
		int sum = 0;
		int vertex;
		vector<char> u(adjList.size() + 1);
		for (map< int, set< pair<int, int> > >::iterator i = adjList.begin(); i != adjList.end(); ++i) {
			int v = -1;
			for (map< int, set< pair<int, int> > >::iterator j = adjList.begin(); j != adjList.end(); ++j)
				if (!u[j->first] && (v == -1 || d[j->first] < d[v]))
					v = j->first;
			if (d[v] == INF)
				break;
			u[v] = true;

			map< int, set< pair<int, int> > >::iterator from = adjList.find(v);
			for (set< pair<int, int> >::iterator toIt = from->second.begin(); toIt != from->second.end(); toIt++) {
				int to = toIt->first,
					len = toIt->second;
				if (d[v] + len < d[to])
					d[to] = d[v] + len;
					sum += d[v];
					vertex = fromIt->first;
			}
		}
		excentr.push_back(make_pair(sum, vertex));
		d.clear();
	}
	int rad = INF;
	cout << "SUM - " << "Vertex\n";
	for (int j = 0; j < excentr.size(); j++) {
		cout << excentr[j].first << " " << excentr[j].second;
		cout << "\n";
		if (excentr[j].first < rad)
			rad = excentr[j].first;
	}
	cout << "\nMIN ";
	vector<int> vertex;
	for (int j = 0; j < excentr.size(); j++) {
		if (excentr[j].first == rad)
			cout << excentr[j].second;
	}
}