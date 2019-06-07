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

							// �������� (���������) ����� ������
							// ��� ������� � ������ �������� ������ ������ ������ Graph
private:

	// ������-����� (����) ������
	bool weighted;  // ����, true, ���� ���� ����������, ����� false
	bool oriented;  // ����, true, ���� ���� ���������������, ����� false
	int N;          // ���������� ������
	int M;          // ���������� �����

	map< int, set< pair< int, int > > > adjList;  // ������ ���������
};

#endif // GRAPH_H