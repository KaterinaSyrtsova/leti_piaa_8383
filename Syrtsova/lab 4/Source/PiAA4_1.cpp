#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector <int> prefix(string arr) {//префикс функция от подстроки
	cout << endl << "Вычислим префикс функцию подстроки" << endl;
	vector <int> p(arr.size());
	size_t j = 0;
	size_t i = 1;
	p[0] = 0;
	cout << "Первое вхождение символа\nПрефикс " << arr[0] << " = 0"<< endl;
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

vector <int> KMP(string form, string line) {
	vector <int> res;
	vector <int> p = prefix(form);//находим префикс
	size_t i = 0;
	size_t j = 0;
	cout << endl << "Алгоритм KMP" << endl;
	while (i < line.length()) {
		if (line[i] == form[j]) {//если символы равны
			cout << "Символ " << line[i] << " совпадает с " << form[j] << endl;
			i++;
			j++;
			if (j == form.length()) {
				res.push_back(i - form.length());
			}
		}
		else if (j != 0) {
			if (j!=form.length()) cout << "Символ " << line[i] << " не совпадает с " << form[j] << endl;
			j = p[j - 1];
			cout << "Вернемся к предыдущему символу префикса в подстроке" << endl;
		}
		else {
			cout << "Символы не совпадают" << endl;
			i++;
		}
	}
	return res;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string form;//подстрока
	string line;//строка
	getline(cin, form);
	getline(cin, line);
	vector <int> res = KMP(form, line);
	if (res.size() == 0) {
		cout << "\nПодстрока " << form << " не входит в строку " << line << endl;
		cout << "-1";
	}
	else {
		cout << "\nИндексы начала вхождений " << form << " в " << line << endl;
		for (size_t i = 0; i < res.size(); i++) {
			cout << res[i];
			if (i != res.size() - 1) {
				cout << ",";
			}
		}
	}
	return 0;
}