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

//only one public function that returns final hashed string.

class MD5{
private:
    static const unsigned int WordA = 19088743;     // 19088743 equivalent to 0x01234567
    static const unsigned int WordB = 2309737967;   // 2309737967 equivalent to 0x89abcdef
    static const unsigned int WordC = 4275878552;   // 4275878552 equivalent to 0xfedcba98 
    static const unsigned int WordD = 1985229328;   // 1985229328 equivalent to 0x76543210

    static string AddPaddingTo(string); // padding is added to make string a multiple of 512 bits. returns string of binary representation of the original inputted string
    static vector<string> Blocks(string); // makes message into 512 bit blocks
    static int* SubBlocks(string);      // recieves message as 512 bits is further broken down to 16 sub blocks of 32 bits into integers;
    static int BitStringToInt(string);  // converts string of binary to integer to hold 32 bits
    static unsigned int round1(unsigned int,unsigned int,unsigned int); //1: (b AND c) OR ((NOT b) AND (d))
    static unsigned int round2(unsigned int,unsigned int,unsigned int); //2: (b AND d) OR (c AND (NOT d))
    static unsigned int round3(unsigned int,unsigned int,unsigned int); //3: b XOR c XOR d
    static unsigned int round4(unsigned int,unsigned int,unsigned int); //4: c XOR (b OR (NOT d))

    /*
        need to make functions for step 3+
    */
public:
    static string Hash(string);         // recieves string as input and returns final hash
    

};