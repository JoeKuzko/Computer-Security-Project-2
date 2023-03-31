#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

class MD5{
public:
    string Password; // will be recieved as input though user input
    string Salt; // should be 12 to 48 bits long
    string SaltedPassword; // salt is added to password
    string SaltedPaddedPassword; // string to hold final string after padding

    void GenerateSalt(); // will random generate a salt
    void AddSaltToPassword(); // self-explanetory
    void AddPaddingToSaltedPassword(); // padding is up to length of new string modulo 512 bits = 64 bits
    void AddOriginalLengthOfMessageAsPadding(); // the 64 bits are padded on as the original password length



};