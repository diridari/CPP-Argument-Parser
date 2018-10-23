#include <iostream>
#include <argvParser.h>

using namespace std;


int callBackPort(int i,char *buff[]){
    cout << "set port to "<<buff[i]<<endl;
    (i++);
    return i;
}

int yourCallBack(int i,char *buff[]){
    cout << "Call Back "<<endl;

    return i;
}

int requiredCallBack(int i,char *buff[]) {
    cout << "Call Back required method"<<endl;
    return i;
}

int main(int args, char ** argv) {
    argvParser *parser = new argvParser("bla bla bla");
    parser->addArg("-p","--port","set port to <Value>",callBackPort);
    parser->addArg("-a","--aa","e.g call back",yourCallBack);
    parser->addArg("-r","--req","required arg",requiredCallBack,true);


    if(parser->analyseArgv(args,argv)){
        cout << "parsed all args with success"<<endl;
    }else{
        cout <<parser->getHelpMessage()<<endl;
        if(!parser->foundAllRequierdArgs()){
            cout<<"you have forgotten at least on required argument"<<endl;
        }
    }

    return 0;
}