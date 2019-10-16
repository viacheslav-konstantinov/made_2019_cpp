#include <iostream>
#include <cstring>
#include <stdexcept>
#include "classArthParser.h"
using namespace std;


int main() {
    cout << "Enter your arithmetic expression or 'quit'/'exit' to finish:" << endl;
    cout << "Just digits and symbols '+', '-', '*', '/' and binary'-' are allowed" << endl;
    cout << "Neither parentheses, nor spaces are allowed" << endl;
    string inputString;

    while (true)
    {
        getline(cin, inputString);
        if (inputString=="exit" or inputString=="quit") {
            return 0;
        }

        char * myStr = new char [inputString.length()];
        strcpy (myStr, inputString.c_str());
        
        ArthParser myParser (myStr);

        try {
            cout << myParser.Calculate() << endl;
        }
        catch(const invalid_argument e) {
            cerr << e.what() << '\n';
            return -1;
        }
    }
}