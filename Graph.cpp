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
Деструктор класса Graph
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
	if (!file.is_open()) {
		cout << "Can't open file " << fileName << "\n";
		return;
	}

	cout << "Reading graph from file " << fileName << "...\t";

	// Очистить текущее содержимое списка смежности
	adjList.clear();
	// Очистить текущее содержимое транспонированного списка смежности
	adjListTransp.clear();

	// Считать количество вершин
	file >> N;

	// Считать индикатор ориентированности графа
	file >> oriented;

	// Считать индикатор взвешенности графа
	file >> weighted;

	// Установить курсор чтения файла на новую строку
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// Считать N связей вершин
	for (int from = 0; from < N; from++) {
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
		iss >> node;

		// Бежать по rowStr
		while (!iss.eof() && !iss.str().empty()) {
			// Переменная для хранения номера соседней вершины
			int to;

			// Переменная для хранения веса ребра
			int weight;

			// Считать соседнюю вершину
			iss >> to;

			// Если описан взвешенный граф
			if (weighted)
				// Считать вес ребра
				iss >> weight;

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
	if (fromIt != adjList.end()) {
		// Подсчитываем, сколько ребер у удаляемой вершины
		count_edge = adjList.count(from);

		//Удаляем ее
		adjList.erase(from);

		// Бежать по строкам
		for (map< int, set< pair<int, int> > >::iterator It = adjList.begin(); It != adjList.end(); It++) {

			// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
			toIt = find_if(It->second.begin(), It->second.end(), [&from](const pair<int, int>& edge) {
				if (edge.first == from) return true;
				return false;
			});

			// Если удалось что-то найти
			if (toIt != It->second.end()) {
				// Удалить требуемое ребро
				It->second.erase(toIt);
				count_edge++;
			}
		}
	}
	else { //если нет такой вершины 
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
	if (fromIt == adjList.end()) {
		// Сообщить об ошибке
		cout << "\nCan't add edge from " << from << " to " << to << ", the node doesn't exists\n";
		return;
	}

	// Итератор множества - указывает на соседние вершины в списке связей множества
	set< pair<int, int> >::iterator toIt;

	// Если граф взвешенный
	if (weighted)
		// Найти соседей текущей вершины, используя указанный вес
		toIt = fromIt->second.find(make_pair(to, weight));
	else    // // Если граф невзвешенный
			// Найти соседей текущей вершины, используя в качестве веса 0
		toIt = fromIt->second.find(make_pair(to, 0));

	// Если удалось что-то найти
	if (toIt != fromIt->second.end()) {
		// Сообщить об ошибке
		cout << "\nCan't add edge from " << from << " to " << to << ", the edge already exists\n";
		return;
	}

	// Если граф взвешенный
	if (weighted)
		// Вставить новую соседнюю вершину, назначить ребру указанный вес
		fromIt->second.insert(make_pair(to, weight));
	else    // Если граф невзвешенный
			// Вставить новую соседнюю вершину, назначить ребру вес 0
		fromIt->second.insert(make_pair(to, 0));

	// Если граф неориентированный, то добавить также ребро в обратном направлении
	if (!oriented) {
		// Если граф взвешенный
		if (weighted)
			// Использовать указанный вес
			adjList[to].insert(make_pair(from, weight));
		else    // Если граф невзвешенный
				// Использовать вес 0
			adjList[to].insert(make_pair(from, 0));
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
	if (fromIt != adjList.end()) {
		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&to](const pair<int, int>& edge) {
			if (edge.first == to) return true;
			return false;
		});

		// Если удалось что-то найти
		if (toIt != fromIt->second.end())
			// Удалить требуемое ребро
			fromIt->second.erase(toIt);
	}

	// Если граф неориентированный, то удалить также ребро в обратном направлении
	if (!oriented) {
		// Найти в списке смежности начало ребра
		fromIt = adjList.find(to);

		// Если не найдено
		if (fromIt == adjList.end())
			// Вершина не существует, удалять нечего
			return;

		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&from](const pair<int, int>& edge) {
			if (edge.first == from) return true;
			return false;
		});

		// Если не удалось ничего найти
		if (toIt == fromIt->second.end()) {
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
	if (weighted == false) {
		// Сообщить об ошибке (у ребер нет весов, модифицировать нечего)
		cout << "\tSorry, you can't modify an edge weight of non-weighted graph\n";
		return -1;
	}
	else {
		cout << "Changing edge from " << from << " to " << to << ", new weight is " << newWeight << "...\t";
		int oldWeight = 0;

		// Итератор отображения - указывает на список связей вершины в списке смежности
		map< int, set< pair<int, int> > >::iterator fromIt;

		// Найти в списке смежности начало ребра
		fromIt = adjList.find(from);

		// Если не найдено
		if (fromIt == adjList.end()) {
			// Сообщить об ошибке
			cout << "\nCan't change edge from " << from << " to " << to << ", the node doesn't exists\n";
			return -1;
		}

		fromIt->second.insert(make_pair(1, 2));

		// Итератор множества - указывает на соседние вершины в списке связей множества
		set< pair<int, int> >::iterator toIt;

		// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо изменить
		toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&to](const pair<int, int>& edge) {
			if (edge.first == to) return true;
			return false;
		});

		// Если не удалось ничего найти
		if (toIt == fromIt->second.end()) {
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
		if (!oriented) {
			// Найти в списке смежности начало ребра
			fromIt = adjList.find(to);

			// Если не найдено
			if (fromIt == adjList.end())
				// Вершина не существует, удалять нечего
				return -1;

			// Найти среди соседей вершину, являющуюся концом ребра, которое необходимо удалить
			toIt = find_if(fromIt->second.begin(), fromIt->second.end(), [&from](const pair<int, int>& edge) {
				if (edge.first == from) return true;
				return false;
			});

			// Если не удалось ничего найти
			if (toIt == fromIt->second.end()) {
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
		cout << "oriented, ";
	else
		cout << "not oriented, ";

	// Отобразить флаг взвешенности
	if (weighted)
		cout << "weighted, ";
	else
		cout << "not weighted, ";

	cout << "adjacency list)...\t\n";

	// Бежать по строкам
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		cout << "node " << fromIt->first << ": ";
		// Бежать по столбцам
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) {
			// Напечатать конец текущего ребра
			cout << "to " << toIt->first;

			// Если граф взвешенный
			if (weighted)
				// Напечатать вес текущего ребра
				cout << " weight: " << toIt->second << ", ";
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
	if (!file.is_open()) {
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
	for (map< int, set< pair<int, int> > >::iterator fromIt = adjList.begin(); fromIt != adjList.end(); fromIt++) {
		file << fromIt->first << " ";
		// Бежать по столбцам
		for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++) {
			// Напечатать конец текущего ребра
			file << toIt->first;

			// Если граф взвешенный
			if (weighted)
				// Напечатать вес текущего ребра
				file << " " << toIt->second;
			file << " ";
		}
		file << "\n";
	}
	cout << "done\n";
}


/*----------------------------------------------РЕАЛИЗАЦИЯ ЗАДАЧ----------------------------------------------------*/


/*Для данной вершины орграфа вывести все "выходящие" вершины*/
void Graph::task_1a()
{
	if (oriented) {
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
				cout << "This node has no neighbors\n";
			else {
				// Бежать по столбцам
				for (set< pair<int, int> >::iterator toIt = fromIt->second.begin(); toIt != fromIt->second.end(); toIt++)
					// Напечатать выходящую вершину
					cout << toIt->first << " ";
			}
		}
		else
			cout << "\nThis node doesn't exists\n";
	}
	else
		cout << "\nThe task is impossible. Graph is non-oriented\n";
}


/*Выяснить, совпадают ли два заданных графа*/
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



/*Выяснить, является ли орграф сильно связным*/

vector<char> used; //массив пометок  
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
	// Транспонирование списка смежности 
	// Сначала переводим его в транспонированный список ребер
	edgListTransp.clear();
	for (map< int, set< pair< int, int > > >::iterator i = adjList.begin(); i != adjList.end(); i++) {
		for (set< pair< int, int > >::iterator j = i->second.begin(); j != i->second.end(); j++) {
			edgListTransp.insert(make_tuple(j->first, i->first, j->second));
		}
	}
	//Транспонированный список ребер переводим обратно в список смежности 
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


/*Вывести кратчайшие пути из заданной вершины во все остальные*/

void Graph::task_2b()
{
	const int INF = 1000000000;
	int s; //стартовая вершина 
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


/*Алгоритм Крускала для выявления минимального остовного дерева*/

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


/*Эксцентриситет вершины — максимальное расстояние из всех минимальных расстояний от других вершин до данной вершины. 
Радиус графа — минимальный из эксцентриситетов его вершин. 
Найти центр графа — множество вершин, эксцентриситеты которых равны радиусу графа*/

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