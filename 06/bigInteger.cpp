#include "bigInteger.hpp"

// по умолчанию в BigInteger записывается 0
BigInteger::BigInteger()
    : digitLength_(0)
    , isNegative_(false)
    , digits_(nullptr) 
{
    unsigned char * ptr = new unsigned char[1];
    *ptr = 0;
    digits_ = ptr;
    digitLength_ = 1;
}
        
BigInteger::~BigInteger()
{
    delete[] digits_;
}

BigInteger::BigInteger(BigInteger&& other)
{
    // перемещение в текущий объект
    digits_ = other.digits_;
    digitLength_ = other.digitLength_;
    isNegative_ = other.isNegative_;

    // зануление other
    other.digits_ = nullptr;
    other.isNegative_ = false;
    other.digitLength_ = 0;
}


BigInteger::BigInteger(const BigInteger& other)
{
    // начальная инициализация
    isNegative_ = false;
    digitLength_ = 0;
    digits_ = nullptr;

    //поразрядное копирование
    size_t necessaryDigits(other.digitLength_);
    unsigned char * ptr = new unsigned char[necessaryDigits];
    for(size_t i = 0; i < necessaryDigits; ++i)
        *(ptr + i) = *(other.digits_ + i);

    // сохранение скопированных разрядов
    digits_ = ptr;
    digitLength_ = necessaryDigits;
    isNegative_ = other.isNegative_;
}

BigInteger::BigInteger(const int other)
{
    isNegative_ = false;
    digitLength_ = 0;
    digits_ = nullptr;

    int buffer(other);
    size_t necessaryDigits;
    if (buffer != 0)
        necessaryDigits = 1 + std::floor(std::log2(std::abs(buffer)) / std::log2(base));
    else
        necessaryDigits = 1;
    unsigned char * ptr = new unsigned char[necessaryDigits];
    
    if (buffer < 0) 
    {
        isNegative_ = true;
        buffer = -buffer;
    }

    if(buffer != 0)
    {
        size_t counter(0);
        while (buffer > 0)
        {
            *(ptr + counter) = static_cast<unsigned char>(buffer % base);
            buffer = buffer / base;
            ++counter;
        }
    }
    else
        *ptr = 0;

    digits_ = ptr;
    digitLength_ = necessaryDigits;
}


BigInteger& BigInteger::operator=(BigInteger&& other)
{
    if(this == &other)
        return *this;

    // удаление разрядов текущего объекта
    delete[] digits_;

    // передача управления разрядами из other
    // и сохранение информации оттуда
    digits_      = other.digits_;
    digitLength_ = other.digitLength_;
    isNegative_  = other.isNegative_;

    // "обнуление" other
    other.digits_ = nullptr;
    other.isNegative_ = false;
    other.digitLength_ = 0;

    return *this;
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
    if(this == &other)
        return *this;

    size_t necessaryDigits(other.digitLength_);
    unsigned char * ptr = new unsigned char[necessaryDigits];
    for(size_t i = 0; i < necessaryDigits; ++i)
        *(ptr + i) = *(other.digits_ + i);

    delete[] digits_;

    digits_ = ptr;
    digitLength_ = necessaryDigits;
    isNegative_ = other.isNegative_;

    return *this;
}

BigInteger& BigInteger::operator=(const int other)
{
    int buffer(other);
    size_t necessaryDigits;
    if (buffer != 0)
        necessaryDigits = 1 + std::floor(std::log2(std::abs(buffer)) / std::log2(base));
    else
        necessaryDigits = 1;
    unsigned char * ptr = new unsigned char[necessaryDigits];
    
    if (buffer < 0) 
    {
        isNegative_ = true;
        buffer = -buffer;
    }

    if(buffer != 0)
    {
        size_t counter(0);
        while (buffer > 0)
        {
            *(ptr + counter) = static_cast<unsigned char>(buffer % base);
            buffer = buffer / base;
            ++counter;
        }
    }
    else
        *ptr = 0;

    digits_ = ptr;
    digitLength_ = necessaryDigits;

    return *this;
}



