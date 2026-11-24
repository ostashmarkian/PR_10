// Lab_10_2_B.cpp
// Лабораторна робота № 10.2. Впорядкування та бінарний пошук в масиві структур
// Варіант 22, рівень В
//   1. Фізичне впорядкування: в першу чергу – за номером курсу, в другу чергу – за назвою
//      спеціальності, в останню чергу – за прізвищем за спаданням (зворотній алфавітний порядок).
//   2. Індексне впорядкування: за номером курсу, далі – за спеціальністю, далі – за прізвищем.
//   3. Бінарний пошук: чи навчається студент із вказаним прізвищем на вказаному курсі
//      на вказаній спеціальності.

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstdlib>
#ifdef _WIN32
#define NOMINMAX                  // щоб макроси min/max з Windows.h не ламали numeric_limits<...>::max()
#include <Windows.h>              // забезпечення відображення кирилиці
#endif
using namespace std;

enum Spec { KN, INF, ME, FI, TN };
string specStr[] = { "КН", "Інформатика", "Мат. та екон.", "Фіз. та інф.", "Труд. навч." };

struct Student
{
    string prizv;     // прізвище
    int    kurs;      // курс (1..6)
    Spec   spec;      // спеціальність
    int    fizyka;    // оцінка з фізики
    int    matem;     // оцінка з математики
    union             // третя оцінка – залежить від спеціальності
    {
        int prog;        // програмування    – для KN
        int chysMetody;  // чисельні методи  – для INF
        int pedagogika;  // педагогіка       – для ME, FI, TN
    };
};

int    ReadInt(const string& prompt, const int min, const int max);
string ReadNonEmptyLine(const string& prompt);
void   Create(Student* p, const int N);
void   PrintRow(const Student& s, const int nomer);
void   Print(const Student* p, const int N);
bool   LessPhysical(const Student& a, const Student& b);
void   Sort(Student* p, const int N);
bool   LessIndex(const Student& a, const Student& b);
int*   IndexSort(const Student* p, const int N);
void   PrintIndexSorted(const Student* p, const int* I, const int N);
int    BinSearch(const Student* p, const int N, const string& prizv, const int kurs, const Spec spec);

#ifndef UNIT_TEST
int main()
{
#ifdef _WIN32                     // забезпечення відображення кирилиці:
    SetConsoleCP(1251);           // встановлення сторінки win-cp1251 в потік вводу
    SetConsoleOutputCP(1251);     // встановлення сторінки win-cp1251 в потік виводу
#endif

    int N = ReadInt("Введіть кількість студентів N: ", 1, 1000);
    Student* p = new Student[N];
    bool entered = false;         // чи введені дані
    bool sorted = false;          // чи масив фізично впорядкований (потрібно для бінарного пошуку)

    int kurs, ispec, found;
    string prizv;
    int* I;

    int menuItem;
    do {
        cout << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування (курс, спеціальність, прізвище за спаданням)" << endl;
        cout << " [4] - індексне впорядкування та вивід (курс, спеціальність, прізвище)" << endl;
        cout << " [5] - бінарний пошук студента за прізвищем, курсом та спеціальністю"
             << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        menuItem = ReadInt("Введіть значення: ", 0, 5);
        cout << endl;

        if (menuItem >= 2 && !entered)
        {
            cout << "Спочатку введіть дані (пункт 1)!" << endl;
            continue;
        }

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            entered = true;
            sorted = false;
            break;
        case 2:
            Print(p, N);
            break;
        case 3:
            Sort(p, N);
            sorted = true;
            Print(p, N);
            break;
        case 4:
            I = IndexSort(p, N);
            PrintIndexSorted(p, I, N);
            delete[] I;
            break;
        case 5:
            if (!sorted)
            {   // бінарний пошук працює лише на впорядкованому масиві
                cout << "Масив ще не впорядкований – виконуємо фізичне впорядкування." << endl;
                Sort(p, N);
                sorted = true;
            }
            prizv = ReadNonEmptyLine("Введіть прізвище: ");
            kurs  = ReadInt("Введіть курс (1..6): ", 1, 6);
            ispec = ReadInt("Введіть спеціальність (0 - КН, 1 - Інформатика, 2 - Мат. та екон.,"
                            " 3 - Фіз. та інф., 4 - Труд. навч.): ", 0, 4);
            found = BinSearch(p, N, prizv, kurs, (Spec)ispec);
            if (found != -1)
                cout << "Так, такий студент навчається: позиція № " << found + 1
                     << " у впорядкованому масиві." << endl;
            else
                cout << "Ні, такий студент на цьому курсі та спеціальності не навчається." << endl;
            break;
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}
#endif

// введення цілого числа з діапазону [min; max] з контролем помилок користувача
int ReadInt(const string& prompt, const int min, const int max)
{
    int x;
    while (true)
    {
        cout << prompt;
        if (cin >> x && x >= min && x <= max)
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return x;
        }
        if (cin.eof())            // кінець потоку вводу – далі вводити нічого
            exit(0);
        cout << "Помилка! Введіть ціле число від " << min << " до " << max << "." << endl;
        cin.clear();                                              // скидаємо стан помилки потоку
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');      // відкидаємо хибний ввід
    }
}

// введення непорожнього літерного рядка
string ReadNonEmptyLine(const string& prompt)
{
    string s;
    do {
        cout << prompt;
        if (!getline(cin, s))     // кінець потоку вводу – далі вводити нічого
            exit(0);
        if (s.empty())
            cout << "Помилка! Рядок не може бути порожнім." << endl;
    } while (s.empty());
    return s;
}

