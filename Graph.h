// Файл "Graph.h"
// Заголовочный файл класса Graph (интерфейс)

// Сторожевой define
// Необходим для того, чтобы невозможно было включить этот файл дважды
#ifndef GRAPH_H
#define GRAPH_H

// Команды подключения классов стандартной библиотеки STL
#include <string>   // Строка - последовательность символов
#include <vector>   // Вектор - массив, который может изменять свой размер
#include <set>      // Множество - набор элементов одного типа
#include <map>      // Отображение - набор элементов "ключ-значение"
#include <tuple>    // Кортеж - N элементов, рассматриваемых как единое целое

// Сделать все функции и объекты стандартной библиотеки "видимыми" в текущем коде
using namespace std;

// Класс Graph
// Представляет графы в программе
class Graph
{
	// Откытая (публичная) часть класса, его интерфейс
	// Функции из этой части могут быть вызваны извне класса
public:
	// Конструктор класса
	Graph();

	// Деструктор класса
	~Graph();

	// Считывает граф из файла fileName
	void readGraph(string fileName);

	// Сохраняет граф в файл fileName
	void writeGraph(string fileName);

	// Выводит текущий рабочий граф в консольку
	void printGraph();

	// Добавляет вершину
	void addNode();

	// Удаляет вершину
	void removeNode(int from);

	// Добавляет ребро с весом weight между вершинами from и to
	void addEdge(int from, int to, int weight);

	// Удаляет ребро между вершинами from и to
	void removeEdge(int from, int to);

	// Изменяет вес ребра между вершинами from и to на newWeight
	// Возвращает прежнее значение веса
	int changeEdge(int from, int to, int newWeight);

	bool is_weighted();		// Возвращает флаг взвешенности графа

	void task_1a();
	void task_1b();
	void task_2a();
	void dfs1(int v);
	void dfs2(int v);
	void task_2b();
	void task_3();

	map< int, set< pair< int, int > > > adjList;  // Список смежности
	map< int, set< pair< int, int > > > adjListTransp;  // Список смежности транспонированный
	vector< vector< int > > adjMatrix;            // Матрица смежности
	set< tuple< int, int, int > > edgList;        // Список ребер
	set< tuple< int, int, int > > edgListTransp;        // Список ребер транспонированный
	vector < pair < int, pair<int, int> > > listWeight; //Список ребер для сортирвки по весу в алгоритме Крускала

														// Закрытая (приватная) часть класса
														// Эти функции и данные доступны только внутри класса Graph
private:

	// Данные-члены (поля) класса
	bool weighted;  // Флаг, true, если граф взвешенный, иначе false
	bool oriented;  // Флаг, true, если граф ориентированный, иначе false
	int N;          // Количество вершин
	int M;          // Количество ребер


};

#endif // GRAPH_H