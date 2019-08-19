//
// Created by basto on 12/25/18.
//


#include <cstdarg>
#include "argParserAdvancedConfiguration.h"

static string autoCompletionScript;


int callBackInstallAutoCompletion(int index, char **buff) {
#ifdef __linux__
    string programName = string(buff[0]);
    cout << "generate auto completion" << endl;
    int i = 0;
    // extract Binary name
    while ((i = programName.find("/")) != string::npos) {
        programName.erase(0, i + 1);
    }
    string location = programName + ".bash";
    string script = autoCompletionScript + programName + " \n";
    script += "complete -F _function ./" + programName + " \n";
    fstream fileWirter;
    fileWirter.open(location, fstream::out | ios::trunc);
    if (!fileWirter.is_open()) {
        cout << "failed to open " + location << endl;
        return index;
    }
    fileWirter << script;
    fileWirter.close();

    cout << "try it out with: \n\t \"source " + programName + ".bash\"" << endl;
    cout << "to persist the atuo completion by copying the .bash file to /etc/bash_completion.d/ with:  \n\t \"cp "
            "" + programName + ".bash  /etc/bash_completion.d/" + programName + ".bash\" " << endl;

    exit(0);
#elif __WIN32
    cout << "Auto completion not (jet) suportet under Windows"<<endl;
#endif
    return index;
}

string argParserAdvancedConfiguration::getAdditionalHelpFor(string command){

    argument * arg = getArgument(command);
    if(arg == nullptr)
        return "no such command \"" + command +"\"";

    string help = arg->additionalHelp;
    string out = "\n additional help for \"" + command + "\" \n\t <" +  arg->argShort+ ">  <" +
            arg->argLong + ">  \t" + arg->helpMessage;
    if(help != "")
        out += "\n" + help;
    else
        out += "\n\t no additional help available";
    return out;
}

argParserAdvancedConfiguration *
argParserAdvancedConfiguration::allowedParameter(int numberOfEnums, const char *enums, ...) {

    string list;
    va_list vl;
    va_start(vl, enums);

    helpMessage += SIXTYSPACES + " accepted arguments : {";
    for (int i = 0; i < numberOfEnums; i++) {
        helpMessage += string(enums) + ", ";
        list += " " + string(enums);
        enums = va_arg(vl, const char *);
    }
    helpMessage.erase(helpMessage.size() - 2, 2);
    helpMessage += "}\n";
    enumDesciption t;
    t.enums = list + " ";

    t.asFile = false;

    newargconfig->back()->arguments->back()->enums = list +" ";
    return this;

}

string argParserAdvancedConfiguration::generateAutoCompletion() {

    string script = "#/usr/bin/env bash\n_function()\n{\n\n";
    argument * arg;

    for (int i = 0; i < newargconfig->size(); i++) {
        for(int y = 0; y<newargconfig->at(i)->arguments->size();y++){
            arg = newargconfig->at(i)->arguments->at(y);
            script += R"(if [ "${COMP_WORDS[${COMP_CWORD} -1 ]}" == ")" + arg->argLong +
                      R"(" ] || [ "${COMP_WORDS[${COMP_CWORD} -1 ]}" == ")" + arg->argLong +
                      "\" ]; then\n";
            if (arg->enumIsFile) {
                script += "    compopt -o nospace -o dirnames -o filenames\n"
                          "    COMPREPLY=($(compgen  -f \"${COMP_WORDS[${COMP_CWORD}]}\"))      \n";
            } else {
                script += "     COMPREPLY=($(compgen -W \"" + arg->enums +
                          "\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
            }
            script += "\n   el";
        }
    }
    script += "se\n     COMPREPLY=($(compgen -W \"" + topLevelArgs + "\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
    script += "  fi\n compgen -o bashdefault\n}\n";
    script += "complete -F _function ";
    autoCompletionScript = script;
    return script;

}

argParserAdvancedConfiguration *argParserAdvancedConfiguration::asFile() {
    newargconfig->back()->arguments->back()->enumIsFile = true;
    return this;
}

argParserAdvancedConfiguration * argParserAdvancedConfiguration::addAdditionalHelp(string additionalHelp){
    argument *x = newargconfig->back()->arguments->back();
    x->additionalHelp = additionalHelp;
    return this;
}

argParserAdvancedConfiguration *argParserAdvancedConfiguration::required() {
    argument *x = newargconfig->back()->arguments->back();
    x->requiredAndNotHitJet = true;
    requiredArgs += buildHelpLine(x->argShort, x->argLong, x->helpMessage);
    return this;
}


argParserAdvancedConfiguration *argParserAdvancedConfiguration::numberOfParameter(int number) {
    newargconfig->back()->arguments->back()->numberOfArguments = number;

    return this;
}


string argParserAdvancedConfiguration::buildHelpLine(const string argvShort, const string argvLong, const string help) {
    string s = "";
    if (!argvShort.empty())
        s += "     <" + argvShort + ">";
    while (s.size() < 16)
        s += " ";
    if (!argvLong.empty())
        s += " <" + argvLong + "> ";
    while (s.size() < 45)
        s += " ";
    s += " : " + help + "\n";
    return s;
}

argParserAdvancedConfiguration::argument * argParserAdvancedConfiguration::getArgument(string param){

    for(int secIndex = 0; secIndex < newargconfig->size();secIndex++)
    {
        section * sec;
        sec = newargconfig->at(secIndex);

        for(int argIndex = 0; argIndex < sec->arguments->size();argIndex++) {
            argument * arg = sec->arguments->at(argIndex);
                if (param != "" && (arg->argShort == param || arg->argLong == param)) {
                    return arg;
                }
            }
    }
    lastFailedArg = param;
    return nullptr;
}
bool argParserAdvancedConfiguration::existArg(string args) {
    section * sec;
    for (int x = 0; x < newargconfig->size(); x++) {
        sec = newargconfig->at(x);

        for (int y = 0; y < sec->arguments->size(); y++) {
            argument * arg = sec->arguments->at(y);
            if ((arg->argShort == args || arg->argLong == args) && args != "") {
                return true;
            }
        }
    }
    return false;
}