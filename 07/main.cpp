/*
Практическая часть 7

Реализована функция pythonString для форматирования строки, которая поддерживает любой тип, 
который может быть выведен в поток вывода. Формат строки форматирования:

"{0} any text {1} {0}"
Номер в фигурных скобках - номер аргумента. Если аргументов меньше, чем число в скобках, и в 
случае прочих ошибок выбрасывается исключение std::runtime_error

Пример:

auto text = format("{1}+{1} = {0}", 2, "one");
assert(text == "one+one = 2");

Фигурные скобки - зарезервированный символ, если встречаются вне контекста {n} выбрасывается 
исключение std::runtime_error
*/

#include <iostream>
#include "pythonString.hpp"


int main()
{
    std::cout << "This is an example of using of the pythonString function\n";

    int x = 2;
    int y = 7;
    double z = 8.5;
    bool myBool = true;
    std::string myStr = "true";

    std::string expr1 = "Expression 1: {3} {1} / {0} + ({1} - {0}) = {2} is {4} ({5})";
    std::string expr2 = "Expression 2: {0}{1 }/{2} = {3} is also {4}";
    std::cout << pythonString(expr1, x, y, z, "Expression", myBool, myStr) << std::endl;
    std::cout << pythonString(expr2, x, y, 3, 9, myStr) << std::endl;
    return 0;
}
