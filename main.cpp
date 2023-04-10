#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>

#include "header.h"

#define DES_SALT_ALPHABET "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

using namespace std;

const string passwordFileName = "passwordFile.txt";
const string plaintextPasswordsFile = "passwordPlaintextFile.txt";
const string batchUserPasswordFile = "possiblePasswords.txt";

const int SALTLENGTH = 6;
const string MAGIC = "$";

//
void addUser();
void verifyPassword();
void saveUser(string userID, string password);
void parseUserInfo(string userInfo[], string info);
void batchAddUsers();
bool uniqueID(string userID);
string getUserInfo(string userID);
string generateSalt();
void sortRainbowTable();


int systeminterface() // this is to make VScode happy -- duplicate main functions
//int main()
{
    srand(time(0));

    int answer = 1;//stores input of menu selection
    while (answer != 0)
    {
        //show the menu
        cout << "  0. exit" << endl;
        cout << "  1. to add a user" << endl;
        cout << "  2. to verify password of a user" << endl;
        cout << "  3. batch add user passwords from file [ " << batchUserPasswordFile << " ]" << endl;
        cout << "  4. sort rainbow table" << endl;

        //set [answer] to a fixed option if you need to bulk add/verify users
        cin >> answer;
        
        //reply to selection
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
        case 3:
            batchAddUsers();
        case 4:
            sortRainbowTable();
        default:
            cout << "invalid selection, please try again\n";
            break;
        }
    }

    return 0;
}

void addUser()
{
    string userID;  //name of the user
    string password; //plaintext password input
    cout << "enter a unique user ID: ";
    cin >> userID;

    //forbid duplicate users
    while (!uniqueID(userID))
    {
        cout << "Sorry, that user ID is taken. \nPlease try again: ";
        cin >> userID;
    }

    cout << "enter a password: ";
    cin >> password;

    //write user to the file
    saveUser(userID, password);
}

void verifyPassword()
{
    string userID;    //user name input
    string password; //user's password input
    string hashedInfo; //hash of the info
    string readpasswordline;//raw line from passwordfile
    string userInfo[3];//split password information -- should probably be a struct


    //enter userID
    cout << "Enter the userID: ";
    cin >> userID;
    while (uniqueID(userID))
    {
        cout << "The userID [ " << userID << " ] does not exist,\n please try again: ";
        cin >> userID;
    }
    
    //enterpassword
    cout << "Enter " << userID << " password: ";
    cin >> password;

    //get entry from password file
    readpasswordline = getUserInfo(userID);
    //split into strings -- (de-MAGIC it)
    parseUserInfo(userInfo, readpasswordline);


    string salt = userInfo[2];
    string savedHash = userInfo[3];
    hashedInfo = MD5::Hash(salt + password);

    //compare the hash with hashedInfo
    if (savedHash.compare(hashedInfo) == 0)
    {
        cout << "Password verified!\n";
    }
    else
    {
        cout << "Password is incorrect\n";
    }
}

void batchAddUsers(){
    cout << "this will add 500 new users from file [ " << batchUserPasswordFile << " ] " <<endl;
    cout << "are you sure you want to continue? [y/n]: " ;

    string input;
    cin >> input;
    if(input[0] != 'y'){
        cout << "input was not 'y'. aborting" << endl;
        return;
    }

    ifstream pwdfile;  //stream to read the password file
    string line;       //gets a line from the passwordFile
    
    pwdfile.open(batchUserPasswordFile);
    if(!pwdfile.is_open()){
        cout << "file not OPen: in uniqueID" << endl;
        throw "file not OPen: in uniqueID";
    }

    string sessionSalt = generateSalt()+generateSalt();
    int count =0;
    //read ever line till we match -- this could be optimized a lot
    while (getline(pwdfile, line))
    {
        cout << "adding: [ " << count << " ] " ; //<<endl;//save user function could add the newline
        saveUser("batched_" + sessionSalt + '_' + to_string(count), line);
        count++;
    }
    


    return;
}

