#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

#define MAX 10000000
#define COUNT 26

using namespace std;

class Ford_Falkerson {
	int size;//количество ориентированных ребер
	char source;//исток
	char stock;//сток
	char from;//начальная вершина ребра
	char to;//конечная вершина ребра
	int cost;//величина потока ребра
	int delta = 0;//для сортировки в лексикографическом порядке
	vector<vector<int>> graph;//граф
	vector<vector<int>> flows;//поток
	vector<bool>visited;
	vector<int> way;//путь
public:
	Ford_Falkerson(int digit, char symbol) :size(digit), source(symbol), stock(symbol), from(symbol), to(symbol), cost(digit), delta(digit), graph(COUNT, vector<int>(COUNT, 0)), flows(COUNT, vector<int>(COUNT, 0)),
		visited(COUNT, false), way(COUNT, 0) {}

	void print_graph() {//вывод списка путей с текущим потоком
		cout << " ---------------------------------" << endl;
		cout << "|Путь|Текущий поток/Стоимость пути|" << endl;
		cout << " ---------------------------------" << endl;
		for (int i = 0; i < COUNT; i++) {
			for (int j = 0; j < COUNT; j++) {
				if (graph[i][j] > 0)
					cout << "|" << (char)(i + delta) << "->" << (char)(j + delta) << "|           (" << flows[i][j] << '/' << graph[i][j] << ")            | " << endl;
			}
		}
		cout << " ---------------------------------" << endl;
	}


	void creation_graph() {//создание графа
		cin >> size >> source >> stock;
		if (isalpha(source)) {//буквы
			delta = 97;//ASCII код а
		}
		else {
			delta = 49;//ASCII код 1
		}
		for (int i = 0; i < size; i++)//считываем ребра графа
		{
			cin >> from >> to >> cost;
			graph[from - delta][to - delta] = cost;
		}
	}

	void clear() {//очищает путь
		for (size_t i = 0; i < COUNT; i++) {
			way[i] = 0;
		}
	}

	void DFS(int vertex) {//поиск в глубину
		cout << "Текущая вершина " << char(vertex) << endl;
		visited[vertex - delta] = true;
		for (size_t i = 0; i < visited.size(); i++) {
			//cout << char(vertex) << "->" << endl;
			//перебираем все исходящие из рассматриваемой вершины рёбра
			if (!visited[i] && (graph[vertex - delta][i] - flows[vertex - delta][i] > 0 && graph[vertex - delta][i] != 0 || flows[vertex - delta][i] < 0 && graph[i][vertex - delta] != 0)) {
				//если ребро ведёт в вершину, которая не была рассмотрена ранее, то запускаем алгоритм от этой нерассмотренной вершины
					for (size_t j = 0; j < COUNT; j++) {
						if (graph[vertex - delta][j] > 0)
							cout << (char)(vertex) << "->" << (char)(j + delta) << " (" << flows[vertex - delta][j] << '/' << graph[vertex - delta][j] << ')' << endl;
					}
				way[i] = vertex;//добавляем вершину в путь
				DFS(i + delta);//продолжаем поис в глубину
			}
		}
	}

	bool get_way() {
		cout << "Запускаем поиск в глубину от стока" << endl;
		DFS(source);//поиск в глубину от истока
		for (size_t i = 0; i < visited.size(); i++) {
			visited[i] = false;//убираем просмотренные вершины
		}
		return (way[stock - delta] != 0);//дошел ли путь до стока
	}

	int FF() {//алгоритм форда фалкерсона
		int max_flow = 0;//максимальный поток
		cout << "Строим путь" << endl << endl;
		while (get_way()) {//если доходим до стока
			cout << "Дошли до стока" << endl;
			int tmp = MAX;
			for (int v = stock - delta; 0 <= way[v] - delta; v = way[v] - delta) {
				tmp = min(tmp, graph[way[v] - delta][v] - flows[way[v] - delta][v]);//находим максимальный поток
			}
			cout << "Максимальный поток пути " << tmp << endl;
			for (int v = stock - delta; 0 <= way[v] - delta; v = way[v] - delta) {
				flows[way[v] - delta][v] += tmp;//увеличиваем поток для обратных путей
				flows[v][way[v] - delta] -= tmp;//убавляем поток для текущих путей
			}
			max_flow += tmp;
			print_graph();
			clear();//очищаем путь
		}
		cout << "Дальнейшего пути нет.\nНе дошли до стока" << endl;
		cout << " ---------------------------------" << endl;
		cout << "|           Результат.            |" << endl;
		cout << " ---------------------------------" << endl;
		return max_flow;
	}

	void print_result() {//выводим рёбра графа с фактическим потоком
		for (int i = 0; i < COUNT; i++) {
			for (int j = 0; j < COUNT; j++) {
				if (flows[i][j] != 0 && flows[i][j] < 0)
					flows[i][j] = 0;
				if (graph[i][j] > 0)
					cout << (char)(i + delta) << " " << (char)(j + delta) << " " << flows[i][j] << endl;
			}
		}
		cout << " ---------------------------------" << endl;
	}
};


int main() {
	setlocale(LC_ALL, "Russian");
	Ford_Falkerson A(0, '\0');//создаем пустой граф
	A.creation_graph();//заполняем его
	A.print_graph();
	cout << A.FF() << endl;
	A.print_result();//выводим результат
	system("pause");
	return 0;
}