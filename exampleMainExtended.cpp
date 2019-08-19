#include <iostream>
#include "include/argvParser.h"

using namespace std;

/**
 * argument that has additional parameter
 */
int printCallBack(int index, char **buff) {
    index++;
    cout << "print : " << buff[index] << endl; // access to the next element
    return index; // +1 because one further argument has been used
}

int loggingCallBack(int index, char **buff) {
    cout << "enable logging" << endl;
    return index; // +1 because one further argument has been used
}

/**
 * argument that has additional parameter
 */
int logFileCallBack(int index, char **buff) {
    index++;
    cout << "log to " << buff[index] << endl;
    return index; // +1 because one further argument has been used
}

/**
 * argument that has additional parameter
 */
int enumCallBack(int index, char **buff) {
    index++;
    string got = string(buff[index]);
    if (got == "abc" || got == "def" || got == "xyz") {
        cout << "got enum " << buff[index] << endl;
    } else {
        cout << "gor wrong enum" << endl;
    }
    return index;
}

bool disableCliH = false;

int main(int argvs, char **argv) {
    // define program description
    argvParser *p = new argvParser("Extendend Example","This application intends to be an example ");
    // define program arguments
    // Simple Lambda CallBack
    p->addArg("-t", "--test", "test argument", []() { cout << "got \"test\"" << endl; }); // no further arguments used);
    // Lambda CallBack as required and one additional parameter
    function<int(int, char **)> lambdaCallback = [](int index, char **buff) {
        index++;
        cout << "got \"foo\" with : " << buff[index] << endl;
        return index;
    };
    p->addArg("-f", "--foo", "foo test argument  required argument example",
              lambdaCallback)->required()->numberOfParameter(1)->addAdditionalHelp("prints the next element to the cli");
    // Function as Callback
    p->addArg("-p", "--print", "echo text", printCallBack)->numberOfParameter(1)->addAdditionalHelp(
            "echo the next argument");
    // pre defined parameter for "-e"  with bash autocompletion
    p->addArg("-e", "--enums", "enum example", enumCallBack)->numberOfParameter(1)->allowedParameter(3, "abc", "def",
                                                                                                     "xyz")
            ->addAdditionalHelp("here is just a pre defined set of arguments allowed");
    // autocompletion with filenames
    p->addArg("-o", "--open", "example to complete a file/dir", printCallBack)->numberOfParameter(1)->asFile();
    // lambda
    p->addArg("-nh", "--noHighlight", "disabledCli highlighting", [] { disableCliH = false; });
    p->addSection("logging");
    p->addArg("-l", "--logging", "enable logging", loggingCallBack);
    p->addArg("-logf", "--logFile", "generate logfile", logFileCallBack);

    // check if all arguments are valid
    if (!p->analyseArgv(argvs, argv)) {
        p->printHelpMessage(!disableCliH);
        // check if all required arguments have been parsed
        if (!p->foundAllRequierdArgs()) {
            cout
                    << "you have not entered at least one required argument  \n\t -f has been marked as an required argument try it with -f"
                    << endl;
        }
    }

    return 0;
}