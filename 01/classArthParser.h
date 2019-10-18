#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstring>
using namespace std;

// Функция, которая  
int myStrToInt(char *inputString, int pos, int end) {
    int result = 0;
    int currentDigit;
    for (int i = pos; i < end; i++) {
        currentDigit = *(inputString + i) - '0';
        result = result + pow(10, end-i-1) * currentDigit;
    }
    return result; 
}

class ArthParser{
    char* stringBegin;
    int len;
    char curLexType;
    int curVal;
    int idx;
    bool isLast;

    public:
        ArthParser(char*, int);
        ~ArthParser();
        int Calculate();
        void SetString(char*, int);
    private:
        int GetSumTerm();
        int GetMultTerm();
        void GetNextLex();

};

ArthParser::ArthParser(char *inputExpression, int expressionLength) {
    stringBegin = inputExpression;
    len = expressionLength;
    curLexType = 'S';
    curVal = 0;
    idx=0;
    isLast = false;
}

ArthParser::~ArthParser() {
    delete stringBegin;
}

void ArthParser::SetString(char *inputExpression, int expressionLength) {
    ArthParser::len = expressionLength;
    ArthParser::stringBegin = inputExpression;
    ArthParser::curLexType = 'S';
    ArthParser::curVal = 0;
    ArthParser::idx=0;
    ArthParser::isLast = false;
}

int ArthParser::Calculate() {
    if (ArthParser::curLexType=='S') {
        ArthParser::GetNextLex();
    }

    int result = ArthParser::GetSumTerm();

    while ((ArthParser::curLexType=='+') or (ArthParser::curLexType=='-')) {
        char lexType = ArthParser::curLexType;
        ArthParser::GetNextLex();

        if (lexType=='+') {
            result += ArthParser::GetSumTerm();
        } else {
            result -= ArthParser::GetSumTerm();
        }
    }

    ArthParser::curLexType = 'S';   // current lexem type; 'S' = start of string, 'E' = end of string, 'N' - number
                                    // can be also '+', '-', '*', '/';
    ArthParser::curVal = 0;
    ArthParser::idx = 0;
    ArthParser::isLast = false;

    return result;
}

int ArthParser::GetSumTerm() {
    int result = ArthParser::GetMultTerm();

    while ((ArthParser::curLexType=='*') or (ArthParser::curLexType=='/'))
    {
        char lexType = ArthParser::curLexType;
        ArthParser::GetNextLex();

        if (lexType=='*') {
            result *= ArthParser::GetMultTerm();
        } else {
            int divider = ArthParser::GetMultTerm();
            
            if (divider!=0)
            {
                result /=  divider;
            } else {
                throw invalid_argument("Division by zero is impossible");
            }
            
        }
    }
    return result;
}

int ArthParser::GetMultTerm() {
    if(ArthParser::curLexType== 'N') {
        ArthParser::GetNextLex();
        return ArthParser::curVal;
    } else {
        ArthParser::GetNextLex();
        return ArthParser::Calculate();
    }
}

void ArthParser::GetNextLex() {
    char nums[] = "0123456789";
    char opers[] = "+-*/";

    if (ArthParser::len==0) {
        throw invalid_argument("Empty string, nothing to calculate");
    }

    if ((find(opers, opers+4, *(ArthParser::stringBegin + ArthParser::len-1)) != opers+4)){
        throw invalid_argument("Invalid syntax: string cannot be ended with operator");
    }
    
    if (ArthParser::idx < ArthParser::len) {

        //checking if char is a digit or an arithmetic operator
        if ((find(nums, nums+10, *(ArthParser::stringBegin + ArthParser::idx)) != nums+10) or \
            (find(opers, opers+4, *(ArthParser::stringBegin + ArthParser::idx)) != opers+4)) {
            
            // checking if all the string is read
            if ((ArthParser::idx==ArthParser::len) or (ArthParser::isLast)) {
                ArthParser::curLexType = 'E';         

            // catching arithmetic operators
            } else if (find(opers, opers+4, *(ArthParser::stringBegin + ArthParser::idx)) != opers+4) {



                if ((*(ArthParser::stringBegin + ArthParser::idx)!='-') and \
                    (ArthParser::idx>0) and \
                    (find(opers, opers+4, *(ArthParser::stringBegin + ArthParser::idx-1)) != opers+4)){
                        throw invalid_argument("Invalid syntax: two operators cannot be one after another");
                }

                char perviousLexType = ArthParser::curLexType;
                ArthParser::curLexType = *(ArthParser::stringBegin + ArthParser::idx);

                // catching binar minus
                if (((find(opers, opers+4, perviousLexType) != opers+4) or (perviousLexType=='S')) and \
                    (ArthParser::curLexType=='-')) {
                    
                    ++ArthParser::idx;
                    int firstIdx = ArthParser::idx;

                    if ((ArthParser::idx < ArthParser::len) and \
                        (find(nums, nums+10, *(ArthParser::stringBegin + ArthParser::idx)) != nums+10)) {
                            
                        while ((find(nums, nums+10, *(ArthParser::stringBegin + ArthParser::idx)) != nums+10) and \
                            (ArthParser::idx < ArthParser::len - 1)) {
                            
                            ++ArthParser::idx;
                        }

                        if (ArthParser::idx < ArthParser::len - 1) {
                            ArthParser::curVal = -myStrToInt(ArthParser::stringBegin, firstIdx, ArthParser::idx);
                            
                        } else {
                            // checking if we have finished parsing the string
                            ArthParser::curVal = -myStrToInt(ArthParser::stringBegin, firstIdx, ArthParser::len);
                            
                            ArthParser::isLast = true; 
                        }                            
                    
                        ArthParser::curLexType = 'N';
                    } else {
                        throw invalid_argument("Binar minus syntax error");
                    }
                } else {
                ++ArthParser::idx;
                }
            // catching number
            } else if (find(nums, nums+10, *(ArthParser::stringBegin + ArthParser::idx)) != nums+10) {
                int firstIdx = ArthParser::idx;
                
                while ((find(nums, nums+10, *(ArthParser::stringBegin + ArthParser::idx)) != nums+10) and \
                    (ArthParser::idx < ArthParser::len - 1)) {
                    
                    ++ArthParser::idx;
                }

                if (ArthParser::idx < ArthParser::len - 1) {
                    ArthParser::curVal = myStrToInt(ArthParser::stringBegin, firstIdx, ArthParser::idx);
                    
                } else {
                    // checking if we have finished parsing the string
                    ArthParser::curVal = myStrToInt(ArthParser::stringBegin, firstIdx, ArthParser::len);
                    
                    ArthParser::isLast = true; 
                }
                            
                ArthParser::curLexType = 'N';
            }
        } else {
            throw invalid_argument("Invalid syntax: unknown symbol in string");
        }
    } else {
        if (ArthParser::curLexType == 'S') {
            throw invalid_argument("String consists only of spaces, nothing to calculate");
        }
        ArthParser::curLexType == 'E';
    }
}
