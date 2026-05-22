#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Book {
private:
    string author;
    string title;
    string publisher;
    int year;
    int pages;

    static int count;   // счётчик живых объектов

public:
    // 1) Конструктор по умолчанию
    Book() : author(""), title(""), publisher(""), year(0), pages(0) {
        count++;
        cout << "++ Book() по умолчанию, всего = " << count << endl;
    }

    // 2) Конструктор с параметрами
    Book(const string& a, const string& t, const string& p, int y, int pg)
        : author(a), title(t), publisher(p), year(y), pages(pg) {
        count++;
        cout << "++ Book(параметры), всего = " << count << endl;
    }

    // 3) Конструктор копирования
    Book(const Book& other)
        : author(other.author), title(other.title), publisher(other.publisher),
        year(other.year), pages(other.pages) {
        count++;
        cout << "++ Book(копия), всего = " << count << endl;
    }

    // Деструктор
    ~Book() {
        count--;
        cout << "-- ~Book(), осталось = " << count << endl;
    }

    // Статический метод для получения счётчика
    static int getCount() { return count; }

    // Метод подсчёта возраста
    int getAge() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        return (now->tm_year + 1900) - year;
    }

    // Метод модификации (для функций modify)
    void modify(int newYear, const string& newPublisher) {
        year = newYear;
        publisher = newPublisher;
    }

    void show() const {
        cout << title << ", " << author << ", " << publisher << ", " << year
            << ", " << pages << " стр., возраст " << getAge() << " лет" << endl;
    }
};

int Book::count = 0;

// ----- Внешние функции (ЛР2, пункт 3) -----
void modify_object(Book& obj, int newYear, const string& newPub) {
    obj.modify(newYear, newPub);
    cout << "  modify_object: ";
    obj.show();
}

void try_to_modify_object(Book obj, int newYear, const string& newPub) {
    obj.modify(newYear, newPub);
    cout << "  try_to_modify_object: ";
    obj.show();
}

// -------------------------------------------------------------
// Две реализации: статическая и динамическая
// Раскомментируйте нужную #define
// -------------------------------------------------------------
#define STATIC_VERSION    // или // #define DYNAMIC_VERSION

#ifdef STATIC_VERSION
// -------------------------------------------------------------
// Реализация 1: классическое создание объектов (на стеке)
// -------------------------------------------------------------
int main() {
    system("chcp 1251 > nul");
    cout << "=== СТАТИЧЕСКАЯ реализация (объекты на стеке) ===\n";
    Book b1;                                // конструктор по умолчанию
    Book b2("Оруэлл", "1984", "Секкер", 1949, 328);
    Book b3 = b2;                           // копирование

    cout << "\nДо модификации b2:\n";
    b2.show();

    modify_object(b2, 2000, "НовоеИзд");
    cout << "После modify_object (b2 изменилась):\n";
    b2.show();

    // Тестируем try_to_modify_object
    cout << "\nПробуем try_to_modify_object на b3:\n";
    try_to_modify_object(b3, 2010, "Пробное");
    cout << "Оригинал b3 (не должен измениться):\n";
    b3.show();

    // Объекты b1,b2,b3 удалятся автоматически при выходе из main
    cout << "\nПеред выходом счётчик = " << Book::getCount() << endl;
    return 0;
}
#endif // STATIC_VERSION

#ifdef DYNAMIC_VERSION
// -------------------------------------------------------------
// Реализация 2: динамическое создание через new / delete
// -------------------------------------------------------------
int main() {
    system("chcp 1251 > nul");
    cout << "=== ДИНАМИЧЕСКАЯ реализация (new / delete) ===\n";
    Book* p1 = new Book;                     // конструктор по умолчанию
    Book* p2 = new Book("Толкин", "Хоббит", "Аллен", 1937, 310);
    Book* p3 = new Book(*p2);                // копия

    cout << "\nИсходный p2:\n";
    p2->show();

    modify_object(*p2, 2020, "ДинамическоеИзд");
    cout << "После modify_object p2:\n";
    p2->show();

    try_to_modify_object(*p3, 2025, "Временное");
    cout << "Оригинал p3 не изменился:\n";
    p3->show();

    // Явное удаление всех динамических объектов
    cout << "\nУдаляем объекты...\n";
    delete p1;
    delete p2;
    delete p3;

    cout << "После удаления счётчик = " << Book::getCount() << endl;
    return 0;
}
#endif // DYNAMIC_VERSION