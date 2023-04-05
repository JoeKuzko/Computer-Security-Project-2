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
    static string AddPaddingTo(string); // padding is added to make string a multiple of 512 bits. returns string of binary representation of the original inputted string
    static vector<string> Blocks(string); // makes message into 512 bit blocks and returns vector with those blocks
    static int* SubBlocks(string);      // recieves message as 512 bits is further broken down to 16 sub blocks of 32 bits into integers;
    static int BitStringToInt(string);  // converts string of binary to integer to hold 32 bits
    static string DigestBuffer(vector<string>);     // starts the digest process taking in a vector of the 512 bit blocks
    static string DecToHexa(unsigned int);      // changes decimal int to hexadecimal string for output;

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

    //used for the Rotate left function for how much to bit shift
    static constexpr unsigned int S[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12,
    17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14,
    20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

    //used in digest buffer for adding into the hashing process
    static constexpr unsigned T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

public:
    static string Hash(string);         // recieves string as input and returns final hashed 128 bit string as hex
    

};