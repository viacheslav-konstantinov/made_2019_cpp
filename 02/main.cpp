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

int main() 
{
    LinearAllocator myAllocator(100*sizeof(char));
    cout << "Allocator of size of 100*char has been created" << endl;

    cout << "Testing alloc():" << endl;
    for(int i = 0; i < 12; ++i)
    {
        cout << "iteration=" << dec << i << " Allocating 10*char..." << endl;
        cout << "Address of the allocated memory: " << hex << (uint64_t) myAllocator.alloc(10*sizeof(char)) << endl;
    }
    cout << "Testing reset():" << endl;
    myAllocator.reset();
    cout << "Allocator has been reset" << endl;

    cout << "Checking if the reserved memory is used after reset(): ";
    cout << boolalpha << myAllocator.ifAllocatedMemoryUsed() << endl;;
    
    cout << "Allocating 10*char..." << endl;
    cout << "Address of the allocated memory: ";
    cout << hex << (uint64_t) myAllocator.alloc(10*sizeof(char)) << endl;

    cout << "Checking if the reserved memory is used: ";
    cout <<  boolalpha << myAllocator.ifAllocatedMemoryUsed() << endl;

    myAllocator.reset();
    cout << "Allocator has been reset" << endl;
    cout << "Checking if the reserved memory is used after reset(): ";
    cout <<  boolalpha << myAllocator.ifAllocatedMemoryUsed() << endl;
    return 0;
}