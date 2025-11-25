#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Базовый класс для ошибок в программе
class ProgramError {
protected:
    string errorMessage;

public:
    ProgramError(const string& message) : errorMessage(message) {}
    virtual ~ProgramError() = default;

    virtual string getErrorMessage() const {
        return errorMessage;
    }

    virtual void printError() const {
        cout << "Ошибка: " << errorMessage << endl;
    }
};

// Производные классы ошибок
class MemoryError : public ProgramError {
public:
    MemoryError() : ProgramError("Недостаточно памяти") {}
};

class IOError : public ProgramError {
public:
    IOError() : ProgramError("Ошибка ввода/вывода") {}
};

class FileReadError : public ProgramError {
public:
    FileReadError() : ProgramError("Ошибка чтения файла") {}
};

class FileWriteError : public ProgramError {
public:
    FileWriteError() : ProgramError("Ошибка записи файла") {}
};

// Шаблонный класс для хранения массива указателей на объекты
template<typename T>
class PointerArray {
private:
    vector<T*> pointers;

public:
    // Конструкторы
    PointerArray() = default;

    PointerArray(initializer_list<T*> initList) : pointers(initList) {}

    // Деструктор
    ~PointerArray() {
        for (auto ptr : pointers) {
            delete ptr;
        }
    }

    // Конструктор копирования
    PointerArray(const PointerArray& other) {
        for (auto ptr : other.pointers) {
            if (ptr) {
                pointers.push_back(new T(*ptr));
            }
            else {
                pointers.push_back(nullptr);
            }
        }
    }

    // Оператор присваивания
    PointerArray& operator=(const PointerArray& other) {
        if (this != &other) {
            // Очищаем текущие данные
            for (auto ptr : pointers) {
                delete ptr;
            }
            pointers.clear();

            // Копируем новые данные
            for (auto ptr : other.pointers) {
                if (ptr) {
                    pointers.push_back(new T(*ptr));
                }
                else {
                    pointers.push_back(nullptr);
                }
            }
        }
        return *this;
    }

    // Перегрузка оператора [] для доступа к элементам
    T*& operator[](size_t index) {
        if (index >= pointers.size()) {
            throw out_of_range("Индекс выходит за границы массива");
        }
        return pointers[index];
    }

    // Константная версия оператора []
    const T* operator[](size_t index) const {
        if (index >= pointers.size()) {
            throw out_of_range("Индекс выходит за границы массива");
        }
        return pointers[index];
    }

    // Добавление указателя в массив
    void addPointer(T* ptr) {
        pointers.push_back(ptr);
    }

    // Получение размера массива
    size_t size() const {
        return pointers.size();
    }

    // Удаление указателя по индексу
    void removeAt(size_t index) {
        if (index < pointers.size()) {
            delete pointers[index];
            pointers.erase(pointers.begin() + index);
        }
    }

    // Очистка массива
    void clear() {
        for (auto ptr : pointers) {
            delete ptr;
        }
        pointers.clear();
    }
};

// Демонстрационный класс для тестирования шаблона с другим типом
class DemoClass {
private:
    string name;
    int value;

public:
    DemoClass(const string& n, int v) : name(n), value(v) {}

    void display() const {
        cout << "DemoClass: " << name << " (значение: " << value << ")" << endl;
    }

    string getName() const { return name; }
    int getValue() const { return value; }
};

int main() {
    setlocale(LC_ALL, "RU");

    cout << "=== ДЕМОНСТРАЦИЯ РАБОТЫ С КЛАССАМИ ОШИБОК ===" << endl << endl;

    // Создание массива указателей на объекты ProgramError
    PointerArray<ProgramError> errorArray;

    // Добавление различных типов ошибок в массив
    errorArray.addPointer(new MemoryError());
    errorArray.addPointer(new IOError());
    errorArray.addPointer(new FileReadError());
    errorArray.addPointer(new FileWriteError());

    cout << "Содержимое массива ошибок:" << endl;
    // Демонстрация работы перегруженного оператора []
    for (size_t i = 0; i < errorArray.size(); ++i) {
        cout << i << ": ";
        errorArray[i]->printError();
    }

    cout << endl << "=== ДОСТУП К КОНКРЕТНЫМ ЭЛЕМЕНТАМ ЧЕРЕЗ OPERATOR[] ===" << endl;
    // Прямой доступ к элементам через оператор []
    cout << "Первая ошибка: ";
    errorArray[0]->printError();

    cout << "Последняя ошибка: ";
    errorArray[errorArray.size() - 1]->printError();

    cout << endl << "=== ПРОВЕРКА ОБРАБОТКИ ОШИБОК ===" << endl;
    try {
        // Попытка доступа к несуществующему элементу
        cout << "Попытка доступа к элементу с индексом 10..." << endl;
        errorArray[10]->printError();
    }
    catch (const out_of_range& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }

    cout << endl << "=== ДЕМОНСТРАЦИЯ С ДРУГИМ КЛАССОМ ===" << endl;

    // Создание массива указателей на DemoClass
    PointerArray<DemoClass> demoArray;

    demoArray.addPointer(new DemoClass("Объект 1", 100));
    demoArray.addPointer(new DemoClass("Объект 2", 200));
    demoArray.addPointer(new DemoClass("Объект 3", 300));

    cout << "Содержимое массива DemoClass:" << endl;
    for (size_t i = 0; i < demoArray.size(); ++i) {
        cout << i << ": ";
        demoArray[i]->display();
    }

    cout << endl << "=== ТЕСТИРОВАНИЕ ИЗМЕНЕНИЯ ОБЪЕКТОВ ===" << endl;
    // Изменение объекта через оператор []
    if (demoArray.size() > 0) {
        cout << "Изменение первого объекта..." << endl;
        delete demoArray[0]; // Удаляем старый объект
        demoArray[0] = new DemoClass("Измененный объект", 999);
        demoArray[0]->display();
    }

    cout << endl << "=== ТЕСТИРОВАНИЕ КОПИРОВАНИЯ ===" << endl;
    // Тестирование конструктора копирования
    PointerArray<ProgramError> copiedArray = errorArray;
    cout << "Скопированный массив ошибок:" << endl;
    for (size_t i = 0; i < copiedArray.size(); ++i) {
        cout << i << ": ";
        copiedArray[i]->printError();
    }

    cout << endl << "=== ОЧИСТКА РЕСУРСОВ ===" << endl;
    // Деструкторы автоматически очистят память

    cout << "Программа завершена успешно!" << endl;

    return 0;
}