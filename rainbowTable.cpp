//Program to create the rainbow table

#include <iostream>
#include <string>
#include <fstream>
#include "header.h"
#include "Timer.h"

using namespace std;

#define DES_SALT_ALPHABET "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
const int SALTLENGTH = 6;
const string MAGIC = "$";

const string possiblePasswordsFile = "top500WorstPasswords.txt"; //from https://xato.net/10-000-top-passwords-6d6380716fe0
const string loggingFileName = "rainbowLog.txt";



//Function declarations
string hashWord(string);



//Main function
//int hackerInterface() // this is to make VScode happy -- duplicate main functions
int main()
{
    cout << "this will replace the rainbow table." <<endl;
    cout << "it will take several minutes to recompute it" << endl;
    cout << "are you sure you want to continue? [y/n]: " ;

    Timer totalTime;
    Timer trialTime;

    //Open dictionary of possible passwords
    ifstream inFile;
    inFile.open(possiblePasswordsFile);

    //Create output file for table
    ofstream outFile("rainbowTable.txt");
    ofstream logFile(loggingFileName);

    //Read all passwords from dictionary
    string line;
    int count = 0;
    int reportStep = count / 10;
    int saltAlphabetSize = string(DES_SALT_ALPHABET).size();

    

    string input;
    cin >> input;
    if(input[0] != 'y'){
        cout << "input was not 'y'. aborting" << endl;
        return 66;
    }

    
    logFile<< "password index,	Time to Hash entire salt(seconds),	Total time elapsed (seconds),size of password (characters)"<< endl;
    totalTime.reset();
    trialTime.reset();

    while(getline(inFile, line) && count <10000){
        for(int i=0; i < saltAlphabetSize; i ++){
            string hash = MD5::Hash(DES_SALT_ALPHABET[i] + line);
            outFile <<DES_SALT_ALPHABET[i] << MAGIC << hash << MAGIC << line << endl;
        }
        cout << "fully hashed password [ " << count << " ] in [ " << trialTime.elapsed() << " ] seconds" << endl;
        logFile << count << ',' << trialTime.elapsed() << ',' << totalTime.elapsed()<< ',' << line.size()<< endl;
        trialTime.reset();
        count++;
    }
    outFile.close();

    cout << "Finished creation of rainbow table. Results are stored in rainbowTable.txt" << endl;

    return 0;
}

/*
//Function to return the hash of a word
string hashWord(string word){
    int seed = 153;
    int val = 0;
    int size = word.size();
    string hash = "";

    for(int i = 0; i < word.size(); i++){
        //Get value in range 0-93
        val = (val + int (word[i]) * seed) % 64;

        //Get proper ascii character
        hash += DES_SALT_ALPHABET[val];
    }

    return hash;
}
*/