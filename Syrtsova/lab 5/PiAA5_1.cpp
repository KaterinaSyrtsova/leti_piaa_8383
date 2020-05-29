#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#define ALP 6

using namespace std;

struct bohr_vertex {//структура вершины бора
	int next_vertex[ALP];//массив вершин, в которые можно попасть из данной
	int path_num;//номер шаблона
	bool flag;//финальная вершина для шаблона
	int suff_link;//переменная для хранения суффиксной ссылки
	int auto_move[ALP];//массив переходов из одного состояния в другое
	int par;//номер вершины родителя
	char symbol;//символ по которому осуществляется переход от родителя
	int suff_flink;//сжатые суффиксные ссылки
};

vector <bohr_vertex> bohr;//бор
vector <string> pattern;//шаблоны

bohr_vertex make_bohr_vertex(int par, char symbol) {//создание вершины бора
	bohr_vertex vertex;
	memset(vertex.next_vertex, 255, sizeof(vertex.next_vertex));
	vertex.flag = false;
	vertex.suff_link = -1;
	memset(vertex.auto_move, 255, sizeof(vertex.auto_move));
	vertex.par = par;
	vertex.symbol = symbol;
	vertex.suff_flink = -1;
	return vertex;
}

void init_bohr() {//создает пустой бор
	bohr.push_back(make_bohr_vertex(-1, -1));
}

int find(char symbol) {//алфавит
	int ch;
	switch (symbol)
	{
	case 'A':
		ch = 0;
		break;
	case 'C':
		ch = 1;
		break;
	case 'G':
		ch = 2;
		break;
	case 'T':
		ch = 3;
		break;
	case 'N':
		ch = 4;
		break;
	default:
		break;
	}
	return ch;
}

char vertex(char v) {//для вывода на консоль
	char ch;
	switch (v)
	{
	case 0:
		ch = 'A';
		break;
	case 1:
		ch = 'C';
		break;
	case 2:
		ch = 'G';
		break;
	case 3:
		ch = 'T';
		break;
	case 4:
		ch = 'N';
		break;
	default:
		ch = '0';
		break;
	}
	return ch;
}

void info() {
	int count[20];
	int max = 0;
	for (int i = 0; i < bohr.size(); i++) {
		count[i] = 0;
		cout << endl << "Вершина номер " << i << endl;
		if (i == 0) cout << "Это корень бора" << endl;
		else cout << "Вершина-родитель с номером " << bohr[i].par << " по символу " << vertex(bohr[i].symbol) << endl;
		cout << "Соседние вершины:" << endl;
		for (int j = 0; j < ALP; j++) {
			if (bohr[i].next_vertex[j] != -1) {
				cout << "Вершина " << bohr[i].next_vertex[j] << " по символу " << vertex(bohr[bohr[i].next_vertex[j]].symbol) << endl;
				count[i]++;
			}
		}
		if (count[i] == 0) cout << "Это финальная вершина." << endl;
		cout << "Суффиксная ссылка: ";
		if (bohr[i].suff_link == -1) cout << "еще не посчитана." << endl;
		else cout << vertex(bohr[i].suff_link) << endl;
	}
	for (int i = 0; i < bohr.size(); i++)
		if (count[i] > max) max = count[i];
	cout << endl << "Максимальное количество дуг, исходящих из одной вершины " << max << endl;
}
 
void add_string_to_bohr(string s) {//вставляет строку в бор
	cout << endl << "Добавляем шаблон \"" << s << "\" в бор." << endl;
	int num = 0;
	for (int i = 0; i < s.length(); i++) {//проходится по строке
		char ch = find(s[i]);//находит номер символа
		if (bohr[num].next_vertex[ch] == -1) {//добавляется новая вершина если её не было
			bohr.push_back(make_bohr_vertex(num, ch));
			bohr[num].next_vertex[ch] = bohr.size() - 1;
			cout << "Добавим новую вершину " << bohr[num].next_vertex[ch] << " по символу " << s[i] << endl;
		}
		else cout << "Вершина по символу " << s[i] << " уже есть в боре" << endl;
		cout << "Перейдем к вершине " << bohr[num].next_vertex[ch] << endl;
		num = bohr[num].next_vertex[ch];//переходим к следующей вершине
	}
	cout << "Финальная вершина шаблона." << endl << endl;
	bohr[num].flag = true;
	pattern.push_back(s);
	bohr[num].path_num = pattern.size() - 1;
}

int get_auto_move(int v, char ch);