bool BigInteger::operator<(const BigInteger& other) const
{
    // если операнды имеют разные знаки, то проверять разряды не требуется
    if(isNegative_ && !other.isNegative_)
        return true;
    else if(!isNegative_ && other.isNegative_)
        return false;
    else if (digitLength_ != other.digitLength_)
    //если знаки одинаковы, но количество разрядов различно, то достаточно сравнить только длины
        return (!isNegative_ == (digitLength_ < other.digitLength_));
    else
    // только если количество битов из знаков одинаково, выполняем поразрядную проверку
        {
            size_t maxNonEqualDigit = digitLength_ - 1;
            while (maxNonEqualDigit > 0 && *(digits_ + maxNonEqualDigit) == *(other.digits_ + maxNonEqualDigit))
                --maxNonEqualDigit;

            if (*(digits_ + maxNonEqualDigit) == *(other.digits_ + maxNonEqualDigit))
                return false;
            return (!isNegative_ == *(digits_ + maxNonEqualDigit) < *(other.digits_ + maxNonEqualDigit));
        }
}

bool BigInteger::operator==(const BigInteger& other) const
{
    return (!(*this < other) && !(other < *this));
}


bool BigInteger::operator!=(const BigInteger& other) const
{
    return !(*this == other);
}


bool BigInteger::operator>(const BigInteger& other) const
{
    return (!(*this < other) && !(*this == other));
}


bool BigInteger::operator>=(const BigInteger& other) const
{
    return (*this > other || *this == other);
}


bool BigInteger::operator<=(const BigInteger& other) const
{
    return (*this < other || *this == other);
}



BigInteger BigInteger::operator+(const BigInteger& other) const
{
    BigInteger resultBigInteger;

    // если операнды имеют одинаковые знаки, то достаточно выполнить поразрадное сложение
    // и приписать результату знак this
    if(isNegative_ == other.isNegative_)
    {
        std::vector<unsigned char> result(std::max(digitLength_, other.digitLength_), 0);
        unsigned char carry = 0; // перенос в старшие разряды
        for (size_t i = 0; i < result.size() || carry; ++i) 
        {
            if (i == result.size())
                result.push_back(0);
            
            result[i] =  (i < digitLength_ ? *(digits_ + i) : 0) + carry;
            result[i] += (i < other.digitLength_ ? *(other.digits_ + i) : 0);

            carry = static_cast<unsigned char>((i < digitLength_ ? *(digits_ + i) : 0) >= 
                                base - (i < other.digitLength_ ? *(other.digits_ + i) : 0) - carry);
            if (carry)
                result[i] -= base;
        }

        unsigned char * ptr = new unsigned char[result.size()];
        for(size_t i  = 0; i < result.size(); ++i)
            *(ptr + i) = result[i];

        delete[] resultBigInteger.digits_;
        resultBigInteger.digits_ = ptr;
        resultBigInteger.digitLength_ = result.size();
        resultBigInteger.isNegative_ = this->isNegative_;

        return resultBigInteger;
    }
    // если операнды имеют разные знаки, то надо вычесть из бОльшего по модулю меньший
    // и приписать знак бОльшего по модулю операнда
    else
    {
        // проверка, какой операнд больше по модулю
        // он записывается в oper1
        // меньший по модулю - записывается в oper2
        BigInteger oper1, oper2;
        if(this->isNegative_)
        {
            if(this->abs() > other)
            {
                oper1 = *this;
                oper2 = other;
            }
            else
            {
                oper2 = *this;
                oper1 = other;
            }

        }
        else
        {
            if(*this > other.abs())
            {
                oper1 = *this;
                oper2 = other;
            }
            else
            {
                oper2 = *this;
                oper1 = other;
            }
        }
        std::vector<unsigned char> result(std::max(digitLength_, other.digitLength_), 0);
        unsigned char carry = 0; // перенос ИЗ старших разрядов
        for (size_t i = 0; i < result.size(); ++i) 
        {
            result[i] =  (i < oper1.digitLength_ ? *(oper1.digits_ + i) : 0) - carry;
            result[i] -= (i < oper2.digitLength_ ? *(oper2.digits_ + i) : 0) ;

            carry = static_cast<unsigned char>((i < oper1.digitLength_ ? *(oper1.digits_ + i) : 0) < 
                        carry + (i < oper2.digitLength_ ? *(oper2.digits_ + i) : 0));
            if (carry)
                result[i] += base;
        }

        size_t resultSize = result.size();

        while(result[resultSize - 1] == 0 && resultSize > 1)
            resultSize--;

        unsigned char * ptr = new unsigned char[resultSize];
        for(size_t i = 0; i < resultSize; ++i)
            *(ptr + i) = result[i];

        delete[] resultBigInteger.digits_;
        resultBigInteger.digits_ = ptr;
        resultBigInteger.digitLength_ = resultSize;
        resultBigInteger.isNegative_ = oper1.isNegative_;

        return resultBigInteger;        
    }
    
}


