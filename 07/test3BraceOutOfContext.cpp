#include <iostream>
#include "pythonString.hpp"

int main()
{
    int x = 44;
    int y = 5;
    std::string myStr = "false";

    std::string expr1 = "Expression 1: {0}{1}/{2} = {3} { is also {4}";

    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Example of an error of using a brace out of context:\n";
    std::cout << pythonString(expr1, x, y, 3, 9, myStr) << std::endl;
    return 0;
}
