// Файл "Graph.cpp"
// Содержит реализации всех функций, описанных в файле заголовка

#include "stdafx.h"
#include "Graph.h"      // Файл заголовка класса Graph
#include <fstream>      // Файловый ввод-вывод
#include <iostream>     // Консольный ввод-вывод
#include <sstream>      // Строковый ввод-вывод
#include <limits>       // Диапазоны типов
#include <algorithm>    // Стандартные алгоритмы

/*
Конструктор класса Graph. Инициализирует все данные-члены (поля) класса
*/
Graph::Graph()
{
	// Установить значения по умолчанию для всех полей класса
	weighted = false;
	oriented = false;
	N = 0;
	M = 0;
}

/*
Деструктор класса Graphъ
*/
Graph::~Graph()
{
}

/*
Функция чтения графа из файла.
Вершины нумеруются с 0.
*/
void Graph::readGraph(string fileName)
{
	// Создать объект файла и попытаться открыть файл с именем "fileName"
	ifstream file(fileName.c_str());

	// Если не удалось открыть файл, кинуть исключение
	if (!file.is_open())
	{
		cout << "Can't open file " << fileName << "\n";
		return;
	}

	cout << "Reading graph from file " << fileName << "...\t";

	// Очистить текущее содержимое списка смежности
	adjList.clear();

	// Считать количество вершин
	file >> N;

	// Считать индикатор ориентированности графа
	file >> oriented;

	// Считать индикатор взвешенности графа
	file >> weighted;

	// Установить курсор чтения файла на новую строку
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// Считать N связей вершин
	for (int from = 0; from < N; from++)
	{
		// Строка для хранения списка связей текущей вершины
		string rowStr;

		// Считать текущую строку
		getline(file, rowStr);

		// Строковый поток ввода (для разбиения row по отдельным числам)
		istringstream iss(rowStr);

		// Переменная для хранения связей текущей вершины
		set< pair<int, int> > row;

		// Переменная для хранения номера вершины
		int node;

		// Считать вершину
		iss >> ws >> node;

		// Бежать по rowStr
		while (!iss.eof() && !iss.str().empty())
		{

			// Переменная для хранения номера соседней вершины
			int to;

			// Считать соседнюю вершину
			iss >> ws >> to;

			// Переменная для хранения веса ребра
			int weight;

			// Если описан взвешенный граф
			if (weighted)
			{
				// Считать вес ребра
				iss >> ws >> weight;
			}

			// Сохранить считанные связи в список связей
			row.insert(make_pair(to, weight));

		};
		// Игнорировать оставшиеся символы в строке
		iss.ignore(128);

		// Добавить список связей считанной вершины в список смежности
		adjList[node] = row;
	}

	cout << "done\n";

	// Напечатать граф
	printGraph();
}


void Graph::addNode()
{
	cout << "Adding node " << "...\t";

	//создать новую вершину
	adjList[N];

	// Увеличить счетчик вершин
	N++;

	cout << "done\n";

	// Напечатать граф
	printGraph();
}

void Graph::removeNode(int from)
{
	cout << "Removing node " << from << "...\t";

	// Итератор отображения - указывает на список связей вершины в списке смежности
	map< int, set< pair<int, int> > >::iterator fromIt;

	// Итератор множества - указывает на соседние вершины в списке связей множества
	set< pair<int, int> >::iterator toIt;

	// Найти в списке смежности начало ребра
	fromIt = adjList.find(from);

	// Количество удаляемых ребер
	int count_edge = 0; 

	// Если найдено
	if (fromIt != adjList.end())
	{
		// Подсчитываем, сколько ребер у удаляемой вершины
		count_edge = adjList.count(from);
		
		//Удаляем ее
		adjList.erase(from);

		// Бежать по строкам
		for (map< int, set< pair<int, int> > >::iterator It = adjList.begin(); It != adjList.end(); It++)
		{

			// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
			toIt = find_if(It->second.begin(), It->second.end(), [&from](const pair<int, int>& edge)
			{
				if (edge.first == from) return true;
				return false;
			});

			// Если удалось что-то найти
			if (toIt != It->second.end())
			{
				// Удалить требуемое ребро
				It->second.erase(toIt);
				count_edge++;
			}
		}
	}
	else //если нет такой вершины
	{
		// Сообщить об ошибке
		cout << "\nCan't removing node " << from << ", the node doesn't exists\n";
		return;
	}

	// Уменьшить счетчик вершин
	N--;

	// Уменьшить счетчик ребер
	M -= count_edge;

	cout << "Deleting edge from this node: " << count_edge << "\n";

	cout << "done\n";

	// Напечатать граф
	printGraph();
}


