#include <utility>

//
// Created by basto on 12/25/18.
//

#ifndef GDBMANIPULATOR_ADVANEDCONFIGURATION_H
#define GDBMANIPULATOR_ADVANEDCONFIGURATION_H


#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * helper class
 * show additional function just in case they are requested
 */
class argParserAdvancedConfiguration{

public:
    /**
     * defines a set of strings of allowed parameter's
     * @param numberOfEnums  number of arguments
     * @param enums first allowed argument
     * @param ... additional parameter's
     * @return this
     */
    argParserAdvancedConfiguration * allowedParameter(int numberOfEnums, const char *enums, ...);

    /**
     * defines the next argument is a file or directory path
     * @return
     */
    argParserAdvancedConfiguration * asFile();

    argParserAdvancedConfiguration * required();

    argParserAdvancedConfiguration * numberOfParameter(int number);

protected:
    /**
    * generated help message
    */
    string helpMessage;

    /**
     * list of toplevel args
     */
    string topLevelArgs = "";
    /**
     * the last long top level arg
     */
    string lastToplevelLong = "";

    /**
   * the last short top level arg
   */
    string lastToplevelShort = "";

    const string SIXTYSPACES = "                                                           "; // /t independent

    /**
    * list of required arguments
    */
    string requiredArgs;

    /**
    * additional description for auto completion
    */
    typedef struct enumDesciption{
        string enums;
        string toplevelComannd;
        string toplevelShort;
        bool asFile;
    }enumDesciption;

    /**
    * argument description.
    */
    typedef struct argument {
        string argShort;
        string argLong;
        string helpMessage;
        int (*callBack)(int, char **);
        bool requiredAndNotHitJet = false;
        int numberOfArguments = -1;
        argument(string argS, string argL, string helpMessage, int (*callBack_)(int, char **)) : argLong(
                std::move(argL)),argShort(std::move(argS)), callBack(callBack_), helpMessage(std::move(helpMessage)) {};
    } argument;

    /**
     * list of defined enums
     */
    vector<enumDesciption> enumsList = vector<enumDesciption>();

    /**
     * generate a bash auto completion script
     * @return script   but append the binary name
     */
    string generateAutoCompletion();

    /**
    * list of configured arguments
    */
    vector<argument *> *argconfig = new vector<argument *>();


    /**
    * generate one help message line.
    * @param argvShort  arg short version
    * @param argvLong   arg long version
    * @param help       help message
    * @return  generated line
    */
    string buildHelpLine(string argvShort, string argvLong, string help);


};


#endif //GDBMANIPULATOR_ADVANEDCONFIGURATION_H
