//
// Created by basto on 4/15/18.
//

#include "../include/argvParser.h"
#include "configFileReader.h"
#include <iostream>
#include <iostream>
#include <argvParser.h>


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
static string autoCompletionScript;
int callBackInstallAutoCompletion(int index, char **buff){
#ifdef __linux__
    string programName = string(buff[0]);
    int i = 0;
    // extreact Binary name
    while ((i = programName.find("/")) != string::npos){
        programName.erase(0,i+1);
    }
    cout << "install auto completion"<<endl;
    string location = programName+".bash";
    string script = autoCompletionScript + programName + " \n\n";
    fstream fileWirte,profile;
    fileWirte.open(location,fstream::out | ios::trunc);
    if(!fileWirte.is_open()){
        cout << "failed to open " + location<<endl;
        return  index;
    }
    fileWirte << script;
    fileWirte.close();

    cout << "try it temporary with: \"source "+programName + ".bash\""<<endl;
    cout << "to persist the atuocompletion by copying the bash file with:  \"cp "
            ""+ programName + ".bash  /etc/bash_completion.d/" + programName + ".bash\" " <<endl;

    exit(0);
#elif __WIN32
    cout << "Auto completion not (jet) suportet under Windows"<<endl;
#endif
    return index;
}

argvParser* argvParser::addArg(string argvShort, string argvLong, string help, int (*callBack)(int, char **),
        int numberOfArguments, bool required) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        argconfig->push_back(new argument(argvShort, argvLong, callBack, required,numberOfArguments));
        helpMessage += "\t<" + argvShort + "> \t <" + argvLong + "> \t : " + help + "\n";
        if (required)
            requiredArgs += "\t<" + argvShort + "> \t <" + argvLong + "> \t : " + help + "\n";
        topLevelArgs += argvLong +" " ;
        lastToplevelLong = argvLong;
        lastToplevelShort = argvShort;
        return this;
    }
    return nullptr;
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
    this->addSection("Argument auto completion");
    this->addArg("-instAutoCompl","","install auto completion for cli usage", callBackInstallAutoCompletion,0);
    generateAutoCompletion();
    for (int i = 1; i < args; i++) {
        int x;

        if( (x = checkArgs(argv[i])) >=0){
            int reqSize = argconfig->at(x)->numberOfArguments +i +1;
            bool enoughSpace =args >= reqSize || argconfig->at(x)->numberOfArguments == -1;
            if(enoughSpace) {
                i = (*argconfig->at(x)->callBack)(i, argv); // call function
                argconfig->at(x)->requiredAndNotHitJet = false; // set to hit if required
            }else{
            helpMessage += "\n\nthe argument " + argconfig->at(x)->argLong + "does require: " + to_string(argconfig->at(x)->numberOfArguments) + "  arguments ";
                return false;
            }
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

void argvParser::addSection(string sectionName) {
    helpMessage += "\n "+sectionName+":\n";
}

bool argvParser::addEnum(int numberOfEnums, const char *enums, ...) {
    string list;
    va_list vl;
    va_start(vl,enums);
    helpMessage += "\t\t\t\t accepted arguments : {";
    for(int i = 0; i< numberOfEnums;i++){
        helpMessage += string(enums) + ", ";
        list += " "+ string(enums);
        enums = va_arg(vl,const char *);
    }
    helpMessage.erase(helpMessage.size()-2,2);
    helpMessage += "}\n";
    enumDesciption t;
    t.enums = list;
    t.toplevelComannd = lastToplevelLong;
    t.toplevelShort = lastToplevelShort;
    t.asFile = false;
    enumsList.push_back(t);
}

string argvParser::generateAutoCompletion() {

    string script = "#/usr/bin/env bash\n_function()\n{\n\n";
        for(int i = 0; i< enumsList.size();i++){
            script += "if [ \"${COMP_WORDS[${COMP_CWORD} -1 ]}\" == \""+enumsList.at(i).toplevelComannd+"\" ] || [ \"${COMP_WORDS[${COMP_CWORD} -1 ]}\" == \""+enumsList.at(i).toplevelShort+"\" ]; then\n";
            if(enumsList.at(i).asFile){
                script += "\tcompopt -o nospace -o dirnames -o filenames\n"
                          "\tCOMPREPLY=($(compgen  -f \"${COMP_WORDS[${COMP_CWORD}]}\"))      \n";
            }else {
                script += "\t COMPREPLY=($(compgen -W \"" + enumsList.at(i).enums +
                          "\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
            }
            script += "\n   el";
        }
        script += "se\n     COMPREPLY=($(compgen -W \""+topLevelArgs +"\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
        script += "  fi\n compgen -o bashdefault\n}\n";
        script += "complete -F _function ";
    autoCompletionScript = script;
    return  script;
}

bool argvParser::asFile() {
    enumDesciption t;
    t.asFile = true;
    t.toplevelComannd = lastToplevelLong;
    t.toplevelShort = lastToplevelShort;
    enumsList.push_back(t);
    return true;
}

