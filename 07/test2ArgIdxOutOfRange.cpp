#include <iostream>
#include "pythonString.hpp"


int main()
{
    int x = 44;
    int y = 5;
    double z = 4.5;
    bool myBool = false;
    std::string myStr = "false";

    std::string expr1 = "Expression 1: {0}{1}/{2} = {55} is also {4}";

    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Example of an error of an argument index out of range:\n";
    std::cout << pythonString(expr1, x, y, 3, 9, myStr) << std::endl;
    return 0;
}
