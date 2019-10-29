#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iomanip>
using namespace std;

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    LinearAllocator(const LinearAllocator&) = delete;
    LinearAllocator(const LinearAllocator&&) = delete;
    LinearAllocator& operator= (const LinearAllocator&) = delete;
    LinearAllocator& operator= (const LinearAllocator&&) = delete;
    ~LinearAllocator();
    char * alloc(size_t size);
    void reset();
    bool ifAllocatedMemoryUsed();

private:
    char * beginPtr_;
    char * endPtr_;
    char * currentPtr_;
};

LinearAllocator::LinearAllocator(size_t maxSize)
{
    if (maxSize == 0)
    {
        beginPtr_ = nullptr;
        endPtr_ = nullptr;
        currentPtr_ = nullptr;
        throw std::invalid_argument("maxSize must be non-zero");
    }
    try
    {
        beginPtr_ = (char*) malloc(maxSize);
        endPtr_ = beginPtr_ + maxSize;
        currentPtr_ = beginPtr_;
    }
    catch (const std::bad_alloc& ba)
    {
        beginPtr_ = nullptr;
        endPtr_ = nullptr;
        currentPtr_ = nullptr;
        throw std::invalid_argument("Not enough memory to allocate");
    }
}

LinearAllocator::~LinearAllocator()
{
    delete beginPtr_;
}

void LinearAllocator::reset() 
{
    currentPtr_ = beginPtr_;
}

bool LinearAllocator::ifAllocatedMemoryUsed()
{
    return !(beginPtr_ == currentPtr_);
}

char * LinearAllocator::alloc(size_t size)
{
    // offset pointer first, align it, and offset it back
    if ((currentPtr_ < endPtr_) and  (currentPtr_+size <= endPtr_) and (size > 0))
    {
        char * allocatedPtr = currentPtr_;
        currentPtr_ += size;
        return allocatedPtr;
    } 
    else 
        return nullptr;
}

TEST_CASE( "Arithmetic expressions", "[classArthParser]" ) 
{
    char * beginPtr;
    LinearAllocator testAllocator(100*sizeof(char));
    
    //adress of the beginning of the allocated memory
    beginPtr = testAllocator.alloc(20*sizeof(char));

    REQUIRE(testAllocator.alloc(30*sizeof(char)) == beginPtr + 20);
    REQUIRE(testAllocator.alloc(40*sizeof(char)) == beginPtr + 20 + 30);
    REQUIRE(testAllocator.alloc(05*sizeof(char)) == beginPtr + 20 + 30 + 40);
    REQUIRE(testAllocator.alloc(01*sizeof(char)) == beginPtr + 20 + 30 + 40 + 5);
    //REQUIRE((int)testAllocator.alloc(100*sizeof(char)) == 0);
}