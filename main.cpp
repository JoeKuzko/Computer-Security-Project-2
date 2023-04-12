#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include "Timer.h" //from learnCPP.com external timer library to measure code efficency

#include "header.h"

#define DES_SALT_ALPHABET "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" // from freeBSD, the salt alphabet

using namespace std;

const string passwordFileName = "passwordFile.txt";                        //name of the unix passwords file
const string plaintextPasswordsFile = "passwordPlaintextFile.txt";         //plain text passwords to help us remember them
const string batchUserPasswordFile = "possiblePasswords.txt";              //what can be added from batch user addition
const string rainbowTableFile = "rainbowTable.txt";                        //file of pre-computed hashes

const int SALTLENGTH = 6;                                                  //how many bits of salt to add to user passwords before hash
const string MAGIC = "$";                                                  //character deliminator for passwordfile

//
void addUser();
void verifyPassword();
void saveUser(string userID, string password);
void parseUserInfo(string userInfo[], string info);
void batchAddUsers();
bool uniqueID(string userID);
string getUserInfo(string userID);
string generateSalt();
void createRainbow();
void crackOnRails();
bool crackPassword(string userInfo[]);

int main()
{
    srand(time(0));

    int answer = 1; // stores input of menu selection
    while (answer != 0)
    {
        // show the menu
        cout << "  0. exit" << endl;
        cout << "  1. to add a user" << endl;
        cout << "  2. to verify password of a user" << endl;
        cout << "  3. batch add user passwords from file [ " << batchUserPasswordFile << " ]" << endl;
        cout << "  4. crack a user's password." << endl;
        cout << "  5. sort rainbow table" << endl;
        cout << "  6. create new rainbow table" << endl;

        // set [answer] to a fixed option if you need to bulk add/verify users
        // answer = 4;
        cin >> answer;

        // reply to selection
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
            break;
        case 4:
            crackOnRails();//function to try to crack user passwords from the rainbow table
            break;
        case 5:
            cout << "TODO: implement sort rainbow" << endl;
            cout << "this could have speed improvements with a slightly different coding architeture." << endl;
            break;
        case 6:
            void createRainbow();
            break;
        default:
            cout << "invalid selection, please try again\n";
            break;
        }
    }

    return 0;
}

/// @brief input loop for manual user addition
void addUser()
{
    string userID;   // name of the user
    string password; // plaintext password input
    cout << "enter a unique user ID: ";
    cin >> userID;

    // forbid duplicate users
    while (!uniqueID(userID))
    {
        cout << "Sorry, that user ID is taken. \nPlease try again: ";
        cin >> userID;
    }

    cout << "enter a password: ";
    cin >> password;

    // write user to the file
    saveUser(userID, password);
    cout << "User saved" << endl;
}

/// @brief input loop for manual user password verification
void verifyPassword()
{
    string userID;           // user name input
    string password;         // user's password input
    string hashedInfo;       // hash of the info
    string readpasswordline; // raw line from passwordfile
    string userInfo[3];      // split password information -- should probably be a struct

    // enter userID
    cout << "Enter the userID: ";
    cin >> userID;
    while (uniqueID(userID))
    {
        cout << "The userID [ " << userID << " ] does not exist,\n please try again: ";
        cin >> userID;
    }

    // enterpassword
    cout << "Enter " << userID << " password: ";
    cin >> password;

    // get entry from password file
    readpasswordline = getUserInfo(userID);
    // split into strings -- (de-MAGIC it)
    parseUserInfo(userInfo, readpasswordline);

    string salt = userInfo[2];
    string savedHash = userInfo[3];
    hashedInfo = MD5::Hash(salt + password);

    // compare the hash with hashedInfo
    if (savedHash.compare(hashedInfo) == 0)
    {
        cout << "Password verified!\n";
    }
    else
    {
        cout << "Password is incorrect\n";
    }
}

