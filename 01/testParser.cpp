#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "classArthParser.h"


TEST_CASE( "Arithmetic expressions", "[classArthParser]" ) {
    int res = 85-9*7+99/11;
    ArthParser myParser (std::to_string(res));
    REQUIRE(myParser.calculate() == res);

    res = -12*4-98-19/9;
    myParser.setString(std::to_string(res));
    REQUIRE(myParser.calculate() == res);

    res = (-74)*2+148-8+27/3;
    myParser.setString(std::to_string(res));
    REQUIRE(myParser.calculate() == res);

    res = 144/(-4)-(-8)+4*(-5);
    myParser.setString(std::to_string(res));
    REQUIRE(myParser.calculate() == res);

    res = 78;
    myParser.setString(std::to_string(res));
    REQUIRE(myParser.calculate() == res);
}
