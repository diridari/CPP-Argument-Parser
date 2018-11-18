#include <iostream>
#include "include/argvParser.h"
using namespace std;

int testCallBacl(int index,char** buff){
    cout << "got \"test\""<<endl;
    return index; // no further arguments used
}
int fooCallBack(int index, char** buff){
    index++;
    cout << "got \"foo\" with : " << buff[index]<<endl;
    return index; // +1 because one further argument has been used
}
int main(int argvs, char** argv) {
    argvParser *p = new argvParser("example Programm\n\t this application intens to be an example ");
    p->addArg("-t","--test","test argument",testCallBacl);
    p->addArg("-f","--foo","foo test argument ",fooCallBack,true);
    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage();
    }
    if(!p->foundAllRequierdArgs()){
        cout << "you have not entert at least one required argument"<<endl;
    }
    return 0;
}