//Program to create the rainbow table

#include <iostream>
#include <string>
#include <fstream>
#include "header.h" //
#include "Timer.h"  //from learnCPP.com, timer class to measure results

using namespace std;

#define DES_SALT_ALPHABET "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" //from freeBSD server salt implementation
const int SALTLENGTH = 6; //amount of salt in bits
const string MAGIC = "$"; //deliminator for entries in the passwordfile

const string possiblePasswordsFile = "top500WorstPasswords.txt"; //from https://xato.net/10-000-top-passwords-6d6380716fe0
const string rainbowTableFile = "rainbowTable.txt";              //file name of rainbow table data
const string loggingFileName = "rainbowLog.txt";                 //log file to measure results



//Function declarations
string hashWord(string);
void sortRainbowTable();


/// @brief creates new rainbow table from stcrach
void createRainbow()
{
    cout << "this will replace the rainbow table." <<endl;
    cout << "it will take several minutes to recompute it" << endl;
    cout << "are you sure you want to continue? [y/n]: " ;

    Timer totalTime; // total time taken process information
    Timer trialTime; // time for a single trial of hashing

    //Open dictionary of possible passwords
    ifstream inFile;
    inFile.open(possiblePasswordsFile);

    //Read all passwords from dictionary
    string line;
    int count = 0;
    int reportStep = count / 10;
    int saltAlphabetSize = string(DES_SALT_ALPHABET).size();

    
    //confirm user wants to re-create rainbow tabl.e
    string input;
    cin >> input;
    if(input[0] != 'y'){
        cout << "input was not 'y'. aborting" << endl;
        return;
    }

    //Create output file for table
    ofstream outFile(rainbowTableFile);
    ofstream logFile(loggingFileName);

    
    logFile<< "password index,	Time to Hash entire salt(seconds),	Total time elapsed (seconds),size of password (characters)"<< endl;
    totalTime.reset();
    trialTime.reset();

    //loop to read possible passwords to hash
    while(getline(inFile, line) && count <10000){
        //for a password, hash every possible salt value
        for(int i=0; i < saltAlphabetSize; i ++){
            string hash = MD5::Hash(DES_SALT_ALPHABET[i] + line);
            outFile <<DES_SALT_ALPHABET[i] << MAGIC << hash << MAGIC << line << endl;
        }

        //report results
        cout << "fully hashed password [ " << count << " ] in [ " << trialTime.elapsed() << " ] seconds" << endl;
        logFile << count << ',' << trialTime.elapsed() << ',' << totalTime.elapsed()<< ',' << line.size()<< endl;
        trialTime.reset();
        count++;
    }
    outFile.close();

    //Sort the file according to salt value
    //TODO, change look up architecture for faster results
    //sortRainbowTable();

    cout << "Finished creation of rainbow table. Results are stored in rainbowTable.txt" << endl;

    return ;
}

/*
/// @brief //TODO, change look up architecture for faster results
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

    inFile.close();

    for(int i = 0; i < passwords.size(); i++){
        outFile << passwords[i] << endl;
    }

    outFile.close();

    cout << "Rainbow Table has been sorted. Results stored in rainbowTable2.txt" << endl;
}
*/