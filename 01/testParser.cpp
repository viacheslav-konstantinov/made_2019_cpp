#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "classArthParser.h"
using namespace Catch::literals;


TEST_CASE( "Arithmetic expressions", "[classArthParser]" ) {

    SECTION( "Checking accuracy of computation" ) {
        int res;

        char myStr[] = "85-9*7+99/11";
        res = 85-9*7+99/11;
        ArthParser myParser(myStr);
        REQUIRE(myParser.Calculate() == res);
        
        char myStr1[] = "-12*4-98-19/9";
        res = -12*4-98-19/9;
        myParser.SetString(myStr1);
        REQUIRE(myParser.Calculate() == res);

        char myStr2[] = "-74*2+148-8+27/3";
        res = (-74)*2+148-8+27/3;
        myParser.SetString(myStr2);
        REQUIRE(myParser.Calculate() == res);

        char myStr3[] = "144/-4--8+4*-5";
        res = 144/(-4)-(-8)+4*(-5);
        myParser.SetString(myStr3);
        REQUIRE(myParser.Calculate() == res);

        char myStr4[] = "78";
        res = 78;
        myParser.SetString(myStr4);
        REQUIRE(myParser.Calculate() == res);
    }

    SECTION( "Checking exception catching" ){
        
        char myStr[] = "";
        ArthParser myParser(myStr);
        REQUIRE_THROWS_WITH(myParser.Calculate(), "Empty string, nothing to calculate");

        char myStr2[] = "15/7+4-";
        myParser.SetString(myStr2);
        REQUIRE_THROWS_WITH(myParser.Calculate(), "Invalid syntax: string cannot be ended with operator");

        char myStr3[] = "145-*65";
        myParser.SetString(myStr3);
        REQUIRE_THROWS_WITH(myParser.Calculate(), "Invalid syntax: two operators cannot be one after another");

        char myStr4[] = "74+2-3 k8/2+8*3";
        myParser.SetString(myStr4);
        REQUIRE_THROWS_WITH(myParser.Calculate(), "Invalid syntax: unknown symbol in string");        
    }
}