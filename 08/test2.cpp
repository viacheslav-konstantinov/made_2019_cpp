#include <iostream>
#include <vector>
#include <algorithm>
#include "Vector.hpp"

int main()
{
    std::cout << "This program is the test #2 of the realizations of the class Vector, similar to the std::vector one\n";
    std::cout << "and the auxiliary classes of Iterator and Allocator\n";

    std::cout << "Creation of a Vector of size 10 with elements = 5\n";
    Vector<int> v(10, 5);

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

    std::cout << "Adding new elements to the array:\n";   
    for(i = 0; i < 5; ++i)
        v.push_back(i+10);


    std::cout << "Elements of the array:\n";  
    for(i = 0; i < v.size(); ++i )
    {
        std::cout << "i = " << i << ", element = ";
        std::cout << v[i] << std::endl;
    }

    std::cout << "is empty = " << v.empty() << std::endl;
    std::cout << "size = " << v.size() << std::endl;
    std::cout << "capacity = " << v.capacity() << std::endl;
    
    std::cout << "Calling for_each of the array:\n";
    std::for_each(v.cbegin(), v.cend(), [](int x) {std::cout << "x*x = " << x*x << std::endl;});
    return 0;
}
