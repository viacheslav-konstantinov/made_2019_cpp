#include <iostream>
#include <cstring>
#include <stdexcept>
#include "classArthParser.h"
using namespace std;


int main(int argc, char *argv[]) {

    if(argc == 1) {
        printf("ERROR: Expected at least 1 argument\n");
        return -1;
    }
    
    int size = 0;
    for (int i = 1; i<=argc-1; i++) {
        size = size + strlen(argv[i]);
    }

    char *inputExpression = new char[size];
    
    for(int i = 1; i <= argc-1; i++) {
        strcat(inputExpression, argv[i]);
    }

    try {
        
        ArthParser myParser (inputExpression, size);
        cout << myParser.Calculate() << endl;
        delete inputExpression;
        return 0;
    }
    catch(const invalid_argument& e) {
        delete inputExpression;
        cerr << e.what() << '\n';
        return -1;
    }
}