int get_suff_link(int v) {//реализует получение суффиксной ссылки для данной вершины
	cout << endl << "Найдем суффиксную ссылку для вершины " << v << endl;
	if (bohr[v].suff_link == -1) {
		if (v == 0 || bohr[v].par == 0) {//если это корень или начало шаблона
			if (v == 0) cout << "Текущая вершина - корень бора. Суффиксная ссылка равна 0." << endl;
			else cout << "Текущая вершина - начало шаблона. Суффиксная ссылка равна 0." << endl;
			bohr[v].suff_link = 0;
		}
		else {
			cout << "Пройдем по суффиксной ссылке предка " << bohr[v].par << " и запустим переход по символу " << vertex(bohr[v].symbol) << endl;
			bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par), bohr[v].symbol);  //пройдем по суф.ссылке предка и запустим переход по символу.
			cout << "Значит суффиксная ссылка для вершины " << v << " равна " << bohr[v].suff_link << endl << endl;
		}
	}
	else cout << "Суффиксная ссылка для вершины " << v << " равна " << bohr[v].suff_link << endl << endl;
	return bohr[v].suff_link;
}

int get_auto_move(int v, char ch) {             //вычисляемая функция переходов
	if (bohr[v].auto_move[ch] == -1) {
		if (bohr[v].next_vertex[ch] != -1) {            //если из текущей вершины есть ребро с символом ch
			cout << "Из вершины " << v << " есть ребро с символом " << vertex(ch) << endl;
			cout << "Переходим по этому ребру в вершину " << bohr[v].next_vertex[ch] << endl;
			bohr[v].auto_move[ch] = bohr[v].next_vertex[ch];    //то идем по нему
		}
		else {//если нет
			if (v == 0) {//если это корень бора
				//cout << "Текущая вершина " << vertex(bohr[v].symbol) << " - корень бора." << endl;
				bohr[v].auto_move[ch] = 0;
			}
			else {
				cout << "Из вершины " << v << " нет ребра с символом " << vertex(ch) << endl;
				cout << "Перейдем по суффиксной ссылке." << endl << endl;
				bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v), ch);  //иначе перейдем по суффиксальной ссылке
			}
		}
	}
	return bohr[v].auto_move[ch];
}

int get_suff_flink(int v) {//функция вычисления сжатых суффиксальных ссылок
	if (bohr[v].suff_flink == -1) {
		int u = get_suff_link(v);
		if (u == 0) {//если корень или начало шаблона
			bohr[v].suff_flink = 0;
		}
		else {//если по суффиксальной ссылке конечная вершина-равен суффиксальной ссылке, если нет-рекурсия.
			bohr[v].suff_flink = (bohr[u].flag) ? u : get_suff_flink(u); 
		}
	}
	return bohr[v].suff_flink;
}

void check(int v, int i) {
	for (int u = v; u != 0; u = get_suff_flink(u)) {
		if (bohr[u].flag) {
			cout << endl << "Найден шаблон с номером " << bohr[u].path_num + 1 << ", позиция в тексте " << i - pattern[bohr[u].path_num].length() + 1 << endl;
		}
	}
}

void find_all_pos(string s) {//поиск шаблонов в строке
	int u = 0;//текущая вершина
	int n = 0;
	int arr[20][2];
	cout << endl << "Вычислим функции переходов." << endl << endl;
	for (int i = 0; i < s.length(); i++) {
		u = get_auto_move(u, find(s[i]));
		for (int v = u; v != 0; v = get_suff_flink(v)) {
			if (bohr[v].flag) {
				cout << v << endl;
				arr[n][0] = i - pattern[bohr[v].path_num].length() + 2;//?
				arr[n][1] = bohr[v].path_num + 1;
				n++;
			}
		}
		check(u, i + 1);//отмечаем по сжатым суффиксным ссылкам строки, которые нам встретились и их позицию
	}
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << endl << "Ответ:" << endl << "Позиция в тексте/номер шаблона" << endl;
	for (int i = 0; i < n; i++) 
		cout << arr[i][0] << " " << arr[i][1] << endl;
	for (int i = n-1; i >=0; i--) {
		string::const_iterator sub = find_end(s.begin(), s.end(), (pattern[arr[i][1] - 1]).begin(), (pattern[arr[i][1] - 1]).end());
		if (sub != s.end()) {
			if (i != 0 && arr[i - 1][0] + pattern[arr[i - 1][1] - 1].size() - 1 >= arr[i][0])
				s.erase(sub + arr[i - 1][0] + pattern[arr[i - 1][1]-1].size() - arr[i][0], sub + pattern[arr[i][1] - 1].size());
			else
				s.erase(sub, sub + pattern[arr[i][1] - 1].size());
		}
	}
	cout << "Строка после удаления найденных шаблонов: " << s << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string text;
	int n;//количество шаблонов
	init_bohr();
	cout << "Введите текст:" << endl;
	cin >> text;
	cout << "Введите количество шаблонов:" << endl;
	cin >> n;
	cout << "Введите набор шаблонов:" << endl;
	for (int i = 0; i < n; i++) {
		string temp;//шаблон
		cin >> temp;
		add_string_to_bohr(temp);
	}
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Индивидуализация:\nВычислим количество дуг из вершин." << endl;
	info();
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << endl << "Найдем шаблоны в тексте." << endl;
	find_all_pos(text);
	system("pause");
	return 0;
}