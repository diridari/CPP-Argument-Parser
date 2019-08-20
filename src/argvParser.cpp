//
// Created by basto on 4/15/18.
//

#include <sstream>
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
#define FOLDER_SEPERATOR  ("\\")
#define ENVIREMENT_HOME (getenv("USERPROFILE"))
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
#define FOLDER_SEPERATOR ( "/")
#define ENVIREMENT_HOME (getenv("HOME"))
#endif

extern int callBackInstallAutoCompletion(int index, char **buff);


argParserAdvancedConfiguration * argvParser::addArg(string argvShort, string argvLong, string help, function<void()> callBack) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        if (!existArg(argvShort) && !existArg(argvLong)) {
            newargconfig->push_back(new section());
        }
        newargconfig->back()->arguments->push_back(new argument(argvShort, argvLong, help, callBack));
        return this;
    }
    return nullptr;
}

argParserAdvancedConfiguration *
argvParser::addArg(string argvShort, string argvLong, string help, function<int(int, char **)> callBack) {
    if (!existArg(argvShort) && !existArg(argvLong)) {
        if(newargconfig->size() == 0){ // no section defined jet
            newargconfig->push_back(new section());
        }
        newargconfig->back()->arguments->push_back(new argument(argvShort, argvLong, help, callBack));
        return this;
    }
    return nullptr;
}

argParserAdvancedConfiguration * argvParser::addArg(string argvShort, string argvLong, string help, int (*callBack)(int, char **)) {
    function<int(int, char **)> tmpCallback = [callBack](int i, char **argv) { return callBack(i, argv); };
    return addArg(argvShort, argvLong, help, tmpCallback);
}

argvParser::argvParser(string applicationNam_, string description_, bool addDefaultHelpCommand_, string commentToken_) {
    applicationName = applicationNam_;
    description = description_ + "\n";
    addHelp = addDefaultHelpCommand_;
    commentToken = commentToken_;
    newargconfig = new vector<section*>();
    newargconfig->push_back(new section());
}




