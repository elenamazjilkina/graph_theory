// ���� "Graph.h"
// ������������ ���� ������ Graph (���������)

// ���������� define
// ��������� ��� ����, ����� ���������� ���� �������� ���� ���� ������
#ifndef GRAPH_H
#define GRAPH_H

// ������� ����������� ������� ����������� ���������� STL
#include <string>   // ������ - ������������������ ��������
#include <vector>   // ������ - ������, ������� ����� �������� ���� ������
#include <set>      // ��������� - ����� ��������� ������ ����
#include <map>      // ����������� - ����� ��������� "����-��������"
#include <tuple>    // ������ - N ���������, ��������������� ��� ������ �����

// ������� ��� ������� � ������� ����������� ���������� "��������" � ������� ����
using namespace std;

// ����� Graph
// ������������ ����� � ���������
class Graph
{
	// ������� (���������) ����� ������, ��� ���������
	// ������� �� ���� ����� ����� ���� ������� ����� ������
public:
	// ����������� ������
	Graph();

	// ���������� ������
	~Graph();

	// ��������� ���� �� ����� fileName
	void readGraph(string fileName);

	// ��������� ���� � ���� fileName
	void writeGraph(string fileName);

	// ������� ������� ������� ���� � ���������
	void printGraph();

	// ��������� �������
	void addNode();

	// ������� �������
	void removeNode(int from);

	// ��������� ����� � ����� weight ����� ��������� from � to
	void addEdge(int from, int to, int weight);

	// ������� ����� ����� ��������� from � to
	void removeEdge(int from, int to);

	// �������� ��� ����� ����� ��������� from � to �� newWeight
	// ���������� ������� �������� ����
	int changeEdge(int from, int to, int newWeight);

	bool is_weighted();		// ���������� ���� ������������ �����

	void task_1a();
	void task_1b();
	void task_2a();
	void dfs1(int v);
	void dfs2(int v);
	void task_2b();
	void task_3();

	map< int, set< pair< int, int > > > adjList;  // ������ ���������
	map< int, set< pair< int, int > > > adjListTransp;  // ������ ��������� �����������������
	vector< vector< int > > adjMatrix;            // ������� ���������
	set< tuple< int, int, int > > edgList;        // ������ �����
	set< tuple< int, int, int > > edgListTransp;        // ������ ����� �����������������
	vector < pair < int, pair<int, int> > > listWeight; //������ ����� ��� ��������� �� ���� � ��������� ��������

														// �������� (���������) ����� ������
														// ��� ������� � ������ �������� ������ ������ ������ Graph
private:

	// ������-����� (����) ������
	bool weighted;  // ����, true, ���� ���� ����������, ����� false
	bool oriented;  // ����, true, ���� ���� ���������������, ����� false
	int N;          // ���������� ������
	int M;          // ���������� �����


};

#endif // GRAPH_H