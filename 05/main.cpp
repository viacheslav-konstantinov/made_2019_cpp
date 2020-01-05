/*
Домашнее задание 5
Простой сериализатор поддерживающий два типа: uint64_t и bool.
Сериализовать в текстовый вид с разделением пробелом, bool сериализуется как true и false
*/

#include <iostream>
#include <ostream>
#include <string>

// класс ошибок
enum class Error
{
    NoError,
    CorruptedArchive
};

//функция для передачи в поток данных
//по умолчанию putOutStream ничего не передает и возвращает 
//Error::CorruptedArchive
template <typename T>
Error putOutStream(T val, std::ostream& os, char sep)
{
    return Error::CorruptedArchive;
}

//перегрузка putOutStream под флаговый тип
//и в этом случае возвращает Error::NoError
Error putOutStream(bool val, std::ostream& os, char sep)
{
    if (val)
        os << "true" << sep;
    else 
        os << "false" << sep;

    return Error::NoError;
}

//перегрузка putOutStream под u_int64_t
//в этом случае также возвращает Error::NoError
Error putOutStream(u_int64_t val, std::ostream& os, char sep)
{
    os << val << sep;

    return Error::NoError;
}

//класс сериалайзера
class Serializer
{
    static constexpr char Separator = ' ';
    public:
        explicit Serializer(std::ostream& out)
            : out_(out) {}

        template <class T>
        Error save(T& object)
        {
            return object.serialize(*this);
        }

        template <class... Args>
        Error operator()(Args&&... args)
        {
            return process_(std::forward<Args>(args)...);
        }
        
    private:
        std::ostream &out_;

        //сериализуем до тех пор, пока не встретится ошибочный тип поля
        //или не закончатся все поля объекта, у которого все поля или 
        //флагового типа, типа u_int64_t
        template<class U, class... Args>
        Error process_(U&& val, Args&&... args)
        {
            Error result = putOutStream(std::forward<U>(val), out_, Separator);
            if(result == Error::NoError)
                return process_(std::forward<Args>(args)...);
            else
                return Error::CorruptedArchive;
        }

        template<class U>
        Error process_(U&& val)
        {
            Error result = putOutStream(std::forward<U>(val), out_, Separator);
            if(result == Error::NoError)
                return Error::NoError;
            else
                return Error::CorruptedArchive;
        }
};


//функция для чтения данных из потока
//по умолчанию getInStream ничего не читает, 
//не сохраняет и возвращает Error::CorruptedArchive
template <typename T>
Error getInStream(T& val, std::istream& inStream)
{
    return Error::CorruptedArchive;
}

//getInStream перегружена под флаговый тип
//и в этом случае записывет в val данные и возвращает Error::NoError
//в случае удачного считывания и Error::CorruptedArchive при ошибке
Error getInStream(bool& val, std::istream& inStream)
{
    std::string buffer;
    inStream >> buffer;
    
    if (buffer == "true")
        val = true;
    else if (buffer == "false")
        val = false;
    else
        return Error::CorruptedArchive;

    return Error::NoError;
}


//getInStream также перегружена под u_int64_t
//и в этом случае записывет в val данные и также возвращает Error::NoError
//в случае удачного считывания и Error::CorruptedArchive при ошибке
Error getInStream(u_int64_t& val, std::istream& inStream)
{
    std::string buffer;
    inStream >> buffer;
    
    try
    {
        val = std::stoll(buffer);
        return Error::NoError;
    }
    catch(std::invalid_argument &e)
    {
        return Error::CorruptedArchive;
    }
}

//класс десериалайзера
class Deserializer
{
    public:
        explicit Deserializer(std::istream& in)
            : in_(in) {}

        template <class T>
        Error load(T& object)
        {
            return object.serialize(*this);
        }

        template <class... Args>
        Error operator()(Args&&... args)
        {
            return process_(std::forward<Args>(args)...);
        }
    
    private:
        std::istream &in_;

        //десериализуем до тех пор, пока не встретится ошибочный тип поля
        //или ошибочный формат данных,
        //или не закончатся все поля объекта, у которого все поля или 
        //флагового типа, типа u_int64_t
        template<class U, class... Args>
        Error process_(U&& val, Args&&... args)
        {
            Error result = getInStream(std::forward<U>(val), in_);
            if(result == Error::NoError)
                return process_(std::forward<Args>(args)...);
            else
                return Error::CorruptedArchive;
        }

        template<class U>
        Error process_(U&& val)
        {
            Error result = getInStream(std::forward<U>(val), in_);
            if(result == Error::NoError)
                return Error::NoError;
            else
                return Error::CorruptedArchive;
        }
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;
    uint64_t d;
    uint64_t e;
    uint64_t f;
    bool g;

    Data(uint64_t aa, bool bb, uint64_t cc, uint64_t dd, uint64_t ee, uint64_t ff, bool gg)
        : a(aa), b(bb), c(cc), d(dd), e(ee), f(ff), g(gg) {}
    Data()
        : a(), b(), c(), d(), e(), f(), g() {}

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, e, f, g);
    }
};

int main()
{
    Data myData1(250, false, 320, 41, 17, 82, true );
    Data myData2(20, 1, 32, 44, 7, 182, 0 );

    Serializer mySerializer(std::cout);

    std::cout << "Serializing object 1 into std::cout: ";
    mySerializer.save(myData1);
    std::cout << "\nSerializing object 2 into std::cout: ";
    mySerializer.save(myData2);
    std::cout << "\n";

    std::cout << "Deserializing object 2 from std::cin...\n";
    std::cout << "Enter new data for object 2 >> ";
    Deserializer myDeserializer(std::cin);

    Error err = myDeserializer.load(myData2);

    if (err == Error::NoError)
    {
        std::cout << "Deserialization was successful!\n";
        std::cout << "Serializing object 2 once again into std::cout: \n";
        mySerializer.save(myData2);
        std::cout << "\n";
    }
    else
    {
        std::cout << "Incorrect input data, deserialization failed!\n";
    }

    return 0;

}