BigInteger BigInteger::operator-(const BigInteger& other) const
{
    if(*this == other)
        return BigInteger(0);

    BigInteger resultBigInteger(-other);
    resultBigInteger = *this + resultBigInteger;
    return resultBigInteger;
}


BigInteger BigInteger::operator*(const BigInteger& other) const
{
    std::vector<unsigned char> result;
    BigInteger integerToReturn;
    
    if(other == BigInteger(0) || *this == BigInteger(0))
        return BigInteger(0);
    else if(other.abs() < BigInteger(base)) // умножение на малое число
    {
        unsigned char carry = 0;
        int buffer;
        for(size_t i = 0; i < this->digitLength_ || carry; ++i )
        {
            result.push_back(0);
            if (i < this->digitLength_)
                buffer = *(this->digits_ + i) * (*other.digits_) + carry;
            else
                buffer = carry;
                
            result[i] += ( buffer % base);
            carry =      ( buffer / base);
        }
    }   
    else if(this->abs() < BigInteger(base))  // умножение на малое число
    {
        unsigned char carry = 0;
        int buffer;
        for(size_t i = 0; i < other.digitLength_ || carry; ++i )
        {
            result.push_back(0);
            if ( i < other.digitLength_)
                buffer = *(this->digits_) * (*(other.digits_ + i)) + carry;
            else
                buffer = carry;
                
            result[i] += ( buffer % base);
            carry =      ( buffer / base);
        }        
    }
    else // умножение на длинное на длинное число
    {
        //количество разрядов не превышает суммы количества разрядов операндов более, чем на 1
        result.resize(this->digitLength_ + other.digitLength_ + 1, 0);
        for(size_t i = 0; i < this->digitLength_; ++i)
        {
            unsigned char carry = 0;
            int buffer;
            int buffer2;
            for(size_t j = 0; j < other.digitLength_ || carry; ++j)
            {
                if(j < other.digitLength_)
                    buffer = static_cast<int>(*(this->digits_ + i)) * 
                                static_cast<int>((*(other.digits_ + j))) + static_cast<int>(carry);
                else
                    buffer = carry;
                
                buffer2 = static_cast<int>(result[i + j]) + buffer;
                result[i + j] = static_cast<unsigned char>(buffer2 % base);
                carry =          static_cast<unsigned char>(buffer2 / base);
                
            }            
        }
    }

    //удаление лидирующих нулей
    size_t returnSize = result.size();
    for (int i = returnSize -1; i >= 0; --i)
    {
        if(result[i] != 0)
        {
            returnSize = i + 1;
            break;
        }
    }
    
    unsigned char * ptr = new unsigned char[returnSize];

    for(size_t i = 0; i < returnSize; ++i)
        *(ptr + i) = result[i];

    delete[] integerToReturn.digits_;
    integerToReturn.digits_ = ptr;
    integerToReturn.digitLength_ = returnSize;
    integerToReturn.isNegative_ = !(this->isNegative_ == other.isNegative_);
    
    return integerToReturn;
}


// сдвиг разрядов на 1 влево = умножение на base
// требуется в реализации деления большого числа на большое
BigInteger& BigInteger::digitShiftToLeft_()
{
    std::vector<unsigned char> shiftedInteger(this->digitLength_ + 1, 0);
    for(int i = this->digitLength_; i >= 1; --i )
        shiftedInteger[i] = *(this->digits_ + i - 1);

    unsigned char * ptr = new unsigned char[shiftedInteger.size()];

    for(size_t i = 0; i < shiftedInteger.size(); ++i)
        *(ptr + i) = shiftedInteger[i];

    delete[] this->digits_;
    this->digits_ = ptr;
    this->digitLength_ = shiftedInteger.size();

    return *this;
}

