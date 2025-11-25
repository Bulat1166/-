#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;
class Car {
public:
	string brand;
	string model;
	Car(const string& b, const string& m) :brand(b), model(m) {};
	~Car() {
		cout << "Сработал деструктор:" << brand << model << endl;

	}

};
int main(){
	setlocale(LC_ALL, "RU");
	vector<Car>cars;
	ifstream file("car.txt");
	if (!file) {
		cout << " Ошибка,файл не найден!" << endl;
		return 1;
	}
	string brand, model;
	while (file >> brand >> model) {
		cars.push_back(Car(brand, model));
	}
	file.close();
	cout << "Список автомобилей:" << endl;
	for (const Car& car : cars) {
		cout << car.brand << " " << car.model << endl;
	}
	cout << "\n Завершение программы:" << endl;
}


