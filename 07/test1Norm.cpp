#include <iostream>
#include "pythonString.hpp"


int main()
{
    int x = 44;
    int y = 5;
    double z = 4.5;
    bool myBool = false;
    std::string myStr = "false";
    std::string expr1, expr2;
    
    expr1 = "Expression 1: {3} {1} / {0} + ({1} - {0}) = {2} is {4} ({5})";
    expr2 = "Expression 2: {0}{1}/{2} = {3} is also {4}";
    
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Example of using of the pythonString function without errors:\n";
    std::cout << pythonString(expr1, x, y, z, "Expression", myBool, myStr) << std::endl;
    std::cout << pythonString(expr2, x, y, 3, 9, myStr) << std::endl;
    std::cout << "-------------------------------------------------------------------\n";
    return 0;
}
