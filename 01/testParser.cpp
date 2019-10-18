#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace Catch::literals;

int GetStdoutFromCommand(std::string cmd) {

    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) 
            data.append(buffer);
        pclose(stream);
    }
    return std::stoi(data);
}

TEST_CASE( "Arithmetic expressions", "[classArthParser]" ) {

    SECTION( "Checking accuracy of computation" ) {
        
        int res = 85-9*7+99/11;
        std::string cmd = "./arithmeticParser 85-9*7+99/11";
        REQUIRE(GetStdoutFromCommand(cmd) == res);

        res = -12*4-98-19/9;
        cmd = "./arithmeticParser -12*4-98-19/9";
        REQUIRE(GetStdoutFromCommand(cmd) == res);

        res = (-74)*2+148-8+27/3;
        cmd = "./arithmeticParser -74*2+148-8+27/3";
        REQUIRE(GetStdoutFromCommand(cmd) == res);

        res = 144/(-4)-(-8)+4*(-5);
        cmd = "./arithmeticParser 144/-4--8+4*-5";
        REQUIRE(GetStdoutFromCommand(cmd) == res);

        res = 78;
        cmd = "./arithmeticParser 78";
        REQUIRE(GetStdoutFromCommand(cmd) == res);
    }
}
