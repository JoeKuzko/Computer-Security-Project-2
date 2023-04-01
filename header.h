#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

class MD5{
private:
    static string Password; // will be recieved as input though user input
    static string Salt; // should be 12 to 48 bits long
    static string SaltedPassword; // salt is added to password
    static string SaltedPaddedPassword; // string to hold final string after padding

    static void GetPassword(); // gets user inputed password
    static void GenerateSalt(); // will random generate a salt
    static void AddSaltToPassword(); // self-explanetory
    static void AddPaddingToSaltedPassword(); // padding is up to length of new string modulo 512 bits = 64 bits
    static void AddOriginalLengthOfMessageAsPadding(); // the 64 bits are padded on as the original password length
public:
    void Hash(string x); // hashes the final string after padding????


};