//
// Created by basto on 10/16/18.
//

#ifndef CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
#define CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H

#include <iostream>
#include <vector>
#include <fstream>

using  namespace std;

class configfileReader {

    enum typeOfSeperator{
        none,
        space,
        seperator
    };


    string fileName;
    string text;
    unsigned  int index = 0;
    bool readFromString = false;

    vector<char> * septerators = new vector<char>();
    typeOfSeperator isSeperator(char toCHeck);

    char getNextChar();
    void skipNextChar();
    char peekNextChar();
    bool isEOF();
    ifstream *configFile;
    bool isOpen = false;
public:

    configfileReader(string configFile){
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

    string readUntilNextSeperator();
    bool isEndofFile();

};


#endif //CPP_ARGUMENT_PARSER_CONFIGFILEREADER_H