/// @brief addusers in bulk from the [batchpasswordfile]
void batchAddUsers()
{
    int addUserAmount = 777;             //how many additional users to append
    ifstream pwdfile;                    // stream to read the password file
    string line;                         // gets a line from the passwordFile
    string sessionSalt = generateSalt(); //a random character to make batch users unique
    vector<string> passwordsPossible;    //every possible password we may select

    cout << "this will add [ " << addUserAmount << " ] new users from file [ " << batchUserPasswordFile << " ] " << endl;
    cout << "are you sure you want to continue? [y/n]: ";

    string input;
    cin >> input;
    if (input[0] != 'y')
    {
        cout << "input was not 'y'. aborting" << endl;
        return;
    }

    //open passwordfile and error check
    pwdfile.open(batchUserPasswordFile);
    if (!pwdfile.is_open())
    {
        cout << "file not OPen: in uniqueID" << endl;
        throw "file not OPen: in uniqueID";
    }


    //read all possible passwords
    while (getline(pwdfile, line))
    {
        passwordsPossible.push_back(line);
    }
    cout << "found [ " << passwordsPossible.size() << " ] possible passwords" << endl;

    //loop to save users
    for (int count = 0; count < addUserAmount; count++)
    {
        //report progress of adding users
        if (count % 50 == 0)
        {
            cout << "adding user: [ " << count + 1 << " ] " << endl; //<<endl;//save user function could add the newline
        }

        //select a password and save it
        int roll = rand() % passwordsPossible.size();
        string temp = passwordsPossible.at(roll);
        saveUser("bat-" + sessionSalt + '-' + to_string(count), temp);
        passwordsPossible.erase(passwordsPossible.begin() + roll);//remove password, technichally this means passwords are more likely to be unique, so IDK if we should remove them
    }

    return;
}

/// @brief Writes user information into the passwordfile
/// @param userID unique ID of the user being added
/// @param password the plaintext password of the user
void saveUser(string userID, string password)
{
    string salt; // stores random salt created
    string hash; // stores hash of salt +userID

    salt = generateSalt();
    hash = MD5::Hash(salt + string(password));

    // open password and plaintextPasswords
    ofstream pwdfile(passwordFileName, std::ios::app);
    ofstream plainfile(plaintextPasswordsFile, std::ios::app);

    // error check
    if (!(pwdfile.is_open() || plainfile.is_open()))
    {
        // file didn't open
        cout << "in funct, saveuser, file didn't open" << endl;
        throw "in funct, saveuser, file didn't open";
    }

    // write info
    pwdfile << MAGIC << userID << MAGIC << salt << MAGIC << hash << "\n";
    pwdfile.close();

    // write as plaintext because I won't remember their passwords
    plainfile << userID << ' ' << password << endl;
    plainfile.close();

    //
    // cout << "User saved\n";
}

/// @brief check if a userID is unique to the system
/// @param userID id to be checked
/// @return true if the user name isn't taken
bool uniqueID(string userID)
{
    ifstream pwdfile; // stream to read the password file
    string line;      // gets a line from the passwordFile

    pwdfile.open(passwordFileName);
    if (!pwdfile.is_open())
    {
        cout << "file not OPen: in uniqueID" << endl;
        throw "file not OPen: in uniqueID";
    }

    // read ever line till we match -- this could be optimized a lot
    while (getline(pwdfile, line))
    {
        if (line.find(userID) != -1)
        {
            return false; // find method found a matching index, so user is not unique
        }
    }

    // we read the entire file, so the user is not unique
    return true;
}

/// @brief creates a random salt character
/// @return random salty tears... (returns random salt value)
string generateSalt()
{
    // what is salt?
    // salt is long long
    long long salt = 0;       // holds the random number
    string saltAsString = ""; // the random roll as a base 64 character

    // error check that salt is short enough to be stored
    // I don't know what happens if salt is 0 bits
    if (sizeof(salt) * 8 < SALTLENGTH || SALTLENGTH == 0)
    {
        cout << "SALTLENGTH IS TOO LONG" << endl;
        throw "SALTLENGTH IS TOO LONG";
    }

    // generate random 1 and 0 till we get the correct bit length
    for (size_t i = 0; i < SALTLENGTH; i++)
    {
        salt = (salt << 1) | (rand() & 1);
    }

    // construct the random roll into a base64 string of characters
    do
    {
        char insert = DES_SALT_ALPHABET[(salt & 0x3f)]; // 3F is 63 or 11 1111
        saltAsString += insert;
        salt = salt >> 6;
    } while (salt);

    //
    return saltAsString;
}

