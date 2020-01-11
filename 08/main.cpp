/*
Практическая работа 8
Написать свой контейнер Vector аналогичный std::vector, аллокатор и итератор произвольного доступа для него. 
Из поддерживаемых методов достаточно operator[], push_back, pop_back, empty, size, clear, begin, end, rbegin, rend, resize, reserve.
*/

#include <iostream>
#include <vector>
#include "Vector.hpp"


int main()
{
    std::cout << "This program demostrates the realizations of the class Vector, similar to the std::vector one\n";
    std::cout << "and the auxiliary classes of Iterator and Allocator\n";

    std::cout << "Creation of a Vector with a list {0, 1, 2, 3, 4, 5}\n";
    Vector<int> v = {0, 1, 2, 3, 4, 5};

    std::cout << "Elements of the array:\n";
    size_t i = 0;    
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Pushing back a new element 23\n";
    v.push_back(23);
    std::cout << "Calling v[6]\n";
    std::cout << v[6] << std::endl;
    std::cout << "Checking new size of the Vector:\n";
    std::cout << v.size() << std::endl;
    std::cout << std::endl;

    std::cout << "Resizing the Vector up to the size of 20 with elements = 100\n";
    v.resize(20, 100);

    std::cout << "Elements of the array:\n";
    i = 0;
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Deleting the last element of the Vector\n";
    v.pop_back();

    std::cout << "Elements of the array:\n";
    for(i = 0; i < v.size(); ++i )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << v[i] << std::endl;
    }
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Reserving memory for 100 elements\n";
    v.reserve(100);

    std::cout << "Elements of the array:\n";
    for(i = 0; i < v.size(); ++i)
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << v[i] << std::endl;
    }
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Comparing with the std::vector\n";
    std::cout << "Creation of a std::vector with a list {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}\n";
    std::vector<int> v2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};


    std::cout << "Elements of the array:\n";
    i = 0;    
    for(auto it = v2.begin(); it != v2.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v2.size() << std::endl;
    std::cout << "capacity = " << v2.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Pushing back a new element 23\n";
    v2.push_back(123);   
    std::cout << "Reserving memory for an array of the size 100\n";
    v2.reserve(100);

    std::cout << "Elements of the array:\n";
    i = 0;    
    for(auto it = v2.begin(); it != v2.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "size = " << v2.size() << std::endl;
    std::cout << "capacity = " << v2.capacity() << std::endl;


    return 0;
} 