// введення даних про N студентів з клавіатури
void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;
        p[i].prizv  = ReadNonEmptyLine("   прізвище: ");
        p[i].kurs   = ReadInt("   курс (1..6): ", 1, 6);
        p[i].spec   = (Spec)ReadInt("   спеціальність (0 - КН, 1 - Інформатика, 2 - Мат. та екон.,"
                                    " 3 - Фіз. та інф., 4 - Труд. навч.): ", 0, 4);
        p[i].fizyka = ReadInt("   оцінка з фізики (1..5): ", 1, 5);
        p[i].matem  = ReadInt("   оцінка з математики (1..5): ", 1, 5);
        switch (p[i].spec)
        {
        case KN:
            p[i].prog = ReadInt("   оцінка з програмування (1..5): ", 1, 5);
            break;
        case INF:
            p[i].chysMetody = ReadInt("   оцінка з чисельних методів (1..5): ", 1, 5);
            break;
        default:
            p[i].pedagogika = ReadInt("   оцінка з педагогіки (1..5): ", 1, 5);
            break;
        }
        cout << endl;
    }
}

// вивід одного рядка таблиці
void PrintRow(const Student& s, const int nomer)
{
    cout << "| " << setw(3) << right << nomer << " "
         << "| " << setw(13) << left << s.prizv
         << "| " << setw(4) << right << s.kurs << " "
         << "| " << setw(14) << left << specStr[s.spec]
         << "| " << setw(4) << right << s.fizyka << " "
         << "| " << setw(4) << right << s.matem << " ";
    switch (s.spec)
    {
    case KN:
        cout << "| " << setw(6) << right << s.prog << " "
             << "| " << setw(6) << " " << " "
             << "| " << setw(4) << " " << " |" << endl;
        break;
    case INF:
        cout << "| " << setw(6) << " " << " "
             << "| " << setw(6) << right << s.chysMetody << " "
             << "| " << setw(4) << " " << " |" << endl;
        break;
    default:
        cout << "| " << setw(6) << " " << " "
             << "| " << setw(6) << " " << " "
             << "| " << setw(4) << right << s.pedagogika << " |" << endl;
        break;
    }
}

static void PrintHeader(const string& title)
{
    cout << endl << "                    " << title << endl;
    cout << "===================================================================================="
         << endl;
    cout << "|  №  |   Прізвище   | Курс | Спеціальність | Фіз. | Мат. | Прогр. | Ч.мет. | Пед. |"
         << endl;
    cout << "------------------------------------------------------------------------------------"
         << endl;
}

static void PrintFooter()
{
    cout << "===================================================================================="
         << endl << endl;
}

// вивід масиву у вигляді таблиці
void Print(const Student* p, const int N)
{
    PrintHeader("Список студентів");
    for (int i = 0; i < N; i++)
        PrintRow(p[i], i + 1);
    PrintFooter();
}

// порядок фізичного впорядкування: курс ↑, спеціальність ↑, прізвище ↓
// повертає true, якщо a має стояти раніше за b
bool LessPhysical(const Student& a, const Student& b)
{
    if (a.kurs != b.kurs)
        return a.kurs < b.kurs;
    if (a.spec != b.spec)
        return a.spec < b.spec;
    return a.prizv > b.prizv;     // прізвище – за спаданням
}

// фізичне впорядкування масиву методом "бульбашки"
void Sort(Student* p, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (LessPhysical(p[i1 + 1], p[i1]))   // наступний має стояти раніше – міняємо
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
}

// порядок індексного впорядкування: курс ↑, спеціальність ↑, прізвище ↑
bool LessIndex(const Student& a, const Student& b)
{
    if (a.kurs != b.kurs)
        return a.kurs < b.kurs;
    if (a.spec != b.spec)
        return a.spec < b.spec;
    return a.prizv < b.prizv;
}

// формування індексного масиву методом вставки (сам масив p не змінюється)
int* IndexSort(const Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;                 // початково – тотожна перестановка

    int i, j, value;
    for (i = 1; i < N; i++)
    {
        value = I[i];
        for (j = i - 1; j >= 0 && LessIndex(p[value], p[I[j]]); j--)
            I[j + 1] = I[j];
        I[j + 1] = value;
    }
    return I;
}

// вивід масиву в порядку, заданому індексним масивом I
void PrintIndexSorted(const Student* p, const int* I, const int N)
{
    PrintHeader("Індексно впорядкований список");
    for (int i = 0; i < N; i++)
        PrintRow(p[I[i]], i + 1);
    PrintFooter();
}

// бінарний пошук у фізично впорядкованому масиві (порядок – як у Sort)
// повертає індекс знайденого елемента або -1, якщо шуканий елемент відсутній
int BinSearch(const Student* p, const int N, const string& prizv, const int kurs, const Spec spec)
{
    int L = 0, R = N - 1, m;
    while (L <= R)
    {
        m = (L + R) / 2;
        if (p[m].kurs == kurs && p[m].spec == spec && p[m].prizv == prizv)
            return m;
        if ((p[m].kurs < kurs)
            ||
            (p[m].kurs == kurs && p[m].spec < spec)
            ||
            (p[m].kurs == kurs && p[m].spec == spec && p[m].prizv > prizv)) // прізвища ↓
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}
