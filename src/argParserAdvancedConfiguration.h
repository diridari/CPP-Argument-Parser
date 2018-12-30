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
 * Helper class
 * show additional function just in case they are requested
 */
class argParserAdvancedConfiguration{

public:
    /**
     * defines a set of strings of allowed parameter's
     * @param numberOfEnums  number of arguments
     * @param enums first allowed argument
     * @param ... additional parameter's
     * @return
     */
    bool allowedParameter(int numberOfEnums, const char *enums, ...);

    /**
     * defines the next argument is a file or directory path
     * @return
     */
    bool asFile();

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
    * additional description for auto completion
    */
    typedef struct enumDesciption{
        string enums;
        string toplevelComannd;
        string toplevelShort;
        bool asFile;
    }enumDesciption;

    /**
     * list of defined enums
     */
    vector<enumDesciption> enumsList = vector<enumDesciption>();

    /**
     * generate a bash auto completion script
     * @return script   but append the binary name
     */
    string generateAutoCompletion();


};


#endif //GDBMANIPULATOR_ADVANEDCONFIGURATION_H
