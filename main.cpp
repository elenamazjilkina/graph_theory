#include "stdafx.h"
#include "Graph.h"
#include <iostream> 
#include <fstream>  

using namespace std;

int main()
{
	Graph graph;

	string choice;

	do
	{
		cout << "\n\nWhat would you like to do?\n";
		cout << "[1] Read graph from file\n";
		cout << "[2] Save graph as\n";
		cout << "[3] Print current graph\n";
		cout << "[4] Add an edge\n";
		cout << "[5] Change an edge\n";
		cout << "[6] Remove an edge\n";
		cout << "[7] Add a node\n";
		cout << "[8] Remove a node\n";
		cout << "[0] Exit\n";

		cin >> choice;

		if (choice == "1")
		{
			cout << "Reading graph...\n";

			string fileName;

			cout << "\tEnter file name > ";
			cin >> fileName;

			graph.readGraph(fileName);
		}
		else if (choice == "2")
		{
			cout << "Saving graph...\n";

			string fileName;

			cout << "\tEnter file name > ";
			cin >> fileName;

			graph.writeGraph(fileName);
		}
		else if (choice == "3")
		{
			cout << "Current graph...\n";

			graph.printGraph();
		}
		else if (choice == "4")
		{
			cout << "Adding an edge...\n";
			int from = 0;
			int to = 0;
			int weight = 0;

			cout << "\tEnter \"from\" vertex > ";
			cin >> from;

			cout << "\tEnter \"to\" vertex > ";
			cin >> to;

			if (graph.is_weighted())
			{
				cout << "\tEnter a weight > ";
				cin >> weight;
			}

			graph.addEdge(from, to, weight);
		}
		else if (choice == "5")
		{
			cout << "Changing an edge...\n";

			int from = 0;
			int to = 0;
			int newWeight = 0;
			int oldWeight = 0;

			cout << "\tEnter \"from\" vertex > ";
			cin >> from;

			cout << "\tEnter \"to\" vertex > ";
			cin >> to;

			cout << "\tEnter a new weight > ";
			cin >> newWeight;

			oldWeight = graph.changeEdge(from, to, newWeight);

			cout << "\tThe previous weight of " << from << "-" << to << " edge was " << oldWeight << "\n";
		}
		else if (choice == "6")
		{
			cout << "Removing an edge...\n";

			int from = 0;
			int to = 0;

			cout << "\tEnter \"from\" vertex > ";
			cin >> from;

			cout << "\tEnter \"to\" vertex > ";
			cin >> to;

			graph.removeEdge(from, to);
		}
		else if (choice == "7")
		{
			cout << "Add a node...\n";

			graph.addNode();
		}
		else if (choice == "8")
		{
			cout << "Removing a node...\n";

			int from = 0;

			cout << "\tEnter node > ";
			cin >> from;

			graph.removeNode(from);
		}
		else if (choice == "1a")
		{
			graph.task_1a();
		}
		else if (choice == "1b")
		{
			graph.task_1b();
		}
		else if (choice == "2a")
		{
			graph.task_2a();
		}
		else if (choice == "2b")
		{
			graph.task_2b();
		}
		else if (choice == "3a")
		{
			graph.task_3();
		}
		else if (choice == "4a")
		{
			graph.task_4a();
		}
		else if (choice == "4b")
		{
			graph.task_4b();
		}
		cout << "\ndone\n";
	} while (choice != "0");

	return 0;
}