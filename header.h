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

    void GenerateSalt(); // will random generate a salt
    void AddSaltToPassword(); // self-explanetory
    void AddPaddingToSaltedPassword(); // padding is up to length of new string modulo 512 = 64;
    // next step padding??



};