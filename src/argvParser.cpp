//
// Created by basto on 4/15/18.
//

#include "argvParser.h"

using namespace std;

#include <iostream>

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

string argvParser::getHelpMessage() {
    string s = "\u001B[1;32m";
    if (lastFailedArg != "")
        s += "unknown argument : " + lastFailedArg + "\n";
    s += description + "usage:\n" + helpMessage + "\033[0m \n";
    if (requiredArgs != "")
        s += "\n\033[1;31mrequired arguments are : \n " + requiredArgs + "\033[0;0m\n";
    return s;
}

bool argvParser::analyseArgv(int args, char **argv) {


    for (int i = 1; i < args; i++) {
        for (int x = 0; x < argconfig->size(); x++) {
            if (argconfig->at(x)->argShort == argv[i] || argconfig->at(x)->argLong == argv[i]) {
                i = (*argconfig->at(x)->callBack)(i, argv);
                argconfig->at(x)->requiredAndNotHitJet = false; // set to hit if required
                break;
            } else if (x + 1 == argconfig->size()) { // no args hit
                lastFailedArg = argv[i];
                return false;
            }
        }
    }
    return foundAllRequierdArgs();
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
