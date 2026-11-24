// UnitTest10_2_B.cpp – unit-тести до Lab_10_2 (Visual Studio: Native Unit Test Project)
#include "pch.h"
#include "CppUnitTest.h"
#define UNIT_TEST                           // щоб не підключати main() з програми
#include "../Lab_10_2_B/Lab_10_2_B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102B
{
    TEST_CLASS(UnitTest102B)
    {
        // тестовий масив: 4 студенти у "перемішаному" порядку
        void Fill(Student* p)
        {
            p[0].prizv = "Bondar";  p[0].kurs = 2; p[0].spec = KN;
            p[1].prizv = "Andriy";  p[1].kurs = 1; p[1].spec = INF;
            p[2].prizv = "Yarema";  p[2].kurs = 2; p[2].spec = KN;
            p[3].prizv = "Koval";   p[3].kurs = 1; p[3].spec = INF;
            for (int i = 0; i < 4; i++) { p[i].fizyka = 4; p[i].matem = 4; p[i].prog = 4; }
        }

    public:
        TEST_METHOD(TestSort)
        {   // курс ↑, спеціальність ↑, прізвище ↓
            Student p[4]; Fill(p);
            Sort(p, 4);
            Assert::IsTrue(p[0].prizv == "Koval");
            Assert::IsTrue(p[1].prizv == "Andriy");
            Assert::IsTrue(p[2].prizv == "Yarema");
            Assert::IsTrue(p[3].prizv == "Bondar");
        }

        TEST_METHOD(TestIndexSort)
        {   // курс ↑, спеціальність ↑, прізвище ↑; сам масив не змінюється
            Student p[4]; Fill(p);
            int* I = IndexSort(p, 4);
            Assert::AreEqual(1, I[0]);   // Andriy
            Assert::AreEqual(3, I[1]);   // Koval
            Assert::AreEqual(0, I[2]);   // Bondar
            Assert::AreEqual(2, I[3]);   // Yarema
            Assert::IsTrue(p[0].prizv == "Bondar");
            delete[] I;
        }

        TEST_METHOD(TestBinSearch)
        {
            Student p[4]; Fill(p);
            Sort(p, 4);
            Assert::AreEqual(2,  BinSearch(p, 4, "Yarema", 2, KN));
            Assert::AreEqual(1,  BinSearch(p, 4, "Andriy", 1, INF));
            Assert::AreEqual(-1, BinSearch(p, 4, "Yarema", 1, KN));    // не той курс
            Assert::AreEqual(-1, BinSearch(p, 4, "Nobody", 2, KN));
        }
    };
}
