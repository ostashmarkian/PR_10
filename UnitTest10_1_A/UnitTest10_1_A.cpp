// UnitTest10_1_A.cpp – unit-тести до Lab_10_1_A (Visual Studio: Native Unit Test Project)
#include "pch.h"
#include "CppUnitTest.h"
#define UNIT_TEST                           // щоб не підключати main() з програми
#include "../Lab_10_1_A/Lab_10_1_A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest101
{
    TEST_CLASS(UnitTest101)
    {
    public:
        TEST_METHOD(TestCountNoThrees)
        {
            Student p[3];
            p[0].prizv = "A"; p[0].kurs = 1; p[0].spec = KN;  p[0].fizyka = 5; p[0].matem = 4; p[0].informatyka = 5;
            p[1].prizv = "B"; p[1].kurs = 2; p[1].spec = INF; p[1].fizyka = 3; p[1].matem = 5; p[1].informatyka = 5;
            p[2].prizv = "C"; p[2].kurs = 3; p[2].spec = TN;  p[2].fizyka = 4; p[2].matem = 4; p[2].informatyka = 4;

            Assert::AreEqual(2, CountNoThrees(p, 3));
        }

        TEST_METHOD(TestPercentAvgLess4)
        {
            Student p[4];
            p[0].spec = KN;  p[0].fizyka = 5; p[0].matem = 5; p[0].informatyka = 5;        // 5.00
            p[1].spec = INF; p[1].fizyka = 3; p[1].matem = 4; p[1].informatyka = 4;  // 3.67 < 4
            p[2].spec = ME;  p[2].fizyka = 4; p[2].matem = 4; p[2].informatyka = 4;  // 4.00 – не менше
            p[3].spec = FI;  p[3].fizyka = 3; p[3].matem = 3; p[3].informatyka = 3;  // 3.00 < 4

            Assert::AreEqual(50.0, PercentAvgLess4(p, 4), 1e-9);
        }
    };
}
