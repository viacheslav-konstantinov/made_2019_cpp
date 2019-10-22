#include <iostream>
#include <stdexcept>

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    ~LinearAllocator();
    char* alloc(size_t size);
    void reset();

private:
    char* beginPtr;
    char* endPtr;
    char* currentPtr;
};

LinearAllocator::LinearAllocator(size_t maxSize)
{
    if (maxSize == 0)
    {
        throw std::invalid_argument("maxSize must be non-zero");
    }
    beginPtr = (char*) malloc(maxSize);
    endPtr = beginPtr + maxSize;
    currentPtr = beginPtr;
}

void LinearAllocator::reset() 
{
    currentPtr = beginPtr;
}

LinearAllocator::~LinearAllocator()
{
    delete beginPtr;
}

char* LinearAllocator::alloc(size_t allocatedSize)
{
    // offset pointer first, align it, and offset it back
    if (currentPtr < endPtr) 
    {
        char* allocatedPtr = currentPtr;
        currentPtr += allocatedSize;
        return allocatedPtr;
    } else 
    {
        std::cout << "Not enough memory" << std::endl;
        return nullptr;
    }
}