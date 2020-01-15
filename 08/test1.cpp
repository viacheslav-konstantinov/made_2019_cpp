#include <iostream>
#include <vector>
#include "Vector.hpp"


int main()
{
    std::cout << "This program is the test #1 of the realizations of the class Vector, similar to the std::vector one\n";
    std::cout << "and the auxiliary classes of Iterator and Allocator\n";

    std::cout << "Creation of a Vector of size 10 with random elements\n";
    Vector<int> v(10);

    std::cout << "Elements of the array:\n";
    size_t i = 0;    
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "is empty = " << v.empty() << std::endl;
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;

    std::cout << "Clearing the array:\n";
    v.clear();
    i = 0;    
    for(auto it = v.begin(); it != v.end(); ++it )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << *it << std::endl;
        ++i;
    }

    std::cout << "is empty = " << v.empty() << std::endl;
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    std::cout << std::endl;
    
    return 0;
}