void argvParser::printHelpMessage(bool colored) {
    string s;
    if (colored)
        printGreen();
    s += applicationName + "\n";
    s += description + "\nUsage:\n";
    string requiredArgs;
    for(int secIndex = 0; secIndex < newargconfig->size();secIndex++ ){
        section *sec = newargconfig->at(secIndex);
        if(!sec->sectionName.empty()){
            s+= "\n " + sec->sectionName + ": \n";
        }
        for(int argIndex = 0; argIndex<sec->arguments->size();argIndex++){
            argument * arg = sec->arguments->at(argIndex);
            s += buildHelpLine(arg->argShort,arg->argLong,arg->helpMessage);
            if(arg->requiredAndNotHitJet)
                requiredArgs += buildHelpLine(arg->argShort, arg->argLong, arg->helpMessage);
        }
    }
    cout << s<<endl;
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

string argvParser::getHelpMessage() {
    string s;
    s += applicationName + "\n";
    s += description + "\nUsage:\n";
    string requiredArgs;
    for(int secIndex = 0; secIndex < newargconfig->size();secIndex++ ){
        section *sec = newargconfig->at(secIndex);
        if(!sec->sectionName.empty()){
            s+= " " + sec->sectionName + ": \n";
        }
        for(int argIndex = 0; argIndex<sec->arguments->size();argIndex++){
            argument * arg = sec->arguments->at(argIndex);
            s += buildHelpLine(arg->argShort,arg->argLong,arg->helpMessage);
            if(arg->requiredAndNotHitJet)
                requiredArgs += buildHelpLine(arg->argShort, arg->argLong, arg->helpMessage);
        }
    }
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



bool argvParser::analyseArgv(int args, char **argv) {

    if(analyseArgvNotJetRun) {
        analyseArgvNotJetRun = false;
#ifdef __linux__ // auto completion jet just under linux supported
        this->addSection("Argument auto completion");
        this->addArg("-instAutoCompl", "", "install auto completion for cli usage", callBackInstallAutoCompletion)
                ->addAdditionalHelp(
                        "This command generates a bash autocompletion script that can be loaded temporary or permanent.");
        generateAutoCompletion();
#endif
        istringstream tokenStream(defaultConfigFilesLocations);
        string location = "loc not set";
        while(getline(tokenStream,location,' ')){
            if(location.size() >0 && location.at(0) == '~'){
                location.replace(0,1,ENVIREMENT_HOME);
            }
            //cout << " open " << location+"/"+nameOfDefaultConfigFile<<endl;
            analyzeConfigFile(location+FOLDER_SEPERATOR+nameOfDefaultConfigFile);
        }
    }

    if(addHelp) {
        addHelp = false;
        this->addSection("utils");
        // Default help implementation
        this->addArg("-h", "help", "help message or additional information's about an command e.g. \"help <command>\"",
                     [&](int i, char **buff) {
                         printHelpMessage();
                         string s = "";
                         if(buff[i+1] != NULL)
                             s = string(buff[i+1]);
                         cout << getAdditionalHelpFor(s) << endl;
                         exit(0);
                         return -1;
                     });

    }

    for (int i = 1; i < args; i++) {
        if (argv[i] == NULL)
            return false;
        int x;
        argument * arg;
        if ((arg = getArgument(argv[i])) != nullptr) {
            // Simple Callback
            if (arg->simpleCallBack != NULL) {
                arg->simpleCallBack();
            } else {
                int reqSize = arg->numberOfArguments + i + 1;
                bool enoughSpace = args >= reqSize || arg->numberOfArguments == -1;
                if (enoughSpace) {
                    if (checkNextArgumentIfEnum(arg, argv[i + 1])) {
                        i = arg->callBack(i, argv); // call function
                        arg->requiredAndNotHitJet = false; // set to hit if required
                    }
                } else {
                    errorMessage += "\n\nthe argument \"" + arg->argLong + "\" does require: " +
                                    to_string(arg->numberOfArguments) + " parameter";
                    return false;
                }
            }
        } else { // check config file
            if(!analyzeConfigFile(argv[i]))
                return false;

        }
    }
    return foundAllRequierdArgs();
}

bool argvParser::analyzeConfigFile(string fileName) {
    configFileReader *reader = new configFileReader(fileName);
    reader->setCommentChar(commentToken);
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
    return true;
}



bool argvParser::foundAllRequierdArgs() {
    for (int x = 0; x < newargconfig->size(); x++) {
        for(int y = 0; y<newargconfig->at(x)->arguments->size();y++){
            if (newargconfig->at(x)->arguments->at(y)->requiredAndNotHitJet)
                return false;
        }

    }
    return true;
}

void argvParser::addSection(string sectionName) {
    section * sec = new section();
    sec->sectionName = sectionName;
    newargconfig->push_back(sec);
   // helpMessage += "\n " + sectionName + ":\n";
}

bool argvParser::checkNextArgumentIfEnum(argument * arg, char *nextElement) {

    if (arg != nullptr && arg->enums != "") {
        if (arg->enums.find(string(nextElement) + " ") != string::npos)
            return true;
        else if (!arg->enums.empty()) {
            lastFailedArg = arg->argLong;
            errorMessage += "\"" + arg->argLong + "\" does  accept \"" + arg->enums + "\" but not \"" +
                            nextElement + "\"\n";
            return false;
        }


    }
    return true;
}

void argvParser::checkForDefaulConfigFilesIn(string defaultConfigFileName, string  location) {
     nameOfDefaultConfigFile = defaultConfigFileName;
     defaultConfigFilesLocations = location;
}

string argvParser::generateMarkdownArgumentOverview() {

  string md = "";

  md += "# "+ applicationName + "\n";
    string req =  "## Required arguments: \n";
    bool hasReq = false;

  for(int i = 0; i< newargconfig->size();i++) {
      for(int x = 0; x< newargconfig->at(i)->arguments->size();x++) {
          argument * arg = newargconfig->at(i)->arguments->at(x);
          if(arg->requiredAndNotHitJet){
              if(!hasReq){
                  hasReq = true;
                  md += req;
              }
              md += " * " +  arg->argLong + "\n";
          }
    }
  }
  md+= "## Application Arguments \n";
      for(int i = 0; i< newargconfig->size();i++) {
          if (!newargconfig->at(i)->sectionName.empty()) {
              md += "### " + newargconfig->at(i)->sectionName + "\n";
          }
          for (int x = 0; x < newargconfig->at(i)->arguments->size(); x++) {
              argument *arg = newargconfig->at(i)->arguments->at(x);
              md += "#### " + arg->argLong + "\n";
              md += arg->helpMessage + "\n\n";
              md += arg->additionalHelp + "\n\n";
              md += "    short: " + arg->argShort + " \n";
              md += "    long : " + arg->argLong + " \n\n";
              if (arg->numberOfArguments > 0)
                  md += "number of additional parameter: " + to_string(arg->numberOfArguments) + " \n \n ";
              if (arg->enums != "") {
                  md += "allowed parameter: " + arg->enums + "\n";
              }
              if (arg->requiredAndNotHitJet) {
                  md += "This argument is requires \n";
              }

          }
      }
    return md;
}







