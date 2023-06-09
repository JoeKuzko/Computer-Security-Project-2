#include "header.h"
//global variables
unsigned int a,b,c,d;
unsigned int state[4];

// function that recieves input from main and starts the functions to ge the hashed value
string MD5::Hash(string input){
    string padded;                      // holds input string after padding is added
    string finalOutput;                 // holds final hashed input string
    vector<string> blocks;              // holds the input string after spilt into blocks of 512 bits
    padded = AddPaddingTo(input);       // input string is sent into function that adds the padding and the return value of the function is assigned in padded
    blocks = Blocks(padded);            // padded input is sent to be separated into blocks of 512 bits and the returned vector is assigned to blocks
    finalOutput = DigestBuffer(blocks); // blocks vector sent into digest buffer to start the hashing process. the returned string is the final hashed input string
    return finalOutput;                 // returns the final hash string to main
}

// function that adds padding to input
string MD5::AddPaddingTo(string input ){
    long long int originalBitSize = 0;                             // holds the original bit size of the input
    string binaryOfInput;                                          // holds the binary representaion of the nput as a string
    originalBitSize = (input.length()*8);                          // calculates the bit size of the input. assigns it to variable
    bitset<8> bits;                                                // variable that holds 8 bits
    for (char c : input) {                                         // loop that gets the binary of the input string
        bits = bitset<8>(c);                                       // gets 8 bits of char c. assigns to variable
        binaryOfInput = binaryOfInput + bits.to_string();          // gets string of bits and concatenates/adds to variable
    }
    binaryOfInput = binaryOfInput + '1';                           // padding is always required so concatenates 1 with variable

    if (( binaryOfInput.length() % 512) != 448 ){                  // block for adding the padding up to multiples of 512 bits - 64
        int paddingNeeded = 448 - (binaryOfInput.length() % 512);  // calculates the padding needed   
        for(int i = 0; i < paddingNeeded ;i++){                    // loops to add the padding to the string
            binaryOfInput = binaryOfInput + '0';                   // adds 0 for the padding value
        }
    }
    // now to add the original length of the input string as padding
    bitset<64> orgStringLength;                                    // variable to hold the 64 bits of padding in binary
    orgStringLength = bitset<64>(originalBitSize);                 // gets the value of the original string bit size and turns it into 64 bit representation
    string temp = orgStringLength.to_string();                     // gets the string version of the 64 bit value
    for(int i = 63; i < temp.length(); i--){                       // loops to add the 64 bit value from least significant bit first to highest as the final padding needed
        binaryOfInput = binaryOfInput + temp[i];
    }
    return binaryOfInput;                                          // returns the final padded input as a string of the binary representation
}

// function to separate the input string into 512 bit strings and puts it into string vector
vector<string> MD5::Blocks(string input){
    vector<string> blocks;
    string temp= "";
    for(char c : input ){
        temp = temp + c;
        if((temp.size()%512) == 0){
            blocks.push_back(temp);
            temp = "";
        }
    }
    return blocks;
}

// function to separate a 512 bit string into 16 smaller strings of 32 bits
int* MD5::SubBlocks(string block){ // 512 bit as string input only
    static int messageArray[16];
    string bucket = "";
    int j = 0;
    for(int i = 0;i < block.length();i++ ){
        bucket = bucket + block[i];
        if ((bucket.length() % 32) == 0){
            int temp = 0;
            temp = BitStringToInt(bucket);
            messageArray[j] = temp;
            j++;
            bucket = "";
        }
    }
    return messageArray;
}

// function that takes 1 of the 16 strings of 32 bits as input and converts it to its integer equivalent
int MD5::BitStringToInt(string subBlock){
    int block = 0;  //32 bit int for sublock
    int j = 31;
    for (int i = 0; i < 32; i++){
        if(subBlock[i] == '1'){
            block = block + pow(2,j);
        }
        j--;
    }
    return block;
}