/// @brief looks are password file for the user id paramater from file
/// @param userID the name we look for
/// @return raw line entry of ID, SALT, PASSWORD-HASH
string getUserInfo(string userID)
{
    ifstream pwdfile;
    string line = ""; // raw line from the file
    pwdfile.open(passwordFileName);

    while (getline(pwdfile, line))
    {
        if (line.find(userID) != -1)
        {
            // find returned an index that the ID begins on
            break;
        }
    }
    return line;
}

/// @brief seperates a raw line entry of user information
/// @param userInfo //4 index array to store seperate values
/// @param info //raw line read from file 
void parseUserInfo(string userInfo[], string info)
{
    string token; // the word we read
    int next = 0; // index to start at
    int last = 0; // index the end at
    int i = 0;    // what portion of the userinfo we are filling

    // loop through where we find magic symbols and create substrings
    while ((next = info.find(MAGIC, last)) != -1)
    {
        token = info.substr(last, next - last);
        userInfo[i] = token; //
        last = next + 1;
        i++;
    }

    // add in the last substring
    userInfo[i] = info.substr(last);
}

/// @brief looks at multiple entries in the passwordfile and tries to crack them from the rainbow table.
void crackOnRails()
{
    ofstream crackLog("crackLog.txt");//log file for time measurements
    ifstream pwdfile;     // stream to read the password file
    vector<string> lines; // stores all user data
    string line;          // gets a line from the passwordFile
    int trials;           // how many times we will look through our rainbow table
    int cracks = 0;       // success rates
    Timer trialTimer;     // timing class to measure results

    // open passwords
    pwdfile.open(passwordFileName);
    if (!pwdfile.is_open())
    {
        cout << "file not OPen: in uniqueID" << endl;
        throw "file not OPen: in uniqueID";
    }

    // read every line
    while (getline(pwdfile, line))
    {
        lines.push_back(line);
    }

    // get indexes
    cout << "how many random indexes would you like to try? [0 - " << lines.size() << " ] " << endl;
    cin >> trials;

    // try random indexes
    crackLog << "trial number, time (seconds),outcome" << endl;
    for (int i = 0; i < trials; i++)
    {   
        trialTimer.reset();
        cout << "trying to find index [ " << i << endl;
        int roll = rand() % lines.size();
        string userinfo[4];

        parseUserInfo(userinfo, lines.at(roll));//get individual components of the users information

        //try to crack
        if (crackPassword(userinfo))
        {
            //password broken
            cracks++;
            crackLog << i << ',' << trialTimer.elapsed() << ',' << "success" << endl;
        }
        else
        {
            //cracking failed
            crackLog << i << ',' << trialTimer.elapsed() << ',' <<"failure" << endl;
        }

        lines.erase(lines.begin() + roll); // get rid of indexes we already look at
        
    }

    // report findings
    cout << "ran [ " << trials << " ] trials. cracked [ " << cracks << " ] passwords" << endl;
    cout << ((double)cracks) / trials << " ] % success" << endl;
    cout << "last Trial took [ " << trialTimer.elapsed() << endl;
}

/// @brief looks at the rainbow table to look up [userInfo]
/// @param userInfo information from passwordfile to try to reverse
/// @return true if password is cracked
bool crackPassword(string userInfo[])
{
    ifstream inFile(rainbowTableFile);
    string line;
    
    // Lookup password in the rainbow table
    string key = userInfo[2] + "$" + userInfo[3];
    int loc;//used to keep track of string indexes

    //read lines to find the hash
    while (getline(inFile, line))
    {
        loc = line.find(key);

        // Verify if the key has been found
        if (loc != -1)
        {
            //[line] contain rainbow table entry of the hash combo
            // cout << "found entry" << endl;
            break;
        }
    }

    // Alert user if the salt/hash combo has not been found in the rainbow table
    if (loc == -1)
    {
        return false;
        // cout << "Error finding salt/hash in rainbow table." << endl;
    }
    else
    {
        // Extract password from the line of the rainbow table
        string userName = userInfo[1];
        loc = line.find(MAGIC);
        loc = line.find(MAGIC, loc + 1);

        //print out the cracked password
        string password = line.substr(loc + 1);
        cout << "The password for user [ " << userName << " ] is: [ " << password << endl;

        return true;
    }
}