BigInteger BigInteger::operator/(const BigInteger& other) const
{

    // проверка деления на нуль
    if(other == BigInteger(0)) 
        throw std::overflow_error("Division by zero exception");
    
    // если делим на большее по модулю число, вернем нуль
    if(this->abs() < other.abs())
    {
        return BigInteger(0);
    }
    // проверка, не делим ли на само себя
    if( other == *this)
    {
        return BigInteger(1);
    }
    else if(other == -*this)
    {
        return BigInteger(-1);
    }
    // деление на малое число < base
    else if (other.abs() < BigInteger(base)) 
    {
        std::vector<unsigned char> result(digitLength_, 0);
        BigInteger integerToReturn;
        unsigned char reminder = 0;
        int buffer;

        for(int i = (digitLength_ - 1); i>=0; --i)
        {
            buffer = reminder * base + *(digits_ + i);
            result[i] = buffer / (*other.digits_);
            reminder =  buffer % (*other.digits_);
        }

        size_t returnSize = result.size();
        if(!result[returnSize - 1] && returnSize != 1)
            --returnSize;


        unsigned char * ptr = new unsigned char[returnSize];
        for(size_t i = 0; i < returnSize; ++i)
            *(ptr + i) = result[i];
        
        delete []  integerToReturn.digits_;
        integerToReturn.digits_ = ptr;
        integerToReturn.digitLength_ = returnSize;
        integerToReturn.isNegative_ = !(this->isNegative_ == other.isNegative_);
        return integerToReturn;
    }
    // деление длинное на длинное, делитель < делимого
    else
    {   
        //количество разрядов не превышает количества рарядов в делимом
        std::vector<unsigned char> result(this->digitLength_, 0);
        BigInteger currentValue;
        
        for(int i = this->digitLength_ - 1; i >= 0; --i)
        {
            
            // подбираем такое максимальное число x: other*x <= currentValue
            *(currentValue.digits_) = *(this->digits_ + i);
            unsigned char x = 0;
            unsigned char l = 0;
            unsigned char r = base;

            while(l <= r)
            {

                unsigned char m = (l + r) >> 1;
                BigInteger curr = other.abs() * BigInteger(m);
                if( curr <= currentValue)
                {
                    x = m;
                    l = m + 1;
                }
                else
                    r = m - 1;
            }
            result[i] = x;
            
            currentValue = currentValue - other.abs() * BigInteger(x);
            currentValue.digitShiftToLeft_();
            
        }

        // удаление лидирующих нулей
        size_t returnSize = result.size();
        for (int i = returnSize -1 ; i >= 0; --i)
        {
            if(result[i] != 0)
                {
                    returnSize = i + 1;
                    break;
                }
        }
            
        unsigned char * ptr = new unsigned char [returnSize];
        for(size_t i = 0; i < returnSize; ++i)
            *(ptr + i) = result[i];
        BigInteger integerToReturn;
        delete[] integerToReturn.digits_;
        integerToReturn.digits_ = ptr;
        integerToReturn.digitLength_ = returnSize;
        integerToReturn.isNegative_ = !(this->isNegative_ == other.isNegative_);

        return integerToReturn;
    }
}


BigInteger& BigInteger::operator++()
{
    *this = *this + BigInteger(1);
    return *this;
}


BigInteger& BigInteger::operator--()
{
    *this = *this - BigInteger(1);
    return *this;
}

//унарный минус
BigInteger BigInteger::operator-() const 
{
    BigInteger result(*this); 
    result.isNegative_ = !this->isNegative_; 
    return result;
}

//модуль числа
BigInteger BigInteger::abs() const
{
    BigInteger result(*this);
    result.isNegative_ = false;
    return result;
}


BigInteger BigInteger::operator++(int /*dummyInt = 0*/)
{
    *this = *this + BigInteger(1);
    return *this;
}


BigInteger BigInteger::operator--(int /*dummyInt = 0*/)
{
    *this = *this - BigInteger(1);
    return *this;
}


BigInteger& BigInteger::operator+=(const BigInteger& other)
{
    *this = *this + other;
    return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& other)
{
    *this = *this - other;
    return *this;
}
BigInteger& BigInteger::operator*=(const BigInteger& other)
{
    *this = *this * other;
    return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& other)
{
    *this = *this / other;
    return *this;
}

void BigInteger::print() const
{
    std::cout << "IsNeg = " << isNegative_ << std::endl;
    std::cout << "Chars: ";
    for(size_t i = 0; i < digitLength_; ++i)
        std::cout << static_cast<int>(*(digits_ + i)) << "; ";
    std::cout << std::endl;
    std::cout << "len = " << digitLength_ << "\n";
}


std::ostream& operator<<(std::ostream& os, const BigInteger& n)
{
    if(n.isNegative_)
        os << '-';
    {
        for(int i = n.digitLength_ - 1; i >=0; --i)
        {
            // если количество разрядов больше 1, то если в разряд не самый старший
            // и в нём записано число меньше 10, до добавим нуль перед ним
            std::string digits = std::to_string(static_cast<int>(*(n.digits_ + i)));
            if(i != n.digitLength_ - 1 && digits.length() < 2)
                digits = '0' + digits;
            os << digits;   
        }
    }

    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& n)
{
    int buffer;
    is >> buffer;
    n = BigInteger(buffer);

    return is;
}