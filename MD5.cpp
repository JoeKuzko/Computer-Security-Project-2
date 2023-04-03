#include "header.h"

string MD5::Hash(string input){
    string padded;
    string finalOutput;
    int* messageArray;
    padded = AddPaddingTo(input);
    messageArray = SubBlocks(padded);

    return finalOutput;
}

string MD5::AddPaddingTo(string input ){
    long long int originalBitSize =0;
    string binaryOfInput;
    originalBitSize = (input.length()*8);
    bitset<8> bits;
    for (char c : input) {
        bits = bitset<8>(c);
        binaryOfInput = binaryOfInput + bits.to_string();
    }
    //cout << (448 - originalBitSize) <<"\n";
    binaryOfInput = binaryOfInput + '1'; // padding is always required

    if (( binaryOfInput.length() % 448) != 0){
        int paddingNeeded = 448 - ( binaryOfInput.length() % 448);
        //cout << paddingNeeded <<"\n";
        for(int i = 0; i < paddingNeeded ;i++){
            binaryOfInput = binaryOfInput + '0';
        }
    }
    // now to add the original length of the input string as padding
    bitset<64> orgStringLength;
    orgStringLength = bitset<64>(originalBitSize);
    binaryOfInput = binaryOfInput + orgStringLength.to_string();

    //cout << orgStringLength;
    //cout <<binaryOfInput << "\n";

    return binaryOfInput;
}

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
    cout << messageArray[15];
    return messageArray;
}

int MD5::BitStringToInt(string subBlock){
    int block = 0;  //32 bit int for sublock
    int j = 31;
    for (int i = 0; i < 32; i++){
        if(subBlock[i] == '1'){
            block = block + pow(2,j);
        }
        j--;
    }
    //cout << block << "\n";
    return block;
}

