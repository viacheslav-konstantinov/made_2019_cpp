#include <iostream>
#include "pythonString.hpp"

int main()
{
    int x = 44;
    int y = 5;
    double z = 4.5;
    bool myBool = false;
    std::string myStr = "false";

    std::string expr1 = "Expression 1: {3} {1} / {0} + ({1} - {0}) = {2} is {4} ({y})";

    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Example of an error of non integer within braces:\n";
    std::cout << pythonString(expr1, x, y, z, "Expression", myBool, myStr) << std::endl;
    return 0;
}
