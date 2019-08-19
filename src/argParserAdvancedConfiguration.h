#include <utility>

//
// Created by basto on 12/25/18.
//

#ifndef GDBMANIPULATOR_ADVANEDCONFIGURATION_H
#define GDBMANIPULATOR_ADVANEDCONFIGURATION_H


#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std;

/**
 * helper class
 * show additional function just in case they are requested
 */
class argParserAdvancedConfiguration {

public:
    /**
     * defines a set of strings of allowed parameter's
     * If an argument gets defined with this method the framework does check whether the additional
     * arguments do match with the pre defined strings
     * @param numberOfEnums  number of arguments
     * @param enums first allowed argument
     * @param ... additional parameter's
     * @return this
     */
    argParserAdvancedConfiguration *allowedParameter(int numberOfEnums, const char *enums, ...);

    /**
     * defines the argument is a file or directory path
     * @return this
     */
    argParserAdvancedConfiguration *asFile();

    /**
     * mark the argument as required.
     * It the application does not called with this argument the 'analyseArgv' method does returns false
     * @return this
     */
    argParserAdvancedConfiguration *required();

    /**
     * define the number of additional parameters.
     * before invoking the callback function the framework does check whether the are enough arguments int the buffer left
     * @param number number of arguments
     * @return this
     */
    argParserAdvancedConfiguration *numberOfParameter(int number);


    /**
     * add additional Help messages.
     * This method can be used add add more information about an specific.
     * This information can be accessed via getAdditionalHelpFor(string) or by using the default help command implementation.
     *  e.g -h  <command>  would return the long and the short version of this command and if defined the additional help
     *
     * @param additionalHelp   additional help the add
     * @return this
     */
    argParserAdvancedConfiguration * addAdditionalHelp(string additionalHelp);
protected:
    /**
    * generated help message
    */
    string helpMessage;

    string applicationName;


    const string SIXTYSPACES = "                                                           "; // /t independent



    /**
    * argument description.
    */
    typedef struct argument {
        string argShort;
        string argLong;
        string helpMessage;
        string additionalHelp = "";
        string enums;
        bool enumIsFile;
        function<int(int, char **)> callBack = NULL;
        function<void()> simpleCallBack = NULL;
        bool requiredAndNotHitJet = false;
        int numberOfArguments = -1;

        argument(string argS, string argL, string helpMessage, function<int(int, char **)> callBack_) : argLong(
                std::move(argL)), argShort(std::move(argS)), callBack(callBack_), helpMessage(
                std::move(helpMessage)) {};

        argument(string argS, string argL, string helpMessage, function<void()> lambda_) : argLong(
                std::move(argL)), argShort(std::move(argS)), simpleCallBack(lambda_), helpMessage(
                std::move(helpMessage)) {};
    } argument;

    typedef struct  section{
        string sectionName = "";
        vector <argument *> *arguments = new vector<argument*>();
    }section;



    /**
     * generate a bash auto completion script
     * @return script   but append the binary name
     */
    string generateAutoCompletion();

    /**
    * list of configured arguments
    */
    vector<section*> *newargconfig ;


    /**
    * generate one help message line.
    * @param argvShort  arg short version
    * @param argvLong   arg long version
    * @param help       help message
    * @return  generated line
    */
    string buildHelpLine(string argvShort, string argvLong, string help);

    /**
     * get the additional help for an spceffic command.
     * @param command  command to get additional help
     * @return if command is known additional help else information that no such command excised
     */
    string getAdditionalHelpFor(string command);

    /**
    * get the index of the argument
    */
   // int checkArgs(string param);
    argument * getArgument(string param);
    /**
     * check whether a argument has been configured
     */
    bool existArg(string arg);

    /**
    * last failed argument
    */
    string lastFailedArg;

    string errorMessage;
    

};


#endif //GDBMANIPULATOR_ADVANEDCONFIGURATION_H
