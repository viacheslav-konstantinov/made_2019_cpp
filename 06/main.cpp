/*
Практическое задание 6

Написать класс для работы с большими целыми числами. Размер числа ограничен только размером памяти.
Нужно поддержать семантику работы с обычным int.

Реализовать оператор вывода в поток, сложение, вычитание, унарный минус, все операции сравнения.

std::vector и другие контейнеры использовать нельзя - необходимо самостоятельно управлять памятью.

*/

#include "bigInteger.hpp"

int main()
{
    BigInteger int1, int2;
    std::cout << "Enter two integers >> \n";

    std::cin >> int1 >> int2;

    std::cout << "Printing the entered integers: \n";
    std::cout << "int1 = " << int1 << std::endl;
    std::cout << "int2 = " << int2 << std::endl;

    std::cout << "Comparing the integers:\n";
    std::cout << "int1 <  int2 ? " << (int1 <  int2) << std::endl;
    std::cout << "int1 <= int2 ? " << (int1 <= int2) << std::endl;
    std::cout << "int1 >  int2 ? " << (int1 >  int2) << std::endl;
    std::cout << "int1 >= int2 ? " << (int1 >= int2) << std::endl;
    std::cout << "int1 == int2 ? " << (int1 == int2) << std::endl;
    std::cout << "int1 != int2 ? " << (int1 != int2) << std::endl;

    std::cout << "Unary operations:\n";
    std::cout << "-int1 = " << -int1 << std::endl;
    std::cout << "-int2 = " << -int2 << std::endl;

    std::cout << "--int1 = " << --int1 << std::endl;
    std::cout << "--int2 = " << --int2 << std::endl;
    std::cout << "++int1 = " << ++int1 << std::endl;
    std::cout << "++int2 = " << ++int2 << std::endl;

    std::cout << "Arithmetic operations:\n";
    std::cout << "int1 + int2 = " << (int1 + int2) << std::endl;
    std::cout << "int1 - int2 = " << (int1 - int2) << std::endl;
    std::cout << "int2 - int1 = " << (int2 - int1) << std::endl;
    std::cout << "int1 * int2 = " << (int1 * int2) << std::endl;
    std::cout << "int1 / int2 = " << (int1 / int2) << std::endl;
    std::cout << "int2 / int1 = " << (int2 / int1) << std::endl;

    std::cout << "Digits-wise printing of int1: \n";
    int1.print();
    std::cout << "Digits-wise printing of int1*int2: \n";
    (int1 * int2).print();

    return 0;
}
