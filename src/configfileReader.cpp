//
// Created by basto on 10/16/18.
//


#include "configfileReader.h"

string configfileReader::readUntilNextSeparator() {

    string out;
    bool isInSeperator = false;
    // go to the next element
    while(!isEOF()){
        typeOfSeparator tmp = isSeparator(peekNextChar());
        if(tmp == typeOfSeparator::none || tmp == typeOfSeparator::seperator){
            if(tmp == typeOfSeparator::seperator) {
                isInSeperator = true;
                skipNextChar(); // lift over seperator
            }
            break;
        }

        skipNextChar();
    }
    while(!isEOF()){
        typeOfSeparator tmp = isSeparator(peekNextChar());
            if(tmp == typeOfSeparator::none){
                out += getNextChar();

            }else if(isInSeperator && tmp == typeOfSeparator::space) {
                out += getNextChar();
            } else {
                skipNextChar();
                if (out == "")
                    out = readUntilNextSeparator();
                return out;
            }
    }
    return out;


}

configfileReader::typeOfSeparator configfileReader::isSeparator(char toCheck) {
    for(int i = 0; i< septerators->size();i++){
        if(toCheck == septerators->at(i)){
            return typeOfSeparator::seperator;
        }else if (toCheck == ' ' || toCheck == '\n' ||toCheck == '\t' ){
            return typeOfSeparator::space;
        }
    }
    return typeOfSeparator::none;
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
    if(isSeparator(out) == typeOfSeparator::space)
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

bool configfileReader::faildToOpen() {
    if(readFromString == false && isOpen && configFile->rdstate() && ios::failbit)
    return false;
}
