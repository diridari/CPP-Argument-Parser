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

bool disableCliH = false;
int callBachCli(int index, char** buff){
    disableCliH = true;
    return index;
}

int main(int argvs, char** argv) {
    argvParser *p = new argvParser("example Programm\n\t this application intens to be an example ");
    p->addArg("-t","--test","test argument",testCallBacl);
    p->addArg("-f","--foo","foo test argument  equired argument example",fooCallBack,1,true);
    p->addArg("-p","--print","echo text",printCallBack,1);
    p->addArg("-e", "--enums", "enum example", enumCallBack, 1)->allowedParameter(3, "abc", "def", "xyz");
    p->addArg("-o","--open","example to complete a file/dir",printCallBack,1)->asFile();
    p->addArg("-nh","--noHighlight","disabledCli highlighting",callBachCli);
    p->addSection("logging");
    p->addArg("-l","--logging" ,"enable logging",loggingCallBack);
    p->addArg("-logf","--logFile","generate logfile",logFileCallBack);

    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage(!disableCliH);
    }
    if(!p->foundAllRequierdArgs()){
        cout << "you have not entered at least one required argument  \n\t -f has been marked as an required argument try it with -f"<<endl;
    }
    return 0;
}