#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

//only one public function that returns final hashed string.

class MD5{
private:
    //static void AddPaddingTo(); // padding is up to length of new string modulo 512 bits = 64 bits
    //static void AddOriginalLengthOfMessageAsPadding(); // the 64 bits are padded on as the original password length
public:
    static string Hash(string x); // recieves string as input and returns final hash
    

};