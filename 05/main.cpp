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
//по умолчанию putOutStream ничего не передает и возвращает -1
template <typename T>
int putOutStream(T val, std::ostream& os, char sep)
{
    return -1;
}

//putOutStream специализирована под флаговый тип
//и в этом случае возвращает 0
template <>
int putOutStream<bool>(bool val, std::ostream& os, char sep)
{
    if (val)
        os << "true" << sep;
    else 
        os << "false" << sep;

    return 0;
}

//putOutStream также специализирована под u_int64_t
//и в этом случае также возвращает 0
template <>
int putOutStream<u_int64_t>(u_int64_t val, std::ostream& os, char sep)
{
    os << val << sep;

    return 0;
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

        template <class... ArgsT>
        Error operator()(ArgsT... args)
        {
            return process_(args...);
        }
        
    private:
        std::ostream &out_;

        //сериализуем до тех пор, пока не встретится ошибочный тип поля
        //или не закончатся все поля объекта, у которого все поля или флагового типа, типа u_int64_t
        template<class U, class... Args>
        Error process_(U&& val, Args&&... args)
        {
            int result = putOutStream(val, out_, Separator);
            if(result == 0)
                return process_(std::forward<Args>(args)...);
            else
                return Error::CorruptedArchive;
        }

        template<class U>
        Error process_(U&& val)
        {
            int result = putOutStream(val, out_, Separator);
            if(result == 0)
                return Error::NoError;
            else
                return Error::CorruptedArchive;
        }
};


//функция для чтения данных из потока
//по умолчанию getInStream ничего не читает, 
//не сохраняет и возвращает -1
template <typename T>
int getInStream(T& val, std::istream& inStream)
{
    return -1;
}

//getInStream специализирована под флаговый тип
//и в этом случае записывет в val данные и возвращает 0
template <>
int getInStream<bool>(bool& val, std::istream& inStream)
{
    std::string buffer;
    inStream >> buffer;
    
    if (buffer == "true")
        val = true;
    else if (buffer == "false")
        val = false;
    else
        return -1;

    return 0;
}


//getInStream также специализирована под u_int64_t
//и в этом случае записывет в val данные и также возвращает 0
template <>
int getInStream<u_int64_t>(u_int64_t& val, std::istream& inStream)
{
    std::string buffer;
    inStream >> buffer;
    
    try
    {
        val = std::stoll(buffer);
        return 0;
    }
    catch(...)
    {
        return -1;
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

        template <class... ArgsT>
        Error operator()(ArgsT&... args)
        {
            return process_(args...);
        }
    
    private:
        std::istream &in_;

        //десериализуем до тех пор, пока не встретится ошибочный тип поля
        //или ошибочный формат данных,
        //или не закончатся все поля объекта, у которого все поля или флагового типа, типа u_int64_t
        template<class U, class... Args>
        Error process_(U& val, Args&&... args)
        {
            int result = getInStream(val, in_);
            if(result == 0)
                return process_(std::forward<Args>(args)...);
            else
                return Error::CorruptedArchive;
        }

        template<class U>
        Error process_(U& val)
        {
            int result = getInStream(val, in_);
            if(result == 0)
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