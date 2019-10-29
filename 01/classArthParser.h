#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <stdint.h>

class ArthParser
{
    public:
        ArthParser(const std::string);
        void setString(const std::string);
        std::string getString();
        size_t size();
        int calculate();

    private:
        char curLexType_;
        int curVal_;
        int idx_;
        bool isLast_;
        std::string expressionToCalculate_;

        bool isOperator_(size_t);
        bool isDigit_(size_t);
        int getSumTerm_();
        int getMultTerm_();
        void getNextLex_();
};

ArthParser::ArthParser(const std::string inputExpression) 
{
    expressionToCalculate_ = inputExpression;
    curLexType_ = 'S';
    curVal_ = 0;
    idx_ = 0;
    isLast_ = false;
}

void ArthParser::setString(std::string inputExpression) 
{
    expressionToCalculate_ = inputExpression;
    curLexType_ = 'S';
    curVal_ = 0;
    idx_ = 0;
    isLast_ = false;
}

std::string ArthParser::getString()
{
    return expressionToCalculate_;
}

size_t ArthParser::size()
{
    return expressionToCalculate_.length();
}

bool ArthParser::isOperator_(size_t idx)
{
    if (idx < size())
    {
        std::string operators = "+-*/";
        std::string::size_type n = operators.find(expressionToCalculate_.at(idx));
        return !(n == std::string::npos);
    }
    return false;
}

bool ArthParser::isDigit_(size_t idx)
{
    if (idx < size())
    {
        std::string digits = "0123456789";
        std::string::size_type n = digits.find(expressionToCalculate_.at(idx));
        return !(n == std::string::npos);
    }
    else
        return false;
}

int ArthParser::calculate() 
{
    if (curLexType_ == 'S')
        getNextLex_();

    int result = getSumTerm_();
    char lexType = curLexType_;

    while ((curLexType_ == '+') or (curLexType_ == '-')) 
    {
        lexType = curLexType_;
        getNextLex_();

        if (lexType == '+')
            result += getSumTerm_();
        else
            result -= getSumTerm_();
    }

    curLexType_ = 'S';   // current lexem type; 'S' = start of string, 'E' = end of string, 'N' - number
                         // can be also '+', '-', '*', '/';
    curVal_ = 0;
    idx_ = 0;
    isLast_ = false;

    return result;
}

int ArthParser::getSumTerm_() 
{
    int result = getMultTerm_();
    char lexType;

    while ((curLexType_ == '*') or (curLexType_ == '/'))
    {
        lexType = curLexType_;
        getNextLex_();

        if (lexType == '*')
            result *= getMultTerm_();
        else 
        {
            int divider = getMultTerm_();
            
            if (divider!=0)
                result /=  divider;
            else
                throw std::invalid_argument("Division by zero is impossible");
        }
    }

    return result;
}

int ArthParser::getMultTerm_() 
{
    if(curLexType_== 'N') 
    {
        getNextLex_();
        return curVal_;
    } 
    else 
    {
        getNextLex_();
        return calculate();
    }
}

void ArthParser::getNextLex_() 
{

    if (size() == 0)
        throw std::invalid_argument("Empty string, nothing to calculate");

    if (isOperator_(size() - 1))
        throw std::invalid_argument("Invalid syntax: string cannot be ended with operator");
    
    if (idx_ < size()) 
    {
        //checking if char is a digit or an arithmetic operator
        if (isDigit_(idx_) or isOperator_(idx_))
        {    
            // checking if all the string is read
            if (idx_ == size() or isLast_) 
                curLexType_ = 'E';         

            // catching arithmetic operators
            else if (isOperator_(idx_)) 
            {
                if ((expressionToCalculate_.at(idx_) != '-') and (idx_ > 0) and isOperator_(idx_ - 1))
                        throw std::invalid_argument("Invalid syntax: two operators cannot be one after another");

                char perviousLexType = curLexType_;
                curLexType_ = expressionToCalculate_.at(idx_);

                // catching binar minus
                if ((isOperator_(idx_ - 1) or (perviousLexType == 'S')) and (curLexType_=='-')) 
                {    
                    ++idx_;
                    int firstidx_ = idx_;

                    if ((idx_ < size()) and (isDigit_(idx_))) 
                    {
                        while ((idx_ < size() - 1) and (isDigit_(idx_)))                         
                            ++idx_;

                        if (idx_ < size() - 1)
                            curVal_ = -std::stoi(expressionToCalculate_.substr(firstidx_, idx_ - firstidx_ + 1));
                        else 
                        {
                            // checking if we have finished parsing the string
                            curVal_ = -std::stoi(expressionToCalculate_.substr(firstidx_, size() - firstidx_ + 1));
                            isLast_ = true; 
                        }                            
                        curLexType_ = 'N';
                    } 
                    else
                        throw std::invalid_argument("Binar minus syntax error");
                } 
                else
                ++idx_;
            // catching number
            } 
            else if (isDigit_(idx_)) 
            {
                int firstidx_ = idx_;
                
                while ((isDigit_(idx_)) and (idx_ < size() - 1))
                    ++idx_;

                if (idx_ < size() - 1)
                    curVal_ = std::stoi(expressionToCalculate_.substr(firstidx_, idx_ - firstidx_ + 1));    
                else 
                {
                    // checking if we have finished parsing the string
                    curVal_ = std::stoi(expressionToCalculate_.substr(firstidx_, size() - firstidx_ + 1));
                    isLast_ = true; 
                }
                curLexType_ = 'N';
            }
        } 
        else
            throw std::invalid_argument("Invalid syntax: unknown symbol in string");
    } 
    else 
    {
        if (curLexType_ == 'S')
            throw std::invalid_argument("String consists only of spaces, nothing to calculate");
        curLexType_ == 'E';
    }
}