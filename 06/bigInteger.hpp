#ifndef __BIG_INTEGER_H__
#define __BIG_INTEGER_H__

#include <iostream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <string>

// Основание системы счисления в BigInteger
const unsigned char base(100);

class BigInteger
{
    public:
        BigInteger();        
        ~BigInteger();

        // В конструторах, при перемещении и копированиии можно использовать и BigInteger, и int
        BigInteger(BigInteger&& inputInteger);

        BigInteger(const BigInteger& other);
        BigInteger& operator=(const BigInteger& other);
        BigInteger& operator=(BigInteger&& other);

        BigInteger(const int other);
        BigInteger& operator=(const int other);

        //операторы сравнения
        bool operator<(const BigInteger& other) const;
        bool operator==(const BigInteger& other) const;
        bool operator!=(const BigInteger& other) const;
        bool operator>(const BigInteger& other) const;
        bool operator<=(const BigInteger& other) const;
        bool operator>=(const BigInteger& other) const;

        //арифметические операции
        BigInteger operator+(const BigInteger& other) const;
        BigInteger operator-(const BigInteger& other) const;
        BigInteger operator*(const BigInteger& other) const;
        BigInteger operator/(const BigInteger& other) const;
        
        BigInteger& operator+=(const BigInteger& other);
        BigInteger& operator-=(const BigInteger& other);
        BigInteger& operator*=(const BigInteger& other);
        BigInteger& operator/=(const BigInteger& other);
        
        //унарные операторы
        BigInteger& operator++(); // pre-increment
        BigInteger& operator--(); // pre-decrement
        BigInteger operator++(int /*dummyInt = 0*/); // post-increment
        BigInteger operator--(int /*dummyInt = 0*/); // post-decrement
        
        //унарный минус
        BigInteger operator-() const;

        //модуль числа
        BigInteger abs() const;

        friend std::istream& operator>>(std::istream& is, BigInteger& n);
        friend std::ostream& operator<<(std::ostream& os, const BigInteger& n);
        
        //побитная печать числа
        void print() const;

    private:

        size_t digitLength_;     // количество разрядов
        bool isNegative_;        // флаг отрицательности числа
        unsigned char * digits_; // указатель на массив разрядов, каждый разряд = 0...99
        BigInteger& digitShiftToLeft_(); // сдвиг разрядов на 1 влево == умножение на основание
        
};

#endif
