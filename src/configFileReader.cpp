//
// Created by basto on 10/16/18.
//


#include "configFileReader.h"

string configFileReader::readUntilNextSeparator() {

    string out;
    bool isInSeperator = false;
    // go to the next element
    while(!isEOF()){
        typeOfSeparator tmp = isSeparator(peekNextChar());
        if(tmp == typeOfSeparator::none || tmp == typeOfSeparator::separator){
            if(tmp == typeOfSeparator::separator) {
                isInSeperator = true;
                skipNextChar(); // lift over separator
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

configFileReader::typeOfSeparator configFileReader::isSeparator(char toCheck) {
    for(int i = 0; i< septarators->size();i++){
        if(toCheck == septarators->at(i)){
            return typeOfSeparator::separator;
        }else if (toCheck == ' ' || toCheck == '\n' ||toCheck == '\t' ){
            return typeOfSeparator::space;
        }
    }
    return typeOfSeparator::none;
}

char configFileReader::getNextChar() {
    char out;
    if(readFromString){
        out = peekNextChar();
        index ++;
    }else{
        openFile();
        out = (char)configFile->get();
    }
    if(isSeparator(out) == typeOfSeparator::space)
        out = ' ';

    return out;
}

char configFileReader::peekNextChar() {
    char out;
        if(readFromString){
            out = text.at(index);
        }else{
            openFile();
            out = (char)configFile->peek();
        }
    return out;
}

void configFileReader::skipNextChar() {
    if(readFromString){
        index++;
    }else{
        openFile();
       configFile->get();
    }


}

bool configFileReader::isEOF() {
   if(readFromString){
       return index >= text.length();
   }else{
       openFile();
       if((configFile->rdstate() && ios::eofbit) || (configFile->rdstate() && ios::failbit)){
           return true;
       }
       int count = 0;
       while (!(configFile->rdstate() && ios::eofbit) && !(configFile->rdstate() && ios::failbit) && isSeparator(peekNextChar()) == typeOfSeparator::space){
           skipNextChar();
           count ++;
           if(isSeparator(peekNextChar()) != typeOfSeparator::space && peekNextChar() != -1 ) {
               for(int i = 0; i<count;i++) {
                   configFile->unget();
               }
               return false;
           }
       }
       if(peekNextChar() != -1 )
           return false;
       return true;
   }
}

bool configFileReader::faildToOpen() {
    openFile();
    if(readFromString == false && isOpen && configFile->rdstate() && ios::failbit)
        return false;
    return true;
}

void configFileReader::openFile() {
    if(!isOpen){
        configFile = new ifstream();
        configFile->open(fileName.c_str(),std::ifstream::in);
        isOpen = true;
    }
}
