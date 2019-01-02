//
// Created by basto on 12/25/18.
//


#include <cstdarg>
#include "argParserAdvancedConfiguration.h"

static string autoCompletionScript;


int callBackInstallAutoCompletion(int index, char **buff){
#ifdef __linux__
    string programName = string(buff[0]);
    cout << "generate auto completion"<<endl;
    int i = 0;
    // extract Binary name
    while ((i = programName.find("/")) != string::npos){
        programName.erase(0,i+1);
    }
    string location = programName+".bash";
    string script = autoCompletionScript + programName + " \n";
    fstream fileWirter;
    fileWirter.open(location,fstream::out | ios::trunc);
    if(!fileWirter.is_open()){
        cout << "failed to open " + location<<endl;
        return  index;
    }
    fileWirter << script;
    fileWirter.close();

    cout << "try it out with: \n\t \"source "+programName + ".bash\""<<endl;
    cout << "to persist the atuo completion by copying the .bash file to /etc/bash_completion.d/ with:  \n\t \"cp "
            ""+ programName + ".bash  /etc/bash_completion.d/" + programName + ".bash\" " <<endl;

    exit(0);
#elif __WIN32
    cout << "Auto completion not (jet) suportet under Windows"<<endl;
#endif
    return index;
}




argParserAdvancedConfiguration * argParserAdvancedConfiguration::allowedParameter(int numberOfEnums, const char *enums, ...) {

    string list;
    va_list vl;
    va_start(vl,enums);

    helpMessage += SIXTYSPACES + " accepted arguments : {";
    for(int i = 0; i< numberOfEnums;i++){
        helpMessage += string(enums) + ", ";
        list += " "+ string(enums);
        enums = va_arg(vl,const char *);
    }
    helpMessage.erase(helpMessage.size()-2,2);
    helpMessage += "}\n";
    enumDesciption t;
    t.enums = list + " ";
    t.toplevelComannd = lastToplevelLong;
    t.toplevelShort = lastToplevelShort;
    t.asFile = false;
    enumsList.push_back(t);
    return this;

}

string argParserAdvancedConfiguration::generateAutoCompletion() {

    string script = "#/usr/bin/env bash\n_function()\n{\n\n";
    for(int i = 0; i< enumsList.size();i++){
        script += R"(if [ "${COMP_WORDS[${COMP_CWORD} -1 ]}" == ")" +enumsList.at(i).toplevelComannd+
                  R"(" ] || [ "${COMP_WORDS[${COMP_CWORD} -1 ]}" == ")" +enumsList.at(i).toplevelShort+"\" ]; then\n";
        if(enumsList.at(i).asFile){
            script += "    compopt -o nospace -o dirnames -o filenames\n"
                      "    COMPREPLY=($(compgen  -f \"${COMP_WORDS[${COMP_CWORD}]}\"))      \n";
        }else {
            script += "     COMPREPLY=($(compgen -W \"" + enumsList.at(i).enums +
                      "\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
        }
        script += "\n   el";
    }
    script += "se\n     COMPREPLY=($(compgen -W \""+topLevelArgs +"\" -- \"${COMP_WORDS[${COMP_CWORD}]}\"))   \n";
    script += "  fi\n compgen -o bashdefault\n}\n";
    script += "complete -F _function ";
    autoCompletionScript = script;
    return  script;

}

argParserAdvancedConfiguration * argParserAdvancedConfiguration::asFile() {
    enumDesciption t;
    t.asFile = true;
    t.toplevelComannd = lastToplevelLong;
    t.toplevelShort = lastToplevelShort;
    enumsList.push_back(t);
    return this;
}

argParserAdvancedConfiguration *argParserAdvancedConfiguration::required() {
    argument *x = argconfig->back();
    x->requiredAndNotHitJet = true;
    requiredArgs += buildHelpLine(x->argShort,x->argLong,x->helpMessage);
    return this;
}


argParserAdvancedConfiguration *argParserAdvancedConfiguration::numberOfParameter(int number) {
    argument *x = argconfig->back();
    x->numberOfArguments = number;
    return this;
}


string argParserAdvancedConfiguration::buildHelpLine(const string argvShort, const string argvLong, const string help) {
    string s = "";
    if(!argvShort.empty())
        s +=  "     <" + argvShort + ">";
    while(s.size() < 16)
        s += " ";
    if(!argvLong.empty())
        s += " <" + argvLong + "> ";
    while(s.size() < 45)
        s += " ";
    s += " : " + help + "\n";
    return s;
}

