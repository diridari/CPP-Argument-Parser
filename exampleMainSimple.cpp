#include <iostream>
#include "include/argvParser.h"

using namespace std;


/**
 * argument that has additional parameter
 */
int printCallBack(int index, char **buff) {
    index++;
    cout << "print : " << buff[index] << endl;
    return index; // +1 because one further argument has been used
}

int main(int argvs, char **argv) {

    // define program description
    argvParser *p = new argvParser("simple example program\n\t this application intends to be an example ");
    // define program arguments
    // lambda expression
    p->addArg("-t", "--test", "simple lambda expression", [] { cout << "got test" << endl; })->addAdditianlHelp("type \"help -t  to get this info \"");
    // lambda expression
    function<int(int, char **)> callBack = [](int i, char **buff) {
        i++;
        cout << "extended lambda with parameter : \"" << buff[i] << "\"" << endl;
        return i;
    };
    p->addArg("-x", "--xx", "lambda expression argument", callBack)->numberOfParameter(1);
    // callBack function
    p->addArg("-o", "--open", "example to complete a file/dir", printCallBack)->numberOfParameter(1);

    // check if all arguments are valid
    if (!p->analyseArgv(argvs, argv)) {
        p->printHelpMessage(true);
    }
    return 0;
}