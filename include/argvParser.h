//
// Created by basto on 4/15/18.
//

#ifndef GDBMANIPULATOR_ARGVPARSER_H
#define GDBMANIPULATOR_ARGVPARSER_H

#include <string>
#include <vector>
#include <cstdarg>


using namespace std;

/**
 * Argument Parser.
 * this class is a generic argument parser
 * set up each command with with an short, long an help message and an callback function which get called
 * if the argument is passed.
 * for the case, that the argument has parameter the callback function has access to the argv array and can return
 * the adjusted index
 *
 *  example usage :
 *      argvParser parser("description");
 *       parser.addArg("-p","--port","add Port",callBackPort);
 *       parser.addArg("-short","--long","add d",callBackD);
 *       if(!parser.analyseArgv(argc,argv)) // true if all passed args had been configured
 *           cout <<arser.getHelpMessage()<<endl; // print help**
 *
 *   example callback :
 *       int callBackPort(int index,char *buff[]){
 *        index++; // increase index
 *        port = buff[index];
 *        return index;
        }
 */
class argvParser {

    typedef struct argument {
        string argShort;
        string argLong;
        int (*callBack)(int, char **);
        bool requiredAndNotHitJet;
        int numberOfArguments;
        argument(string argS, string argL, int (*callBack_)(int, char **), bool required_ = false, int numberOfArguments = -1) : argLong(argL),
                                    argShort(argS), callBack(callBack_),requiredAndNotHitJet(required_),numberOfArguments(numberOfArguments) {};
    } argument;

    typedef struct enumDesciption{
        string enums;
        string toplevelComannd;
        string toplevelShort;
        bool asFile;
    }enumDesciption;

    vector<argument *> *argconfig = new vector<argument *>();

    // auto compleation
    string topLevelArgs = "";
    string lastToplevelLong = "";
    string lastToplevelShort = "";
    vector<enumDesciption> enumsList = vector<enumDesciption>();

    string lastFailedArg;
    string description;
    string requiredArgs;
    int checkArgs(string param);
    bool existArg(string arg);

    string helpMessage;
    string generateAutoCompletion();
public:

    /**
     * Argument Paerser.
     * Pars arguments can call if arg eq long or short version the callback function.
     * Help message = description + list of configured param + required param
     * @param description  description of the application
     */
    argvParser(string description = "");

    /**
     * Add Argument.
     * Add an command line argument with its short and long version and an comment and Callback
     * function with the signatur of : "int name(int i,char *buff[]){}"
     * the the argument is markes ar required and the analyseArgv() does not found the argument in the
     * arg array the result is false.
     * i is not allowed to add duplicate arguments. In this case the new argument does not get added the this class
     * and the function return false
     * @param argvShort     short version of the argument
     * @param argvLong      long version of the argument
     * @param help          argument description message
     * @param callBack      call back function
     * @param numberOfArguments number of additional paramethers -1 == no check   default = -1
     * @param required      is this argument required
     * @return added to commands
     */
    argvParser* addArg(string argvShort, string argvLong, string help, int (*callBack)(int, char **), int numberOfArguments = -1,
                bool required = false);

    /**
     * Help Message.
     * print colored helpmessage to the cli
     */
    void printHelpMessage();

    /**
     * Help Message.
     * returns the generated Help Message. And if analyseArgv() failed with the failed argument
     * @return help string
     */
    string getHelpMessage();

    /**
     * check if all required argument are found
     */
    bool foundAllRequierdArgs();

    /**
     * decode program Arguments.
     * expect that argv[0] is the program name and the first argument is stored at argv[1]
     * return true on success(no errors found) and if all required arguments are found
     */
    bool analyseArgv(int args, char *argv[]);

    /**
     * to order arguments in groups
     * @param sectionName section name
     */
    void addSection(string sectionName);

    bool addEnum(int numberOfEnums, const char *enums, ...);
    bool asFile();
};


#endif //GDBMANIPULATOR_ARGVPARSER_H
