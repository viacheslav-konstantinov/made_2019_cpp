#include <iostream>
#include <string>
#include <string.h> 
#include <vector>
#include <algorithm>
#include "tokenParserClass.hpp"

void callbackAtBeginning()
{
    std::cout << "Callback at beginning is called!\n";
}

void callbackAtEnd()
{
    std::cout << "Callback at end is called!\n";
}

void reverseToken(std::string& str)
{
    int n = str.length() - 1;
    for(int i=0; i < (str.length()/2); ++i)
    {
        std::swap(str[i], str[n]);
        n -= 1;
    }
    std::cout << "Reversed token: " << str << "\n";
}

void replaceVowels(std::string& str)
{
    size_t pos(0);
    std::string vowels("aeiouy");

    pos = str.find_first_of(vowels);

    while (pos != std::string::npos) 
    {
        str[pos] = '*';
        pos = str.find_first_of(vowels, pos + 1);
    }   
    std::cout << "Token with vowels replaced with '*': " << str << "\n";
}

void doubleInteger(int integerToDouble)
{
    std::cout << "Doubled integer: " << 2*integerToDouble << "\n";
}

int main()
{
    std::string myInput;

    std::getline (std::cin, myInput);
    
    TokenParser myParser(myInput);

    myParser.doAtBeginning(callbackAtBeginning);
    myParser.doAtEnd(callbackAtEnd);
    myParser.doWithAllTokens(reverseToken);
    myParser.doWithString(replaceVowels);
    myParser.doWithInteger(doubleInteger);

    myParser.runParser();
    myParser.printTokens();
    myParser.runParser();

    return 0;
}