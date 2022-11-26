/*
 * В качестве примера мы замеряем время подсчета факториала числа 10'000
 * и возведение в степень рациональных чисел. При возведении в степень мы
 * сознательно не используем быстрое возведение в степень.
 */

#include <iostream>
#include <ctime>
#include "rational.h"

void factorial(int n)
{
    BigInteger f = 1;
    for (int i = 2; i <= n; ++i)
        f *= i;
    std::cout << "Factorial of " << n << " was calculated" << std::endl;
}

void pow(int n)
{
    float r0 = 2.5;
    Rational r = r0;
    // no fast power
    for (int i = 1; i < n; ++i)
        r *= r0;
    std::cout << "Rational number " << r0 << " raised to the power of " << n << std::endl;
}


int main() {
    clock_t start = clock();
    factorial(10'000);
    clock_t end = clock();
    std::cout << "Time: " << (end - start) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;

    start = clock();
    pow(10'000);
    end = clock();
    std::cout << "Time: " << (end - start) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;
    return 0;
}