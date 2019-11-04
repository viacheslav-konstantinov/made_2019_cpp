#include <iostream>
#include <string>
#include <string.h> 
#include <vector>
#include <algorithm>

//типы колбеков, которые можно зарегистрировать
using simpleCallback =  void (*)(); // колбек в начале и в конце парсинга
using stringCallback =  void (*)(const std::string&); // колбек при нахождении токена любого типа или токена типа строки
using integerCallback = void (*)(int); // колбек при нахождении токена типа числа

//функция для определения типа токена
char tokenType(const std::string& inputExpression)
{
    char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(size_t i = 0; i < inputExpression.length(); ++i)
        if (std::find(std::begin(digits), std::end(digits), inputExpression.at(i)) == std::end(digits)) return 's';
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
        doAtBeginning_(NULL), doAtEnd_(NULL), doWithString_(NULL), doWithAllTokens_(NULL),  doWithInteger_(NULL) {}
    
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
};

void TokenParser::getTokens_()
{
    if(doAtBeginning_)
        doAtBeginning_();
    std::cout << "runParser is called!\n";

    size_t pos;
    std::string currentString;
    std::string delimiters(" \t\n");

    while ((pos = buffer_.find_first_of(delimiters)) != std::string::npos) 
    {
        // токены внутри tokens_ не изменяются при вызове колбеков 
        currentString = buffer_.substr(0, pos);
        std::cout << "Current token: " << currentString << "\n";
        if (currentString.length())
        {
            Token currentToken(currentString);
            tokens_.push_back(currentToken);
            
            if(doWithAllTokens_)
                doWithAllTokens_(currentToken.tokenValue);
            if(currentToken.type == 's' and doWithString_)
                doWithString_(currentToken.tokenValue);
            else if(doWithInteger_)
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

        if(doWithAllTokens_)
            doWithAllTokens_(currentToken.tokenValue);
        if(currentToken.type == 's' and doWithString_)
            doWithString_(currentToken.tokenValue);
        else if(doWithInteger_)
        {
            int currentInteger = std::stoi(currentToken.tokenValue);
            doWithInteger_(currentInteger);
        }
    }

    isBufferParsed_ = true;
    
    if(doAtEnd_)
        doAtEnd_();
}

void TokenParser::recallCallbacks_()
{
    if(doAtBeginning_)
        doAtBeginning_();

    std::cout << "runParser is REcalled.\n";

    for(size_t i = 0; i < tokens_.size(); ++i)
    {
        // токены внутри tokens_ не изменяются при вызове колбеков 
        std::cout << "Current token: " << tokens_.at(i).tokenValue << "\n";
        
        if(doWithAllTokens_)
            doWithAllTokens_(tokens_.at(i).tokenValue);
        if(tokens_.at(i).type == 's' and doWithString_)
            doWithString_(tokens_.at(i).tokenValue);
        else if(doWithInteger_)
            doWithInteger_(std::stoi(tokens_.at(i).tokenValue));
    }
    
    if(doAtEnd_)
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
}

void TokenParser::doAtEnd(simpleCallback func)
{
    doAtEnd_ = func;
}

void TokenParser::doWithString(stringCallback func)
{
    doWithString_ = func;
}

void TokenParser::doWithAllTokens(stringCallback func)
{
    doWithAllTokens_ = func;
}

void TokenParser::doWithInteger(integerCallback func)
{
    doWithInteger_ = func;
}

void callbackAtBeginning()
{
    std::cout << "Callback at beginning is called!\n";
}

void callbackAtEnd()
{
    std::cout << "Callback at end is called!\n";
}

void reverseToken(const std::string& token)
{
    // токены внутри tokens_ не изменяются при вызове колбеков 
    std::string str(token);
    int n = str.length() - 1;
    for(int i=0; i < (str.length()/2); ++i)
    {
        std::swap(str[i], str[n]);
        n -= 1;
    }
    std::cout << "Reversed token: " << str << "\n";
}

void replaceVowels(const std::string& token)
{
    // токены внутри tokens_ не изменяются при вызове колбеков
    std::string str(token);
    size_t pos(0);
    std::string vowels("aeiouyAEIOUY");

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
