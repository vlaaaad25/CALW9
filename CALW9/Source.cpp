#include <iostream>

using namespace std;

// Y = 5.1 * x^2 + 7.4 * x ; x = 10.5 ; x += 3; Y - ? -> 12
void FirstTask() {
    const double SEVENDOTFOUR = 7.4, FIVEDOTONE = 5.1, TENANDHALF = 10.5, THREE = 3;
    double results[13], x = 10.5;

    _asm {
        mov ecx, 12
        start_loop:
        finit; Ініціалізація FPU

            fld SEVENDOTFOUR; st(0) = 7.4
            fmul x; st(0) = 7.4 * x

            fld FIVEDOTONE; st(0) = 5.1 = > st(1) = 7.4 * x
            fmul x
            fmul x; st(0) = 5.1 * x * x = > st(1) = 7.4 * x

            fadd st(0), st(1); st(0) = 5.1 * x * x + 7.4 * x

            fstp results[ecx * 8]

            fld x
            fadd THREE; x += 3
            fstp x
            loop start_loop
    }

    int displayIterator = 1;
    for (int i = 12; i > 0; --i, ++displayIterator) {
        cout << "Result " << displayIterator << ": " << results[i] << endl;
    }
}

// Y = 35 / (x^2 + 6.9) ; x - ? => Y < 1.5
void SecondTask() {
    double x = 0, result, garbage;
    const double THIRTYFIVE = 35, SIXDOTNINE = 6.9, ONEANDHALF = 1.5, ONE = 1;
    _asm {
    iteration:
        finit; Ініціалізація FPU

            fld x; st(0) = x
            fmul x; st(0) = x * x
            fadd SIXDOTNINE; st(0) = x * x + 6.9

            fld THIRTYFIVE; st(0) = 35 = > st(1) = x * x + 6.9

            fdiv st(0), st(1); st(0) = 35 / x * x + 6.9
            fstp result; result = 35 / x * x + 6.9

            fld ONEANDHALF; st(0) = 1.5
            fld result
            fucomip st(0), st(1); 35 / x * x + 6.9 < 1.5

            fld x
            fadd ONE; x += 1
            fstp x

            jnc iteration

            fld x
            fsub ONE; x -= 1
            fstp x
    }

    cout << "x: " << x << "; Result: " << result << endl;
}

// Y = 1/2 * arcCosec((A + 10 * B^2)/4)
void ThirdTask() {
    // A = 1, B = 2
    const double ZEROANDHALF = 0.5, A = 1, B = 2, TEN = 10, FOUR = 4, ONEHUNDREEDEIGHTEEN = 180, PI = 3.14159;
    double radians, degrees;

    /*
    * arcCosec(x) = arcSin(1/x) => arcCosec((A + 10 * B^2)/4) = arcCosec(4/(A + 10 * B^2))
    */
    _asm {
        finit; Ініціалізація FPU

        fld TEN; st(0) = 10 = > st(1) = 4
        fmul B
        fmul B; st(0) = 10 * B * B
        fadd A; st(0) = A + 10 * B * B

        fld FOUR; st(0) = 4 = > st(1) = A + 10 * B * B
        fdiv st(0), st(1); st(0) = 4 / A + 10 * B * B

        ; arcSin operation
        fld st
        fld st
        fld1
        fsubr st(2), st
        fadd
        fmul
        fsqrt
        fpatan; arcSin(1 / (A + 10 * B ^ 2) / 4) = arcCosec((A + 10 * B ^ 2) / 4)

        fmul ZEROANDHALF; st(0) = 1 / 2 * arcCosec((A + 10 * B ^ 2) / 4) (in radians)
        fstp radians

        fld radians
        fld ONEHUNDREEDEIGHTEEN; st(0) = 180
        fdiv PI; st(0) = 180 / PI
        fmul radians; st(0) = radians * 180 / PI = > degrees

        fstp degrees
    }

    cout << "Degrees: " << degrees << "; Radians: " << radians << endl;
}

// Y = 21 * ln(x^6 + x^(1/4)); x = 4; x += 2; Y - ? -> 7
void FouthTask() {
    const double TWENTYONE = 21, E = 2.7182, STEP = 2;
    double x = 4, underLog, res1, log10Res, logERes;
    double results[8];

    _asm {
        mov ecx, 7

        start_loop:
        finit; Ініціалізація FPU

            fld x; st(0) = x
            fmul x; st(0) = x ^ 2
            fmul x; st(0) = x ^ 3
            fmul x; st(0) = x ^ 4
            fmul x; st(0) = x ^ 5
            fmul x; st(0) = x ^ 6

            fld x; st(0) = x = > st(1) = x ^ 6
            fsqrt; st(0) = x ^ 1 / 2
            fsqrt; st(0) = x ^ 1 / 4

            fadd st(0), st(1); st(0) = x ^ 6 + x ^ 1 / 4
            fstp underLog

            ; ln(x) = log10(x) / log10(e)

            ; log10(x ^ 6 + x ^ (1 / 4))
            fld1; st(0) = 1
            fld underLog; st(0) = x ^ 6 + x ^ 1 / 4 = > st(1) = 1
            fyl2x; 1 * log2(x ^ 6 + x ^ 1 / 4)
            fstp res1; res1 = log2(x ^ 6 + x ^ 1 / 4)
            FLDL2T; st(0) = log2(10)
            fld res1; st(0) = res1
            fdiv st(0), st(1); res1 / log2(10) = > log10(x ^ 6 + x ^ (1 / 4))
            fstp log10Res

            ; log10(e)
            fld1; st(0) = 1
            fld E; st(0) = E = > st(1) = 1
            fyl2x; 1 * log2(e)
            fstp res1; res1 = log2(e)
            FLDL2T; st(0) = log2(10)
            fld res1; st(0) = res1 = > st(1) = log2(10)
            fdiv st(0), st(1); res1 / log2(10)
            fstp logERes

            ; log10(x) / log10(e)
            fld log10Res
            fdiv logERes; Отримуюємо результат операції ln(x ^ 6 + x ^ (1 / 4))

            fmul TWENTYONE; st(0) = 21 * ln(x ^ 6 + x ^ (1 / 4))

            fstp results[ecx * 8]; Записуємо результат у пам'ять

            fld x
            fadd STEP; x += 2
            fstp x

            loop start_loop
    }

    int displayIterator = 1;
    for (int i = 7; i > 0; --i, ++displayIterator) {
        cout << "Result " << displayIterator << ": " << results[i] << endl;
    }
}

int main()
{
    cout << "First task: " << endl;
    FirstTask();

    cout << endl << "Second task: " << endl;
    SecondTask();

    cout << endl << "Third task: " << endl;
    ThirdTask();

    cout << endl << "Fourth task: " << endl;
    FouthTask();

    return 0;
}