void Graph::addEdge(int from, int to, int weight)
{
	cout << "Adding edge from " << from << " to " << to << ", weight is " << weight << "...\t";

	// Итератор отображения - указывает на список связей вершины в списке смежности
	map< int, set< pair<int, int> > >::iterator fromIt;

	// Найти в списке смежности начало ребра
	fromIt = adjList.find(from);

	// Если не найдено
	if (fromIt == adjList.end())
	{
		// Сообщить об ошибке
		cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
		return;
	}

	// Итератор множества - указывает на соседние вершины в списке связей множества
	set< pair<int, int> >::iterator toIt;

	// Если граф взвешенный
	if (weighted)
	{
		// Найти соседей текущей вершины, используя указанный вес
		toIt = fromIt->second.find(make_pair(to, weight));
	}
	else    // // Если граф невзвешенный
	{
		// Найти соседей текущей вершины, используя в качестве веса 0
		toIt = fromIt->second.find(make_pair(to, 0));
	}

	// Если удалось что-то найти
	if (toIt != fromIt->second.end())
	{
		// Сообщить об ошибке
		cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
		return;
	}

	// Если граф взвешенный
	if (weighted)
	{
		// Вставить новую соседнюю вершину, назначить ребру указанный вес
		fromIt->second.insert(make_pair(to, weight));
	}
	else    // Если граф невзвешенный
	{
		// Вставить новую соседнюю вершину, назначить ребру вес 0
		fromIt->second.insert(make_pair(to, 0));
	}

	// Если граф неориентированный, то добавить также ребро в обратном направлении
	if (!oriented)
	{
		// Если граф взвешенный
		if (weighted)
		{
			// Использовать указанный вес
			adjList[to].insert(make_pair(from, weight));
		}
		else    // Если граф невзвешенный
		{
			// Использовать вес 0
			adjList[to].insert(make_pair(from, 0));
		}
	}


	// Увеличить счетчик ребер
	M++;

	cout << "done\n";

	// Напечатать граф
	printGraph();
}


void Graph::removeEdge(int from, int to)
{
	cout << "Removing edge from " << from << " to " << to << "...\t";

	// Итератор отображения - указывает на список связей вершины в списке смежности
	map< int, set< pair<int, int> > >::iterator fromIt;

	// Итератор множества - указывает на соседние вершины в списке связей множества
	set< pair<int, int> >::iterator toIt;

	// Найти в списке смежности начало ребра
	fromIt = adjList.find(from);

	// Если найдено
	if (fromIt != adjList.end())
	{
		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
			[&to](const pair<int, int>& edge)
		{
			if (edge.first == to) return true;
			return false;
		});

		// Если удалось что-то найти
		if (toIt != fromIt->second.end())
		{
			// Удалить требуемое ребро
			fromIt->second.erase(toIt);
		}
	}

	// Если граф неориентированный, то удалить также ребро в обратном направлении
	if (!oriented)
	{
		// Найти в списке смежности начало ребра
		fromIt = adjList.find(to);

		// Если не найдено
		if (fromIt == adjList.end())
		{
			// Вершина не существует, удалять нечего
			return;
		}

		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
			[&from](const pair<int, int>& edge)
		{
			if (edge.first == from) return true;
			return false;
		});

		// Если не удалось ничего найти
		if (toIt == fromIt->second.end())
		{
			// Сообщить об ошибке (в неориентированном графе ребро должно быть в обоих направлениях)
			cout << "\nCan't remove edge from " << to << " to " << from << ", the edge doesn't exists\n";
			return;
		}

		// Удалить требуемое ребро
		fromIt->second.erase(toIt);
	}

	// Уменьшить счетчик ребер
	M--;

	cout << "done\n";

	// Напечатать граф
	printGraph();
}



