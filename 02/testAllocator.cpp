#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>

#include "linearAllocator.h"
using namespace Catch::literals;

TEST_CASE( "Arithmetic expressions", "[classArthParser]" ) 
{
    uint64_t beginPtr;
    LinearAllocator testAllocator(100*sizeof(char));
    
    //adress of the beginning of the allocated memory
    beginPtr = (uint64_t) testAllocator.alloc(0*sizeof(char));

    REQUIRE((uint64_t) testAllocator.alloc(20*sizeof(char))==beginPtr+0);
    REQUIRE((uint64_t) testAllocator.alloc(30*sizeof(char))==beginPtr+20);
    REQUIRE((uint64_t) testAllocator.alloc(40*sizeof(char))==beginPtr+20+30);
    REQUIRE((uint64_t) testAllocator.alloc(05*sizeof(char))==beginPtr+20+30+40);
    REQUIRE((uint64_t) testAllocator.alloc(00*sizeof(char))==beginPtr+20+30+40+5);

}