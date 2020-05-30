#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#define ALP 6

using namespace std;

struct numbers {
	int index;
	int pattern_num;
};

struct bohr_vertex {
	int next_vertex[ALP];//массив вершин, в которые можно попасть из данной
	bool flag;//финальная вершина для шаблона
	int suff_link;//переменная для хранения суффиксной ссылки
	int auto_move[ALP];//массив переходов из одного состояния в другое
	int par;//номер вершины родителя
	char symbol;//символ по которому осуществляется переход от родителя
	int suff_flink;//сжатые суффиксные ссылки
	int pattern_num[40];//номера подшаблонов
};

vector<numbers> num;
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
	memset(vertex.pattern_num, 255, sizeof(vertex.pattern_num));
	return vertex;
}

void init_bohr() {//создание пустого бора
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
	case'G':
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
	for (int i = 0; i < 40; i++) {
		if (bohr[num].pattern_num[i] == -1) {
			bohr[num].pattern_num[i] = pattern.size() - 1;
			break;
		}
	}
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
	cout << "Переход к вершине " << bohr[v].auto_move[ch] << " по символу " << vertex(bohr[bohr[v].auto_move[ch]].symbol) << endl;
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

void check(int v, int i) {//////////////////////////////////////////////////////////////////////////////////////////////////
	struct numbers s;
	for (int u = v; u != 0; u = get_suff_flink(u)) {
		if (bohr[u].flag) {
			for (int j = 0; j < 40; j++) {
				if (bohr[u].pattern_num[j] != -1) {
					s.index = i - pattern[bohr[u].pattern_num[j]].length();
					s.pattern_num = bohr[u].pattern_num[j];
					cout << endl << "Вершина " << u << " конечная для шаблона " << s.pattern_num+1 << endl;
					cout << "Найден подшаблон с номером " << s.pattern_num+1 << ", позиция в тексте " << s.index << endl;
					num.push_back(s);
				}
				else
					break;
			}
		}
		else cout << endl << "Вершина " << u << " не конечная" << endl;
		cout << "Перейдем по сжатой суффиксной ссылке " << vertex(bohr[u].suff_flink) << endl;
	}
}

void find_all_pos(string s) {
	int u = 0;
	for (int i = 0; i < s.length(); i++) {
		cout << endl << "Текущая вершина " << u << endl << "Вычислим функцию переходов." << endl << endl;
		u = get_auto_move(u, find(s[i]));
		cout << endl << "Перейдем по хорошим суффиксным ссылкам вершины  " << u;
		if (i+1!= s.length()) cout << " по символу " << s[i + 1] << endl;
		check(u, i + 1);
	}
	cout << endl << "Проход по строке текста завершен." << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<string> patterns;  //подстроки при делении по джокеру
	vector<int> patterns_pos; //позиции подстрок
	string text;//текст
	string temp;//шаблон
	char joker;//джокер
	string pat;
	cout << "Введите текст:" << endl;
	cin >> text;
	cout << "Введите шаблон:" << endl;
	cin >> temp;
	cout << "Введите джокер:" << endl;
	cin >> joker;
	init_bohr();
	cout << endl << "Разделим шаблон на подстроки без джокера" << endl;
	for (int i = 0; i < temp.length(); i++) {
		if (temp[i] != joker) {
			patterns_pos.push_back(i + 1);
			for (int j = i; temp[j] != joker && j != temp.length(); j++) {
				pat += temp[j];
				i++;
			}
			add_string_to_bohr(pat);
			patterns.push_back(pat);
			pat.clear();
		}
	}
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Индивидуализация:\nВычислим количество дуг из вершин." << endl;
	info();
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << endl << "Найдем шаблон в тексте." << endl;
	find_all_pos(text);
	int arr[10];
	int n = 0;
	vector<int> c(text.length(), 0);
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << endl << "Найдем шаблон с джокерами в тексте" << endl;
	for (int i = 0; i < num.size(); i++) {
		cout << endl << "Текущий подшаблон " << pattern[num[i].pattern_num] << " с индексом в тексте " << num[i].index + 1 << " и индексом в шаблоне " << patterns_pos[num[i].pattern_num] << endl;
		if (num[i].index < patterns_pos[num[i].pattern_num] - 1) {//если индекс подшаблона меньше позиции подшаблона в строке
			cout << "индекс подшаблона равен " << num[i].index + 1 << " и меньше позиции подшаблона в шаблоне " << patterns_pos[num[i].pattern_num] << endl;
			continue;
		}
		c[num[i].index - patterns_pos[num[i].pattern_num] + 1]++;//увеличиваем счетчик количества пропусков
		cout << endl << "Количество пропусков между шаблонами равно " << c[num[i].index - patterns_pos[num[i].pattern_num] + 1] << endl;
		cout << "Количество джокеров между шаблонами равно " << patterns.size() << endl << endl;
		if (c[num[i].index - patterns_pos[num[i].pattern_num] + 1] == patterns.size() && //если количество пропусков равно колличеству джокеров
			num[i].index - patterns_pos[num[i].pattern_num] + 1 <= text.length() - temp.length()) { //и есть место чтобы закончить шаблон
			cout << "Количество джокеров между шаблонами равно количеству пропусков. Найден исходный шаблон в тексте" << endl;
			cout << "На позиции " << num[i].index - patterns_pos[num[i].pattern_num] + 2 << endl;
			arr[n] = num[i].index - patterns_pos[num[i].pattern_num] + 2;
			n++;
		}
		
	}
	system("pause");
	cout << "------------------------------------------------------------------------------" << endl;
	cout << endl << "Ответ:" << endl;
	for (int i = 0; i < n; i++) cout << arr[i] << endl;
	for (int i = n - 1; i >= 0; i--) {
		if (i != 0 && arr[i - 1] + temp.size() - 1 >= arr[i]) 
			text.erase(arr[i - 1] + temp.size() - 1, arr[i]-arr[i-1] );
		else 
			text.erase(arr[i] - 1, temp.size());
	}
	cout << "Строка после удаления найденных шаблонов: " << text << endl;
	return 0;
}