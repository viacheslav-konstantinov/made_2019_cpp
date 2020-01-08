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
std::string pythonString(std::string& str = "", TArgs&&... args)
{
    if(str.empty())
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
    
    //строка с заданным индексом i = то, как выводится в стандартный поток вывода
    //аргумент с индексом i из args
    std::vector<std::string> argsPrinted;
    
    try
    {
        //запись в массив argsPrinted того, как args выводятся во внешний поток в виде строк
        getPrintedArgs(argsPrinted, std::forward<TArgs>(args)...);
    }
    // ловим это исключение, если операция ss << std::forward<T>(value);
    // завершится неудачей, то есть value не может быть выведен в стандартный выходной поток
    catch(const std::ios::failure& e)
    {
        std::string message = "Some of the provided arguments cannot be printed into output stream.\n";
        message += e.what();
        throw std::runtime_error(message);
    }


    std::string result = "";
    for(size_t idx = 0; idx < str.length(); )
    {
        if(str.at(idx) == '{')
        {
            size_t  argIdx;
            size_t  pos = idx;     
            try
            {

                argIdx = std::stoi(str.substr(idx + 1), &pos);
                result = result + argsPrinted.at(argIdx);
                idx = idx + 1 + pos;
                
                if( str.at(idx) != '}')
                {
                    //после парсинга числа следующий за ним символ != }' => нарушение синтаксиса
                    std::string message = "Expected a closing brace at position " + std::to_string(idx) + ", ";
                    message = message + "but " + str.at(idx) + " instead.\n";
                    message = message + "Successfully passed part of the string:\n" + result;
                    throw std::runtime_error(message);
                }
                else
                {
                    ++idx;
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
                if (argsPrinted.size() >0 )
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
        else if (str.at(idx)  != '}')
        {
            result += str.at(idx) ;
            ++idx;
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
