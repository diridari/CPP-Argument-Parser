//
// Created by basto on 10/16/18.
//

#ifndef CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
#define CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H

#include <iostream>
#include <vector>
#include <fstream>

using  namespace std;

class configFileReader {

    enum typeOfSeparator{
        none,
        space,
        seperator
    };


    string fileName;
    string text;
    unsigned  int index = 0;
    bool readFromString = false;

    vector<char> * septerators = new vector<char>();
    typeOfSeparator isSeparator(char toCHeck);

    char getNextChar();
    void skipNextChar();
    char peekNextChar();
    void openFile();
    ifstream *configFile;
    bool isOpen = false;
public:

    configFileReader(string configFile){
        fileName = configFile;
        text = "";
        septerators->push_back('"');
    }

    /**
     * use just for Debugging
     * @param searchText
     */
    void inputSourceIsString(string searchText){
        text = searchText;
        readFromString = true;
    }
    bool isEOF();
    string readUntilNextSeparator();
    bool faildToOpen();

};


#endif //CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
