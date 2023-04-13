#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <bitset>
#include <vector>

using namespace std;


class MD5{
private:
    static string AddPaddingTo(string);             // padding is added to make string a multiple of 512 bits. returns string of binary representation of the original inputted string
    static vector<string> Blocks(string);           // makes message into 512 bit blocks and returns vector with those blocks
    static int* SubBlocks(string);                  // recieves message as 512 bits is further broken down to 16 sub blocks of 32 bits into integers;
    static int BitStringToInt(string);              // converts string of binary to integer to hold 32 bits
    static string DigestBuffer(vector<string>);     // starts the digest process taking in a vector of the 512 bit blocks
    static string IntegerToHex(unsigned int);       // changes decimal int to hexadecimal string for output;

    //four auxiliary functions that take 3 32 bit "words" and produce one 32 bit word for the 4 rounds of MD5
    static unsigned int Round1(unsigned int,unsigned int,unsigned int); //1: (b AND c) OR ((NOT b) AND (d))
    static unsigned int Round2(unsigned int,unsigned int,unsigned int); //2: (b AND d) OR (c AND (NOT d))
    static unsigned int Round3(unsigned int,unsigned int,unsigned int); //3: b XOR c XOR d
    static unsigned int Round4(unsigned int,unsigned int,unsigned int); //4: c XOR (b OR (NOT d))

    //for digest buffer uses
    static unsigned int Rotate_Left(unsigned int x, unsigned int n); // n is the number x is shifted to the left

    //Four word buffers for message digest use. used as "32 bit registers". first initialized to certain hex values converted to int
    static const unsigned int WordA = 19088743;     // 19088743 equivalent to 0x01234567 
    static const unsigned int WordB = 2309737967;   // 2309737967 equivalent to 0x89abcdef
    static const unsigned int WordC = 4275878552;   // 4275878552 equivalent to 0xfedcba98 
    static const unsigned int WordD = 1985229328;   // 1985229328 equivalent to 0x76543210


public:
    static string Hash(string);         // recieves string as input and returns final hashed 128 bit string as hex
    

};
