#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>

#include "MD5.cpp"

#define DES_SALT_ALPHABET "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

using namespace std;

const string passwordFileName = "passwordFile.txt";
const string plaintextPasswordsFile = "passwordPlaintextFile.txt";

const int SALTLENGTH = 6;
const string MAGIC = "$";

//
void addUser();
void verifyPassword();
void saveUser(string userID, string password);
void parseUserInfo(string userInfo[], string info);
bool uniqueID(string userID);
string getUserInfo(string userID);
string generateSalt();

int main()
{
    srand(time(0));
    /*
    int answer = 1;
    while (answer != 0)
    {
        cout << "  0. exit" << endl;
        cout << "  1. to add a user" << endl;
        cout << "  2. to verify password of a user" << endl;
        cin >> answer;

        switch (answer)
        {
        case 0:
            break;
        case 1:
            addUser();
            break;
        case 2:
            verifyPassword();
            break;
        default:
            cout << "invalid selection, please try again\n";
            break;
        }
    }
    */

    for (size_t i = 0; i < 50; i++)
    {

        cout << "salt [ " << generateSalt() << endl;
    }

    return 0;
}

void addUser()
{
    string userID;
    cout << "enter a unique user ID: ";
    cin >> userID;
    while (!uniqueID(userID))
    {
        cout << "Sorry, that user ID is taken. \nPlease try again: ";
        cin >> userID;
    }

    string password;
    cout << "enter a password: ";
    cin >> password;

    saveUser(userID, password);
}

void verifyPassword()
{
    string userID, password, hashedInfo, readpasswordline;
    string userInfo[4];

    cout << "Enter the userID: ";
    cin >> userID;
    while (uniqueID(userID))
    {
        cout << "The userID does not exist\n, please try again: ";
        cin >> userID;
    }
    cout << "Enter " << userID << " password: ";
    cin >> password;

    readpasswordline = getUserInfo(userID);

    parseUserInfo(userInfo, readpasswordline);

    string salt = userInfo[2];
    string savedHash = userInfo[3];

    hashedInfo = MD5::Hash(salt + password);

    if (savedHash.compare(hashedInfo) == 0)
    {
        cout << "Password verified!\n";
    }
    else
    {
        cout << "Password is incorrect\n";
    }
}

void saveUser(string userID, string password)
{
    string salt;
    string hash;

    salt = generateSalt();
    hash = MD5::Hash(salt + password);
    ofstream pwdfile(passwordFileName, std::ios::app);
    ofstream plainfile(plaintextPasswordsFile, std::ios::app);

    if (!(pwdfile.is_open() || plainfile.is_open()))
    {
        // file didn't open
        cout << "in funct, saveuser, file didn't open" << endl;
        throw "in funct, saveuser, file didn't open";
    }

    pwdfile << MAGIC << userID << MAGIC << salt << MAGIC << hash << "\n";
    pwdfile.close();

    plainfile.open(passwordFileName, std::ios::app);
    plainfile << MAGIC << userID << MAGIC << salt << MAGIC << password << "\n";
    plainfile.close();

    cout << "User saved\n";
}

bool uniqueID(string userID)
{
    ifstream pwdfile;
    string line, ID;
    int pos;
    pwdfile.open("pwdfile.txt");
    while (getline(pwdfile, line))
    {
        pos = line.find(userID);
        if (pos != -1)
        {
            return false;
        }
    }
    return true;
}

string generateSalt()
{
    // what is salt?
    // salt is long long
    long long salt = 0;

    if (sizeof(salt) * 8 < SALTLENGTH)
    {
        cout << "SALTLENGTH IS TOO LONG" << endl;
        throw "SALTLENGTH IS TOO LONG";
    }

    for (size_t i = 0; i < SALTLENGTH; i++)
    {
        salt = (salt << 1) | (rand() & 1);
    }

    string saltAsString = "";

    do
    {
        char insert = DES_SALT_ALPHABET[(salt & 0x3f)]; // 3F is 63 or 11 1111
        saltAsString += insert;
        salt = salt >> 6;
    } while (salt);

    return saltAsString;
}

string getUserInfo(string userID)
{
    ifstream pwdfile;
    string line = "", ID;
    int pos;
    pwdfile.open(passwordFileName);
    while (getline(pwdfile, line))
    {
        pos = line.find(userID);
        if (pos != -1)
        {
            break;
        }
    }
    return line;
}

void parseUserInfo(string userInfo[], string info)
{

    string delimiter = MAGIC;
    string token;

    int last = 0, next = 0, i = 0;

    while ((next = info.find(delimiter, last)) != -1)
    {
        token = info.substr(last, next - last);
        userInfo[i] = token;
        last = next + 1;
        i++;
    }
    userInfo[i] = info.substr(last);
}