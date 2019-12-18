#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

template<typename T>
void getPrintedArgs(std::vector<std::string>& inputArray, T&& value)
{
    std::stringstream ss;;
    ss << std::forward<T>(value);
    inputArray.push_back(ss.str());
}

template<typename T, typename ...TArgs>
void getPrintedArgs( std::vector<std::string>& inputArray, T&& value, TArgs&&... args)
{
    std::stringstream ss;;
    ss << std::forward<T>(value);
    inputArray.push_back(ss.str());
    getPrintedArgs(inputArray, std::forward<TArgs>(args)...);
}


template<typename ...TArgs>
std::string pythonString(std::string str = "", TArgs&&... args)
{
    if(!str.length())
        return str;
    
    //проверка синтаксиса
    //ожидаем открывающую скобку
    bool openBraceExprected = true;
    for(auto ch : str)
    {
        if(openBraceExprected && ch == '{')
            openBraceExprected = false; // ожидаем закрывающуюю скобку
        else if(!openBraceExprected && ch == '}')
            openBraceExprected = true;  // ожидаем открывающую скобку
        else if(openBraceExprected && ch == '}')
            throw std::runtime_error("Invalid using of a closing brace, check syntax");
        else if(!openBraceExprected && ch == '{')
            throw std::runtime_error("Invalid using of an opening brace, check syntax");
    }

    //если после итерации по строке не ожидается открывающая скобка, 
    //значит последняя открывающая фигурная скобка не имеет пары
    if (!openBraceExprected)
        throw std::runtime_error("Invalid using of an opening brace, check syntax");
    std::string result = "";
    
    //строка с заданным индексом i = то, как выводится в стандартный поток вывода
    //аргумент с индексом i из args
    std::vector<std::string> argsPrinted;
    
    try
    {
        //запись в массив argsPrinted того, как args выводятся во внешний поток в виде строк
        getPrintedArgs(argsPrinted, std::forward<TArgs>(args)...);
    }
    catch(const std::ios::failure& e)
    {
        std::string message = "Some of the provided arguments cannot be printed into std::cout.\n";
        message += e.what();
        throw std::runtime_error(message);
    }
    
    size_t tokenStartIdx = 0;
    for(size_t idx = 0; idx < str.length();)
    {
        if(str.at(idx) == '{')
        {
            size_t idxOfClosingBracket = str.find('}', tokenStartIdx);

            size_t argIdx;
                
            try
            {
                if(idx > 0 && idxOfClosingBracket < str.length())
                {
                    std::string previousToken = str.substr(tokenStartIdx, idx - tokenStartIdx);
                    argIdx = std::stoi(str.substr(idx + 1, idxOfClosingBracket - idx - 1));
                    result = result + previousToken + argsPrinted.at(argIdx);
                    tokenStartIdx = idxOfClosingBracket + 1;
                    idx = idxOfClosingBracket + 1;
                }
                else if (idx == 0 && idxOfClosingBracket < str.length())
                {
                    argIdx = std::stoi(str.substr(idx + 1, idxOfClosingBracket - idx - 1));
                    result = result + argsPrinted.at(argIdx);
                    tokenStartIdx = idxOfClosingBracket + 1;
                    idx = idxOfClosingBracket + 1;            
                }
                else if (idx > 0 && idxOfClosingBracket == str.length() - 1)
                {
                    std::string previousToken = str.substr(tokenStartIdx, idx - tokenStartIdx);
                    argIdx = std::stoi(str.substr(idx + 1, idxOfClosingBracket - idx - 1));
                    result = result + previousToken + argsPrinted.at(argIdx);
                    break;               
                }
                else if (idx == 0 && idxOfClosingBracket == str.length() - 1)
                {
                    argIdx = std::stoi(str.substr(idx + 1, idxOfClosingBracket - idx - 1));
                    result = result + argsPrinted.at(argIdx);
                    break;
                }
            }
            // если не хватит памяти для str.substr
            catch(const std::bad_alloc& e)
            {
                std::string message = "Not enough memory for the execution";
                throw std::runtime_error(message);
            }
            //если индекс внутри фигурных скобок превышает значение максмального индекса в argsPrinted
            catch(const std::out_of_range& e)
            {
                std::string message;
                size_t maxIdx;
                if (argsPrinted.size() >0)
                    maxIdx = argsPrinted.size() - 1;
                else 
                    maxIdx = 0;
                message = "Index of an object within braces is incorrect! ";
                message = message + "Position of the opening brace: " + std::to_string(idx) + ". ";
                message = message + "Passed index: " + std::to_string(argIdx) + ". ";
                message = message + "Max possible index: " + std::to_string(maxIdx) + ".\n";
                message = message + "Successfully passed part of the string:\n" + result;
                throw std::runtime_error(message);
            }
            //если выражение внутри скобок не может быть приведено к численному виду
            catch(const std::invalid_argument& e)
            {
                std::string message;
                message = "Expression within braces is not an integer! ";
                message = message + "Position of the opening brace: " + std::to_string(idx) + ".\n";
                message = message + "Successfully passed part of the string:\n" + result;
                throw std::runtime_error(message);
            }                
        }
        else if (str.at(idx) != '}')
        {
            ++idx;
            if(idx != str.length())
                continue;
            else
                result = result + str.substr(tokenStartIdx, idx - tokenStartIdx);
            
        }
        else
        {
            //найдена закрывающая скобка вне контеста = нарушение синтаксиса
            std::string message = "A closing brace is used out of contex at position " + std::to_string(idx) + ".\n";
            message = message + "Successfully passed part of the string:\n" + result;
            throw std::runtime_error(message);
        }
        
    }

    return result;
}