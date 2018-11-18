//
// Created by basto on 4/15/18.
//

#include "../include/argvParser.h"
#include "configFileReader.h"
#include <iostream>

#ifdef __WIN32
#include <windows.h>
#endif
using namespace std;

#ifdef __WIN32
void printRed(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
}
void resetCLI(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void printGreen(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
}
#endif
#ifdef __linux__
void printRed(){
 cout << "\033[1;31m";
}
void resetCLI(){
 cout << "\033[0;0m";
}
void printGreen(){
cout << "\u001B[1;32m";
}
#endif
bool argvParser::addArg(string argvShort, string argvLong, string help, int (*callBack)(int, char **), bool required) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        argconfig->push_back(new argument(argvShort, argvLong, callBack, required));
        helpMessage += "\t<" + argvShort + "> \t <" + argvLong + "> \t : " + help + "\n";
        if (required)
            requiredArgs += "\t<" + argvShort + "> \t <" + argvLong + "> \t : " + help + "\n";
        return true;
    }
    return false;
}

argvParser::argvParser(string description_) {
    description = description_ + "\n";
    requiredArgs = "";
}

void argvParser::printHelpMessage() {
    if (lastFailedArg != "") {
        printRed();
        cout<< "unknown argument : " + lastFailedArg <<endl;
    }
    printGreen();
    cout<<description + "usage:\n" + helpMessage <<endl;
    if (requiredArgs != "") {
        printRed();
        cout << "\nrequired arguments are : \n " + requiredArgs<<endl;

    }
    resetCLI();
}

int argvParser::checkArgs(string param) {
    for (int x = 0; x < argconfig->size(); x++) {
        if (argconfig->at(x)->argShort == param || argconfig->at(x)->argLong == param) {
           return x;
        } else if (x + 1 == argconfig->size()) { // no args hit
            lastFailedArg = param;
            return -1;
        }
    }
}

bool argvParser::analyseArgv(int args, char **argv) {


    for (int i = 1; i < args; i++) {
        int x;
        if((x = checkArgs(argv[i])) >=0){
            i = (*argconfig->at(x)->callBack)(i, argv); // call function
            argconfig->at(x)->requiredAndNotHitJet = false; // set to hit if required
        }else{
            configFileReader * reader = new configFileReader(argv[i]);
            if(!reader->isEOF()){
                vector <string> *arg  = new vector<string>();
                arg->push_back("program Name");
                while(!reader->isEOF()){
                    string param = reader->readUntilNextSeparator();
                   arg->push_back(param);
                }
                char*  buff[arg->size()];
                for(int y = 0; y<arg->size();y++){
                    buff[y] = const_cast<char *>(arg->at(y).c_str());
                }
                analyseArgv(arg->size(), buff);
                delete reader;
            }else {
                delete reader;
                return false;
            }
        }
    }
    return foundAllRequierdArgs();
}
string argvParser::getHelpMessage() {
    string s = "\u001B[1;32m";
    if (lastFailedArg != "")
        s += "unknown argument : " + lastFailedArg + "\n";
    s += description + "usage:\n" + helpMessage + "\033[0m \n";
    if (requiredArgs != "")
        s += "\n\033[1;31mrequired arguments are : \n " + requiredArgs + "\033[0;0m\n";
    return s;
}
bool argvParser::existArg(string arg) {
    for (int x = 0; x < argconfig->size(); x++) {
        if (argconfig->at(x)->argShort == arg || argconfig->at(x)->argLong == arg) {
            return true;
        }
    }
    return false;
}

bool argvParser::foundAllRequierdArgs() {
    for (int x = 0; x < argconfig->size(); x++) {
        if (argconfig->at(x)->requiredAndNotHitJet) {
            return false;
        }
    }
    return true;
}

