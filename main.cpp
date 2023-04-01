#include "header.h"

int main(){
    MD5 md5;
    string h = md5.Hash("hello world");
    cout << h;

    return 0;
}