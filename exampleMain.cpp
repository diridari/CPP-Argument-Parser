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

int printCallBack(int index, char** buff){
    index++;
    cout << "print : " << buff[index]<<endl;
    return index; // +1 because one further argument has been used
}
int loggingCallBack(int index, char** buff){
    cout << "enable logging"<<endl;
    return index; // +1 because one further argument has been used
}
int logFileCallBack(int index, char** buff){
    index++;
    cout << "log to "<< buff[index]<<endl;
    return index; // +1 because one further argument has been used
}

int enumCallBack(int index, char** buff){
    index++;
    string got = string(buff[index]);
    if(got == "abc" || got == "def" || got == "xyz")
    {
        cout << "got enum " << buff[index]<<endl;
    }else{
        cout << "gor wrong enum"<<endl;
    }
    return index;
}


int main(int argvs, char** argv) {
    argvParser *p = new argvParser("example Programm\n\t this application intens to be an example ");
    p->addSection("usage example section");
    p->addArg("-t","--test","test argument",testCallBacl);
    p->addSection("required argument example");
    p->addArg("-f","--foo","foo test argument ",fooCallBack,true);
    p->addSection("auto check for additional parameter");
    p->addArg("-p","--print","echo text",printCallBack,1);
    p->addSection("enum example");
    p->addArg("-e", "--enums", "enum example", enumCallBack, 1)->addEnum(3, "abc", "def", "xyz");
    p->addSection("example file auto completion (enable autoCompletion with -instAutoCompl)");
    p->addArg("-o","--open","example to complete a file/dir",printCallBack,1);
    p->addSection("logging");
    p->addArg("-l","--logging" ,"enable logging",loggingCallBack);
    p->addArg("-logf","--logFile","generate logfile",logFileCallBack);


    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage();
    }
    if(!p->foundAllRequierdArgs()){
        cout << "you have not entert at least one required argument"<<endl;
    }
    return 0;
}