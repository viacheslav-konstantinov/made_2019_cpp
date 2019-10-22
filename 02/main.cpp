#include <iostream>
#include <cstring>
#include <stdexcept>
#include "linearAllocator.h"
using namespace std;

int main() 
{
    LinearAllocator myAllocator(100*sizeof(char));
    cout << "Allocator of size of 100*char has been created" << endl;

    cout << "Testing alloc():" << endl;
    for(int i = 0; i<12; i++)
    {
        cout << "iteration=" << i << " Allocating 10*char..." << endl;
        cout << hex << (uint64_t) myAllocator.alloc(10*sizeof(char)) << endl;
    }
    cout << "Testing reset():" << endl;
    myAllocator.reset();
    cout << "Allocator has been reset" << endl;

    cout << "Address of the beginning of all the reserved memory: ";
    cout << hex << (uint64_t) myAllocator.alloc(00*sizeof(char)) << endl;;
    
    cout << "Allocating 10*char..." << endl;
    myAllocator.alloc(10*sizeof(char));

    cout << "Address of the allocated memory: ";
    cout << hex << (uint64_t) myAllocator.alloc(00*sizeof(char)) << endl;

    myAllocator.reset();
    cout << "Allocator has been reset" << endl;
    cout << "Address of the beginning of all the reserved memory anfter reset(): ";
    cout << hex << (uint64_t) myAllocator.alloc(00*sizeof(char)) << endl;
    return 0;
}