void saveUser(string userID, string password)
{
    string salt;//stores random salt created
    string hash;//stores hash of salt +userID

    salt = generateSalt();
    hash = MD5::Hash(salt + string(password));
    
    //open password and plaintextPasswords
    ofstream pwdfile(passwordFileName, std::ios::app);
    ofstream plainfile(plaintextPasswordsFile, std::ios::app);

    //error check
    if (!(pwdfile.is_open() || plainfile.is_open()))
    {
        // file didn't open
        cout << "in funct, saveuser, file didn't open" << endl;
        throw "in funct, saveuser, file didn't open";
    }

    //write info
    pwdfile << MAGIC << userID << MAGIC << salt << MAGIC << hash << "\n";
    pwdfile.close();

    //write as plaintext because I won't remember their passwords
    plainfile << userID << ' '<< password << endl;
    plainfile.close();

    //
    cout << "User saved\n";
}


bool uniqueID(string userID)
{
    ifstream pwdfile;  //stream to read the password file
    string line;       //gets a line from the passwordFile
    
    pwdfile.open(passwordFileName);
    if(!pwdfile.is_open()){
        cout << "file not OPen: in uniqueID" << endl;
        throw "file not OPen: in uniqueID";
    }

    //read ever line till we match -- this could be optimized a lot
    while (getline(pwdfile, line))
    {
        if (line.find(userID) != -1)
        {
            return false;//find method found a matching index, so user is not unique
        }
    }

    //we read the entire file, so the user is not unique
    return true;
}

string generateSalt()
{
    // what is salt?
    // salt is long long
    long long salt = 0;  //holds the random number
    string saltAsString = ""; //the random roll as a base 64 character

    //error check that salt is short enough to be stored
    //I don't know what happens if salt is 0 bits
    if (sizeof(salt) * 8 < SALTLENGTH || SALTLENGTH ==0)
    {
        cout << "SALTLENGTH IS TOO LONG" << endl;
        throw "SALTLENGTH IS TOO LONG";
    }

    //generate random 1 and 0 till we get the correct bit length
    for (size_t i = 0; i < SALTLENGTH; i++)
    {
        salt = (salt << 1) | (rand() & 1);
    }

    //construct the random roll into a base64 string of characters  
    do
    {
        char insert = DES_SALT_ALPHABET[(salt & 0x3f)]; // 3F is 63 or 11 1111
        saltAsString += insert;
        salt = salt >> 6;
    } while (salt);

    //
    return saltAsString;
}

string getUserInfo(string userID)
{
    ifstream pwdfile;
    string line = "";//raw line from the file
    pwdfile.open(passwordFileName);

    while (getline(pwdfile, line))
    {
        if (line.find(userID) != -1)
        {
            //find returned an index that the ID begins on
            break;
        }
    }
    return line;
}

void parseUserInfo(string userInfo[], string info)
{
    string token;//the word we read
    int next = 0;//index to start at
    int last = 0;//index the end at
    int i = 0; //what portion of the userinfo we are filling

    //loop through where we find magic symbols and create substrings
    while ((next = info.find(MAGIC, last)) != -1)
    {
        token = info.substr(last, next - last);
        userInfo[i] = token;//
        last = next + 1;
        i++;
    }
    
    //add in the last substring
    userInfo[i] = info.substr(last);
}

void sortRainbowTable(){
    vector<string> passwords;
    ifstream inFile;
    inFile.open("rainbowTable.txt");

    ofstream outFile;
    outFile.open("rainbowTable2.txt");

    for(int i = 0; i <= 63; i++){
        char salt = DES_SALT_ALPHABET[i];

        string line;
        while(getline(inFile, line)){
            char s = line[0];

            if(s == salt){
                passwords.push_back(line);
            }
        }
    }

    for(int i = 0; i < passwords.size(); i++){
        outFile << passwords[i] << endl;
    }

    cout << "Rainbow Table has been sorted. Results stored in rainbowTable2.txt" << endl;
}