int Graph::changeEdge(int from, int to, int newWeight)
{
	// Если граф невзвешенный
	if (weighted == false)
	{
		// Сообщить об ошибке (у ребер нет весов, модифицировать нечего)
		cout << "\tSorry, you can't modify an edge weight of non-weighted graph\n";
		return -1;
	}
	else 
	{
		cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";
		int oldWeight = 0;

		// Итератор отображения - указывает на список связей вершины в списке смежности
		map< int, set< pair<int, int> > >::iterator fromIt;

		// Найти в списке смежности начало ребра
		fromIt = adjList.find(from);

		// Если не найдено
		if (fromIt == adjList.end())
		{
			// Сообщить об ошибке
			cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
			return -1;
		}

		fromIt->second.insert(make_pair(1, 2));

		// Итератор множества - указывает на соседние вершины в списке связей множества
		set< pair<int, int> >::iterator toIt;

		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо изменить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
			[&to](const pair<int, int>& edge)
		{
			if (edge.first == to) return true;
			return false;
		});

		// Если не удалось ничего найти
		if (toIt == fromIt->second.end())
		{
			// Сообщить об ошибке
			cout << "\nCan't change edge from " << from << " to " << to << ", the edge doesn't' exists\n";
			return -1;
		}

		// Сохранить прежнее значение веса
		oldWeight = toIt->second;

		// Назначить ребру указанный вес
		pair<int, int> copy = *toIt;
		copy.second = newWeight;
		fromIt->second.erase(toIt);
		fromIt->second.insert(copy);

		// Если граф неориентированный, то изменить также ребро в обратном направлении
		if (!oriented)
		{
			// Найти в списке смежности начало ребра
			fromIt = adjList.find(to);

			// Если не найдено
			if (fromIt == adjList.end())
			{
				// Вершина не существует, удалять нечего
				return -1;
			}

			// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
			toIt = find_if(fromIt->second.begin(), fromIt->second.end(),
				[&from](const pair<int, int>& edge)
			{
				if (edge.first == from) return true;
				return false;
			});

			// Если не удалось ничего найти
			if (toIt == fromIt->second.end())
			{
				// Сообщить об ошибке (в неориентированном графе ребро должно быть в обоих направлениях)
				cout << "\nCan't change edge from " << to << " to " << from << ", the edge doesn't exists\n";
				return -1;
			}

			// Назначить ребру указанный вес
			pair<int, int> copy = *toIt;
			copy.second = newWeight;
			fromIt->second.erase(toIt);
			fromIt->second.insert(copy);
		}

		cout << "done\n";

		// Напечатать граф
		printGraph();

		// Вернуть прежнее значение веса
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

	// Отобразить флаг ориентированности
	if (oriented)
	{
		cout << "oriented, ";
	}
	else
	{
		cout << "not oriented, ";
	}

	// Отобразить флаг взвешенности
	if (weighted)
	{
		cout << "weighted, ";
	}
	else
	{
		cout << "not weighted, ";
	}

	cout << "adjacency list)...\t\n";

	// Бежать по строкам
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++)
	{
		cout << "node " << fromIt->first << ": ";

		// Бежать по столбцам
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
		{
			// Напечатать конец текущего ребра
			cout << "to " << toIt->first;

			// Если граф взвешенный
			if (weighted)
			{
				// Напечатать вес текущего ребра
				cout << " weight: " << toIt->second << ", ";
			}

			cout << " ";
		}

		cout << "\n";
	}
}


void Graph::writeGraph(string fileName)
{
	// Создать объект файла и попытаться открыть файл с именем "fileName"
	ofstream file(fileName.c_str());

	// Если не удалось открыть файл, кинуть исключение
	if (!file.is_open())
	{
		cout << "Can't open file " << fileName << "\n";
		return;
	}

	cout << "Writing graph to file " << fileName << "...\t";

	// Сохранить количество вершин
	file << N << "\n";

	// Сохранить флаг ориентированности графа
	file << oriented << " ";

	// Сохранить флаг взвешенности графа
	file << weighted << "\n";

	// Бежать по строкам
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++)
	{
		file << fromIt->first << " ";

		// Бежать по столбцам
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
		{
			// Напечатать конец текущего ребра
			file << toIt->first;

			// Если граф взвешенный
			if (weighted)
			{
				// Напечатать вес текущего ребра
				file << " " << toIt->second;
			}

			file << " ";
		}

		file << "\n";
	}

	cout << "done\n";
}

void Graph::task_1a()
{
	if (oriented) 
	{
		cout << "Enter the node: ";
		int from = 0;
		cin >> from;
		map< int, set< pair<int, int> > >::iterator fromIt;
		// Найти в списке смежности вершину
		fromIt = adjList.find(from);
		// Если нашли 
		if (fromIt != adjList.end()) {
			cout << "Outgoing nodes: ";
			if (fromIt->second.size() == 0) 
			{
				cout << "This node has no neighbors\n";
			}
			else
			{
				// Бежать по столбцам
				for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
				{
					// Напечатать выходящую вершину
					cout << toIt->first << " ";
				}
			}
		}
		else
		{
			cout << "\nThis node doesn't exists\n";
		}
	}
	else
	{
		cout << "\nThe task is impossible. Graph is non-oriented\n";
	}
}