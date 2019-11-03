#include <iostream>
#include <string>
#include <string.h> 
#include <vector>
#include <algorithm>

//типы колбеков, которые можно зарегистрировать
using simpleCallback = void (*)(); // колбек в начале и в конце парсинга
using stringCallback = void (*)(std::string&); // колбек при нахождении токена любого типа или токена типа строки
using integerCallback = void (*)(int); // колбек при нахождении токена типа числа

//функция для определения типа токена
char tokenType(const std::string& inputExpression)
{
    char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(size_t i = 0; i < inputExpression.length(); ++i)
        if (std::find(digits, digits + 10, inputExpression.at(i)) == digits + 10) return 's';
    return 'n';
}

//токен
struct Token
{
    std::string tokenValue;
    char type;
    Token(const std::string& inputExpression): tokenValue(inputExpression), type(tokenType(inputExpression)) {}
};

class TokenParser
{
public:
    TokenParser(const std::string& inputExpression): buffer_(inputExpression), isBufferParsed_(false),
        doSmthAtBeginning_(false), doSmthAtEnd_(false), doSmthWithString_(false),
            doSmthWithAllTokens_(false), doSmthWithInteger_(false) {}
    
    void runParser();
    void printTokens();

    //методы для регистрации колбеков
    void doAtBeginning(simpleCallback func);
    void doAtEnd(simpleCallback func);
    void doWithString(stringCallback func);
    void doWithAllTokens(stringCallback func);
    void doWithInteger(integerCallback func);
    
private:
    // метод, который вызывается в runParser();, если строка не была отпарсена
    void getTokens_();
    // метод, который вызывается в runParser();, если строка уже была отпарсена
    void recallCallbacks_();

    std::string buffer_;
    // массив токенов
    std::vector<Token> tokens_;

    // зарегистрированные колбеки
    simpleCallback doAtBeginning_;
    simpleCallback doAtEnd_;
    stringCallback doWithString_;
    stringCallback doWithAllTokens_;
    integerCallback doWithInteger_;

    // флаг, указывающий, была ли уже отпарсена строка
    bool isBufferParsed_;

    // флаги регистрации колбеков
    bool doSmthAtBeginning_;
    bool doSmthAtEnd_;
    bool doSmthWithString_;
    bool doSmthWithAllTokens_;
    bool doSmthWithInteger_;
};

void TokenParser::getTokens_()
{
    if(doSmthAtBeginning_)
        doAtBeginning_();
    std::cout << "runParser is called!\n";

    size_t pos;
    std::string currentString;
    std::string delimiters(" \t\n");

    while ((pos = buffer_.find_first_of(delimiters)) != std::string::npos) 
    {
        // токены в tokens_ не изменияются при вызове колбеков
        // колбеки применяются к копии токена из tokens_
        
        currentString = buffer_.substr(0, pos);
        std::cout << "Current token: " << currentString << "\n";
        if (currentString.length())
        {
            Token currentToken(currentString);
            tokens_.push_back(currentToken);
            
            if(doSmthWithAllTokens_)
                doWithAllTokens_(currentToken.tokenValue);
            if(currentToken.type == 's' and doSmthWithString_)
                doWithString_(currentToken.tokenValue);
            else if(doSmthWithInteger_)
            {
                int currentInteger = std::stoi(currentToken.tokenValue);
                doWithInteger_(currentInteger);
            }
        }
        buffer_.erase(0, pos + 1);
    }   

    if(buffer_.length())
    {
        Token currentToken(buffer_);
        tokens_.push_back(currentToken);

        if(doSmthWithAllTokens_)
            doWithAllTokens_(currentToken.tokenValue);
        if(currentToken.type == 's' and doSmthWithString_)
            doWithString_(currentToken.tokenValue);
        else if(doSmthWithInteger_)
        {
            int currentInteger = std::stoi(currentToken.tokenValue);
            doWithInteger_(currentInteger);
        }
    }

    std::cout << "\n";

    isBufferParsed_ = true;
    
    if(doSmthAtEnd_)
        doAtEnd_();
}

void TokenParser::recallCallbacks_()
{
    if(doSmthAtBeginning_)
        doAtBeginning_();

    std::cout << "runParser is recalled!\n";

    for(size_t i = 0; i < tokens_.size(); ++i)
    {
        // токены в tokens_ не изменияются при вызове колбеков
        // колбеки применяются к копии токена из tokens_
        Token currentToken(tokens_.at(i).tokenValue);
        
        if(doSmthWithAllTokens_)
            doWithAllTokens_(currentToken.tokenValue);
        if(currentToken.type == 's' and doSmthWithString_)
            doWithString_(currentToken.tokenValue);
        else if(doSmthWithInteger_)
        {
            int currentInteger = std::stoi(currentToken.tokenValue);
            doWithInteger_(currentInteger);
        }
    }
    
    if(doSmthAtEnd_)
        doAtEnd_();
}

void TokenParser::runParser()
{
    if (!isBufferParsed_)
        getTokens_();
    else
        recallCallbacks_();
}

void TokenParser::printTokens()
{
    for(size_t i = 0; i < tokens_.size(); ++i)
    {
        std::cout << "i = " << i;
        std::cout << ", value of token: " << tokens_.at(i).tokenValue;
        std::cout << ", type of token: " << tokens_.at(i).type << "\n";
    }
}

void TokenParser::doAtBeginning(simpleCallback func)
{
    doAtBeginning_ = func;
    doSmthAtBeginning_ = true;
}

void TokenParser::doAtEnd(simpleCallback func)
{
    doAtEnd_ = func;
    doSmthAtEnd_ = true;
}

void TokenParser::doWithString(stringCallback func)
{
    doWithString_ = func;
    doSmthWithString_ = true;
}

void TokenParser::doWithAllTokens(stringCallback func)
{
    doWithAllTokens_ = func;
    doSmthWithAllTokens_ = true;
}

void TokenParser::doWithInteger(integerCallback func)
{
    doWithInteger_ = func;
    doSmthWithInteger_ = true;
}