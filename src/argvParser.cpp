//
// Created by basto on 4/15/18.
//

#include "../include/argvParser.h"
#include "configFileReader.h"


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

void printRed() {
    cout << "\033[1;31m";
}

void resetCLI() {
    cout << "\033[0;0m";
}

void printGreen() {
    cout << "\u001B[1;32m";
}

#endif

extern int callBackInstallAutoCompletion(int index, char **buff);


argParserAdvancedConfiguration * argvParser::addArg(string argvShort, string argvLong, string help, function<void()> callBack) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        argconfig->push_back(new argument(argvShort, argvLong, help, callBack));
        helpMessage += buildHelpLine(argvShort, argvLong, help);

        topLevelArgs += argvLong + " ";
        lastToplevelLong = argvLong;
        lastToplevelShort = argvShort;
        return this;
    }
    return nullptr;
}

argParserAdvancedConfiguration *
argvParser::addArg(string argvShort, string argvLong, string help, function<int(int, char **)> callBack) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        argconfig->push_back(new argument(argvShort, argvLong, help, callBack));
        helpMessage += buildHelpLine(argvShort, argvLong, help);

        topLevelArgs += argvLong + " ";
        lastToplevelLong = argvLong;
        lastToplevelShort = argvShort;
        return this;
    }
    return nullptr;
}

argParserAdvancedConfiguration *
argvParser::addArg(string argvShort, string argvLong, string help, int (*callBack)(int, char **)) {
    function<int(int, char **)> tmpCallback = [callBack](int i, char **argv) { return callBack(i, argv); };
    return addArg(argvShort, argvLong, help, tmpCallback);
}


argvParser::argvParser(string description_) : argParserAdvancedConfiguration() {
    description = description_ + "\n";
    requiredArgs = "";

}

void argvParser::printHelpMessage(bool colored) {
    string s;
    if (colored)
        printGreen();
    cout << description << "usage:\n" << helpMessage << endl;

    if (!foundAllRequierdArgs()) {
        if (colored)
            printRed();
        cout << "\nrequired arguments are : \n" << requiredArgs << endl;
        if (colored)
            resetCLI();
    }
    if (colored)
        printRed();
    if (!lastFailedArg.empty())
        cout << "failed argument : \"" << lastFailedArg << "\"";

    if (!errorMessage.empty())
        cout << " " + errorMessage;
    if (colored)
        resetCLI();

    cout << endl << endl;

}


bool argvParser::analyseArgv(int args, char **argv) {
#ifdef __linux__ // auto completion jet just under linux supported
    this->addSection("Argument auto completion");
    this->addArg("-instAutoCompl", "", "install auto completion for cli usage", callBackInstallAutoCompletion);
    generateAutoCompletion();
#endif
    this->addArg("-h","help","help message or additional infomations about an command e.g. \"help <command>\"",[&](int i, char ** buff)
    {printHelpMessage(); cout << getAdditionalHelpFor(string(buff[i+1]))<<endl;exit(0); return -1;});

    for (int i = 1; i < args; i++) {
        if (argv[i] == NULL)
            return false;
        int x;
        if ((x = checkArgs(argv[i])) >= 0) {
            // Simple Callback
            if (argconfig->at(x)->simpleCallBack != NULL) {
                argconfig->at(x)->simpleCallBack();
            } else {
                int reqSize = argconfig->at(x)->numberOfArguments + i + 1;
                bool enoughSpace = args >= reqSize || argconfig->at(x)->numberOfArguments == -1;
                if (enoughSpace) {
                    if (checkNextArgumentIfEnum(argv[i], argv[i + 1])) {
                        i = argconfig->at(x)->callBack(i, argv); // call function
                        argconfig->at(x)->requiredAndNotHitJet = false; // set to hit if required
                    }
                } else {
                    errorMessage += "\n\nthe argument \"" + argconfig->at(x)->argLong + "\" does require: " +
                                    to_string(argconfig->at(x)->numberOfArguments) + " parameter";
                    return false;
                }
            }
        } else { // check config file
            configFileReader *reader = new configFileReader(argv[i]);
            if (!reader->isEOF()) {
                vector<string> *arg = new vector<string>();
                arg->push_back("program Name");
                while (!reader->isEOF()) {
                    string param = reader->readUntilNextSeparator();
                    arg->push_back(param);
                }
                char *buff[arg->size()];
                for (int y = 0; y < arg->size(); y++) {
                    buff[y] = const_cast<char *>(arg->at(y).c_str());
                }
                analyseArgv(arg->size(), buff);
                delete reader;
            } else {
                delete reader;
                return false;
            }
        }
    }
    return foundAllRequierdArgs();
}

string argvParser::getHelpMessage() {
    string s;
    s += description + "usage:\n" + helpMessage + "\n";
    if (!foundAllRequierdArgs()) {
        s += "\nrequired arguments are : \n" + requiredArgs + "\n";
    }

    if (!lastFailedArg.empty()) {
        s += "failed argument : " + lastFailedArg;
        if (!errorMessage.empty()) {
            s += "\n  " + errorMessage;
        }
    }

    return s;
}



bool argvParser::foundAllRequierdArgs() {
    for (int x = 0; x < argconfig->size(); x++) {
        if (argconfig->at(x)->requiredAndNotHitJet) {
            return false;
        }
    }
    return true;
}

void argvParser::addSection(string sectionName) {
    helpMessage += "\n " + sectionName + ":\n";
}

bool argvParser::checkNextArgumentIfEnum(string arg, char *nextElement) {
    int i = checkArgs(arg);
    if (i >= 0) {
        string s = argconfig->at(i)->argShort;
        if (!s.empty()) {
            for (int x = 0; x < enumsList.size(); x++) {
                if (enumsList.at(x).toplevelComannd == argconfig->at(i)->argLong ||
                    enumsList.at(x).toplevelShort == argconfig->at(i)->argShort) {
                    if (enumsList.at(x).enums.find(string(nextElement) + " ") != string::npos)
                        return true;
                    else if (!enumsList.at(x).enums.empty()) {
                        lastFailedArg = arg;
                        errorMessage += "\"" + arg + "\" does  accept \"" + enumsList.at(x).enums + "\" but not \"" +
                                        nextElement + "\"\n";
                        return false;
                    }
                }
            }
        }
    }
    return true;
}





