
#include <iostream>
#include <iomanip>
#include <string>
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>              // забезпечення відображення кирилиці
#endif
using namespace std;

// спеціальності: КН, Інформатика, Математика та економіка, Фізика та інформатика, Трудове навчання
enum Spec { KN, INF, ME, FI, TN };
string specStr[] = { "КН", "Інформатика", "Мат. та екон.", "Фіз. та інф.", "Труд. навч." };

struct Student
{
    string prizv;        // прізвище
    int    kurs;         // курс (ціле число 1..6)
    Spec   spec;         // спеціальність (перелік)
    int    fizyka;       // оцінка з фізики
    int    matem;        // оцінка з математики
    int    informatyka;  // оцінка з інформатики
};

void   Create(Student* p, const int N);
void   Print(const Student* p, const int N);
double AvgBall(const Student& s);
int    CountNoThrees(const Student* p, const int N);
double PercentAvgLess4(const Student* p, const int N);

#ifndef UNIT_TEST
int main()
{
#ifdef _WIN32                     // забезпечення відображення кирилиці:
    SetConsoleCP(1251);           // встановлення сторінки win-cp1251 в потік вводу
    SetConsoleOutputCP(1251);     // встановлення сторінки win-cp1251 в потік виводу
#endif

    int N;
    cout << "Введіть кількість студентів N: "; cin >> N;
    if (N <= 0)
    {
        cout << "Кількість студентів має бути додатною!" << endl;
        return 1;
    }

    Student* p = new Student[N];
    Create(p, N);
    Print(p, N);

    cout << "Кількість студентів, які вчаться без трійок: "
         << CountNoThrees(p, N) << endl;
    cout << "Процент студентів, у яких середній бал менший 4: "
         << fixed << setprecision(2) << PercentAvgLess4(p, N) << "%" << endl;

    delete[] p;
    return 0;
}
#endif

// середній бал студента з трьох предметів
double AvgBall(const Student& s)
{
    return (s.fizyka + s.matem + s.informatyka) / 3.0;
}

// введення даних про N студентів з клавіатури
void Create(Student* p, const int N)
{
    int spec;
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;
        cin.ignore(10000, '\n');  // прибираємо символ "кінець рядка", що залишився після cin >>
        cout << "   прізвище: "; getline(cin, p[i].prizv);
        cout << "   курс: ";     cin >> p[i].kurs;
        cout << "   спеціальність (0 - КН, 1 - Інформатика, 2 - Математика та економіка,"
             << endl << "                  3 - Фізика та інформатика, 4 - Трудове навчання): ";
        cin >> spec;
        p[i].spec = (Spec)spec;
        cout << "   оцінка з фізики: ";     cin >> p[i].fizyka;
        cout << "   оцінка з математики: "; cin >> p[i].matem;
        cout << "   оцінка з інформатики: "; cin >> p[i].informatyka;
        cout << endl;
    }
}

// вивід масиву студентів у вигляді таблиці (7 клітинок у рядку)
void Print(const Student* p, const int N)
{
    cout << endl << "                              Список студентів" << endl;
    cout << "=================================================================================="
         << endl;
    cout << "|  №  |   Прізвище   | Курс | Спеціальність | Фізика | Математика | Інформатика |"
         << endl;
    cout << "----------------------------------------------------------------------------------"
         << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(3) << right << i + 1 << " "
             << "| " << setw(13) << left << p[i].prizv
             << "| " << setw(4) << right << p[i].kurs << " "
             << "| " << setw(14) << left << specStr[p[i].spec]
             << "| " << setw(6) << right << p[i].fizyka << " "
             << "| " << setw(10) << right << p[i].matem << " "
             << "| " << setw(11) << right << p[i].informatyka << " |" << endl;
    }
    cout << "=================================================================================="
         << endl << endl;
}

// кількість студентів, які вчаться без трійок (всі три оцінки – "4" або "5")
int CountNoThrees(const Student* p, const int N)
{
    int k = 0;
    for (int i = 0; i < N; i++)
        if (p[i].fizyka >= 4 && p[i].matem >= 4 && p[i].informatyka >= 4)
            k++;
    return k;
}

// процент студентів, у яких середній бал менший 4
double PercentAvgLess4(const Student* p, const int N)
{
    if (N <= 0)
        return 0;
    int k = 0;
    for (int i = 0; i < N; i++)
        if (AvgBall(p[i]) < 4)
            k++;
    return 100.0 * k / N;
}
