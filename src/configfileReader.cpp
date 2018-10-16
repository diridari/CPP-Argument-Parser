//
// Created by basto on 10/16/18.
//


#include "configfileReader.h"

string configfileReader::readUntilNextSeperator() {

    string out;
    bool isInSeperator = false;
    // go to the next element
    while(!isEOF()){
        typeOfSeperator tmp = isSeperator(peekNextChar());
        if(tmp == typeOfSeperator::none || tmp == typeOfSeperator::seperator){
            if(tmp == typeOfSeperator::seperator) {
                isInSeperator = true;
                skipNextChar(); // lift over seperator
            }
            break;
        }

        skipNextChar();
    }
    while(!isEOF()){
        typeOfSeperator tmp = isSeperator(peekNextChar());
            if(tmp == typeOfSeperator::none){
                out += getNextChar();

            }else if(isInSeperator && tmp == typeOfSeperator::space) {
                out += getNextChar();
            } else {
                skipNextChar();
                if (out == "")
                    out = readUntilNextSeperator();
                return out;
            }
    }
    return out;


}

configfileReader::typeOfSeperator configfileReader::isSeperator(char toCheck) {
    for(int i = 0; i< septerators->size();i++){
        if(toCheck == septerators->at(i)){
            return typeOfSeperator::seperator;
        }else if (toCheck == ' ' || toCheck == '\n' ||toCheck == '\t' ){
            return typeOfSeperator::space;
        }
    }
    return typeOfSeperator::none;
}

bool configfileReader::isEndofFile() {
    return index == text.size();
}

char configfileReader::getNextChar() {
    char out;
    if(readFromString){
        out = peekNextChar();
        index ++;
    }else{
        if(!isOpen){
            configFile = new ifstream();
            configFile->open(fileName.c_str(),std::ifstream::in);
            isOpen = true;
        }
        out = (char)configFile->get();
    }
    if(isSeperator(out) == typeOfSeperator::space)
        out = ' ';

    return out;
}

char configfileReader::peekNextChar() {
    char out;
        if(readFromString){
            out = text.at(index);
        }else{
            if(!isOpen){
                configFile = new ifstream();
                configFile->open(fileName.c_str(),std::ifstream::in);
                isOpen = true;
            }
            out = (char)configFile->peek();
        }
    return out;
}

void configfileReader::skipNextChar() {
    if(readFromString){
        index++;
    }else{
        if(!isOpen){
            configFile = new ifstream();
            configFile->open(fileName.c_str(),std::ifstream::in);
            isOpen = true;
        }
       configFile->get();
    }


}

bool configfileReader::isEOF() {
   if(readFromString){
       return index >= text.length();
   }else{
       if(!isOpen){
           configFile = new ifstream();
           configFile->open(fileName.c_str(),std::ifstream::in);
           isOpen = true;
       }
       return (configFile->rdstate() && ios::eofbit) || configFile->rdstate() && ios::failbit || configFile->peek() == -1;
   }
}