// function for doing the hashing
string MD5::DigestBuffer(vector<string> blocks){
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
    
    int* M;                                 // will point to the array of 16 integers                   
    state[0] = WordA;
    state[1] = WordB;
    state[2] = WordC;
    state[3] = WordD;
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];    
    for(int z = 0; z < 1000; z++){          // 1000 rounds of hashing
        for(int i = 0;i<blocks.size();i++){ // loops through the amount of 512 bit blocks
            M = SubBlocks(blocks[i]);       // now points to the return value of sublocks
            int k = 0;                      // variable to be incremented for the array of hex constants in header file
            int l = 0;                      // variable to be incremented for the array of unsigned int constants in header file
            //round 1
            for(int j = 0; j< 16;j++ ){     // loops through the array of 16 integers for round 1 of MD5
                unsigned int temp = b;
                b = (Rotate_Left( a + Round1(b,c,d) + M[j] +T[k], S[l])+b);
                a = d;
                d = c;
                c = temp;
                k++;
                l++;
            }
            //round 2
            for(int j = 0; j< 16;j++ ){     // loops through the array of 16 integers for round 2 of MD5
                unsigned int temp = b;
                int p = (5 * j + 1) % 16;
                b = (Rotate_Left( a + Round2(b,c,d) + M[p] +T[k], S[l])+b);
                a = d;
                d = c;
                c = temp;
                k++;
                l++;
            }
            //round 3
            for(int j = 0; j< 16;j++ ){     // loops through the array of 16 integers for round 3 of MD5
                unsigned int temp = b;
                int p = (3 * j + 5) % 16;
                b = (Rotate_Left( a + Round3(b,c,d) + M[p] +T[k], S[l])+b);
                a = d;
                d = c;
                c = temp;
                k++;
                l++;
            }
            //round 4
            for(int j = 0; j< 16;j++ ){     // loops through the array of 16 integers for round 4 of MD5
                unsigned int temp = b;
                int p = 7 * j % 16;
                b = (Rotate_Left( a + Round4(b,c,d) + M[p] +T[k], S[l])+b);
                a = d;
                d = c;
                c = temp;
                k++;
                l++;
            }
        k=0;
        l=0;
        // updates the 4 32 bit "registers" to hold the final hash of the current 512 bit block
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        }
    }
    string finalOutput;

    for(int i = 0; i < 4; i++){
        finalOutput += IntegerToHex(state[i]); // changes the decimal values in the state array to their hex equivalent as a string
    }

    return finalOutput;
}

// function for converting integer decimal value to hexadecimal
string MD5::IntegerToHex(unsigned int n)
{
    string hexNumber = "";                  // string to hold hex number as string of inputted value
    while (n != 0) {
        int remainder = 0;
        char hexChar;
        remainder = n % 16;
        if (remainder < 10) {
            hexChar = remainder + 48;
        }
        else {
            hexChar = remainder + 55;
        }
        hexNumber += hexChar;                    // adding the hex characters to the string
        n = n / 16;
    }
    int i = 0, j = hexNumber.size() - 1;
    while(i <= j)
    {
      swap(hexNumber[i], hexNumber[j]);         // swapping the character orders to get the correct hex order equivalent to the input
      i++;
      j--;
    }
    return hexNumber;
}

unsigned int MD5::Round1(unsigned int b, unsigned int c,unsigned int d){
    return (((b) & (c)) | ((~ (b)) & (d))); // formula for round one of the md5 algorithm auxiliary functions
} 

unsigned int MD5::Round2(unsigned int b,unsigned int c,unsigned int d){
    return (((b) & (d)) | ((c) & (~ (d)))); // formula for round two of the md5 algorithm auxiliary functions
}

unsigned int MD5::Round3(unsigned int b,unsigned int c,unsigned int d){
    return ((b) ^ (c) ^ (d));               // formula for round three of the md5 algorithm auxiliary functions
}

unsigned int MD5::Round4(unsigned int b,unsigned int c,unsigned int d){
    return (c ^ (b | (~ (d))));             // formula for round four of the md5 algorithm auxiliary functions
}

unsigned int MD5::Rotate_Left(unsigned int x, unsigned int n){
    return (x << n) | (x >> (32-n));        // circliar bitshift left operation
}
