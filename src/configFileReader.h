//
// Created by basto on 10/16/18.
//

#ifndef CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
#define CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H

#include <iostream>
#include <vector>
#include <fstream>

using  namespace std;

/**
 * This file defines an additional input source of arguments.
 *
 */
class configFileReader {

    /**
     * type of current separator.
     */
    enum typeOfSeparator{
        none,       // no Separator
        space,      // any kind of space
        separator   // any kind of Separator
    };


    /**
     * Name and location of the config file.
     */
    string fileName;

//#################################  JUST For TESTING AND DEBUGGING ###################################
    /**
     * use just for testing!!
     * additional input source.
     */
    string text;

    /**
     * index in the additional input source
     */
    unsigned  int index = 0;

    /**
     * use the additional Input source
     */
    bool readFromString = false;

//########################################################################################################

    /**
     * list of allowed separators
     */
    vector<char> * septarators = new vector<char>();

    /**
     * check one singe char for its type.
     * @param toCHeck   char to check
     * @return          kind of type
     */
    typeOfSeparator isSeparator(char toCHeck);

    /**
     * return the next char.
     * it is possible to iterate overe this source by using this method
     */
    char getNextChar();

    /**
     * jump over the next char.
     */
    void skipNextChar();

    /**
     * peek the next Char
     * if you call this method in a loop all results would be the same char
     * So it is not possible to iterate over the source file
     */
    char peekNextChar();

    /**
     * open an config file
     */
    void openFile();

    /**
     * file descriptor
     */
    ifstream *configFile;
    bool isOpen = false;
public:

    /**
     * create an reader for an specific file
     */
    configFileReader(string configFile){
        fileName = configFile;
        text = "";
        septarators->push_back('"');
    }

    /**
     * use just for Debugging !!
     * @param searchText   aternativ text that migth be in the config file
     */
    void inputSourceIsString(string searchText){
        text = searchText;
        readFromString = true;
    }

    /**
     * check if the last command was the last command in the config file
     */
    bool isEOF();

    /**
     * returns the next command.
     * if the command is joined the howl string gets returned
     * if one joined string does not get closed is get closed automatically at the end of each inout source
     */
    string readUntilNextSeparator();

    /**
     * check if opening the config file has failed
     * @return
     */
    bool faildToOpen();

};


#endif //CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
