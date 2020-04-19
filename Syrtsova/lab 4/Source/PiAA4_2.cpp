#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

vector <int> prefix(string arr) {//префикс функция 
	cout << endl << "Вычислим префикс функцию строки" << endl;
	vector <int> p(arr.size());
	size_t j = 0;
	size_t i = 1;
	p[0] = 0;
	cout << "Первое вхождение символа\nПрефикс " << arr[0] << " = 0" << endl;
	while (i < arr.length()) {
		if (arr[i] == arr[j]) {//если символы совпадают
			cout << "Символ " << arr[i] << " совпадает с " << arr[j] << endl;
			p[i] = j + 1;//увеличиваем префикс 
			cout << "Префикс " << arr[i] << " = " << p[i] << endl;
			i++;
			j++;
		}
		else if (j == 0) {//еще не было совпадений
			p[i] = 0;
			cout << "Символ " << arr[i] << " не совпадает с " << arr[0] << endl;
			cout << "Префикс " << arr[i] << " = " << p[i] << endl;
			i++;
		}
		else {//символы не совпадают
			cout << "Символ " << arr[i] << " не совпадает с " << arr[j] << endl;
			j = p[j - 1];
		}
	}
	cout << "Префикс функция строки ";
	for (i = 0; i < arr.size(); i++) cout << p[i];
	cout << endl;
	return p;
}

int CYCLE_KMP(string s1, string s2) {
	size_t i = 0;
	size_t j = 0;
	if (s1.length() != s2.length()) {//длины строк не равны
		cout << "\nДлины строк не равны" << endl;
		return -1;
	}
	else if (s1 == s2) {//строки равны
		cout << "\nСтроки равны" << endl;
		return 0;
	}
	else {//если нет то алгоритм кмп
		vector <int> p = prefix(s2);
		cout << "\nРассмотрим строки на циклический сдвиг" << endl;
		while (true) {
			if (s1[i] == s2[j]) {//символы совпадают
				cout << "Символ " << s1[i] << " совпадает с " << s2[j] << endl;
				i++;
				if (i == s1.length()) {
					i -= s1.length();
				}
				j++;
				if (j == s2.length()) {
					cout << "\nСтроки циклически сдвинуты" << endl;
					cout << "Индекс начала строки " << s1 << " в " << s2 << endl;
					return i;
					//return s2.length() - i;
				}
			}
			else if (j != 0) {//не совпадают, откатываем назад по префиксу
				if (j != s2.length()) cout << "Символ " << s1[i] << " не совпадает с " << s2[j] << endl;
				j = p[j - 1];
				cout << "Вернемся к предыдущему символу префикса строки" << endl;
			}
			else {//не равны
				cout << "Символы " << s1[i] << " и " << s2[j] << " не совпадают" << endl;
				i++;//идем вперед
				if (i == s1.length()) {//если строка закончилась 
					cout << "\nСтроки не циклически сдвинуты" << endl;
					return -1;
				}
			}
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	string a;//строка 1
	string b;//строка 2
	getline(cin, a);
	getline(cin, b);
	cout << CYCLE_KMP(b, a);
	return 0;
}