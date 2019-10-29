#include <iostream>
#include <cstring>
#include <stdexcept>
#include <stdint.h>
#include "classArthParser.h"
using namespace std;

int main(int argc, char *argv[]) 
{
    if(argc == 1) 
    {
        cout << "ERROR: Expected at least 1 argument" << endl; ;
        return -1;
    }

    try 
    {
        ArthParser myParser ((string) (argv[1]));
        cout << myParser.calculate();
        return 0;
    }
    catch(const invalid_argument& e) 
    {
        cerr << e.what() << endl;
        return -1;
    }
}