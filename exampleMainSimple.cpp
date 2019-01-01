#include <iostream>
#include "include/argvParser.h"
using namespace std;

/**
 * callback function that get called if the corresponding argument get parsed
 */
int testCallBacl(int index,char** buff){
    cout << "got \"test\""<<endl;
    return index; // no further arguments used
}

/**
 * argument that has additional parameter
 */
int printCallBack(int index, char** buff){
    index++;
    cout << "print : " << buff[index]<<endl;
    return index; // +1 because one further argument has been used
}

int main(int argvs, char** argv) {
    // define program description
    argvParser *p = new argvParser("simple example program\n\t this application intends to be an example ");
    // define program arguments
    p->addArg("-t","--test","test argument",testCallBacl);
    p->addArg("-o","--open","example to complete a file/dir",printCallBack,1)->asFile();

    // check if all arguments are valid
    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage(true);
    }
    return 0;
}