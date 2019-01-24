# CPP Argument Parser [![Build Status](https://travis-ci.org/diridari/CPP-Argument-Parser.svg?branch=master)](https://travis-ci.org/diridari/CPP-Argument-Parser)

Easy to use C++ argument parsing library. It intends to be easy to use and gain a good user experience by generating 
help messages, bash auto completion and by providing a simple to use api. For each argument it is possible to register 
a callback function or a lambda expressions, which gets invoked in case of that the corresponding argument gets parsed 
to your application.

Easy to use C++ argument parsing library. It intends to be easy to use and gain a good user experience by generating 
help messages, bash auto completion and by providing a simple to use api. For each argument it is possible to register 
a callback function or a lambda expressions, which gets invoked if the corresponding argument gets parsed 
to your application.

## Features
* Support OS: Linux, Windows, MacOs(not tested)
* Easy argument registration
* Help message generator
* Linux Bash auto completion
* Mark arguments as required
* Organize arguments in sections
* Argument validation
* Define argument parameter as file path or as pre-defined set
* Lambda support

***

![Alt text](doc/wrongArg.jpg?raw=true "example")

simple example:
    
    argvParser *p = new argvParser("simple example program");
    // Simple Lambda CallBack
    p->addArg("-t","--test","test argument",[](){ testVar = true;}); // no further arguments used
    p->addArg("-l","--logging" ,"enable logging",loggingCallBack);
    
     // check if all arguments are valid
    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage(); 
    }
    
extended example:

    // define program description
    argvParser *p = new argvParser("extended example program\n\t this application intends to be an example ");
    // define program arguments
    // Simple Lambda CallBack
    p->addArg("-t","--test","test argument",[](){ testVar = true;}); // no further arguments used
    
    // Lambda CallBack as required and one additional parameter
    function <int(int,char**)> lambdaCallback = [](int index, char ** buff){ index++; cout << "got \"foo\" with : " << buff[index]<<endl;return index;};
    p->addArg("-f","--foo","foo test argument  required argument example",lambdaCallback)
            ->required()->numberOfParameter(1); // requiered  + one additional paramether      
    p->addArg("-e", "--enums", "enum example", enumCallBack)->numberOfParameter(1)
            ->allowedParameter(3, "abc", "def", "xyz"); // checks that just <abc,def or xyz> are additional parameters
    p->addSection("logging");
    p->addArg("-l","--logging" ,"enable logging",loggingCallBack);
    
    // check if all arguments are valid
       if(!p->analyseArgv(argvs,argv)){
           p->printHelpMessage(!disableCliH);
           // check if all required arguments have been parsed
           if(!p->foundAllRequierdArgs()){
               cout << "you have not entered at least one required argument  \n\t -f has been marked as an required argument try it with -f"<<endl;
           }
       }
    
## Usage

### Register an Argument
To register a callback function:

     argvParser parser("your program description"); // program description
     // short version | long version | description of argument | callBackFunction | required to use this argument default = false
     parser.addArg("shortARG", "longARG", "description of argument",callBackFunction);
     
The call back function can be an lambda expression or an function. (see: "Call Back Function")

It is possible to add additional configuration after the addArg method like:
* mark argument as required (user must use the argument)
* define number of parameters
* auto completion definitions(as file or as set of pre-defined parameters)

e.g.

     parser.addArg("short","long", "description",callBackFunction)->required()->asFile->[....];
     
### Check all Arguments
To check all Arguments use:

    parser.analyseArgv(args,argv); // returns true if no unknown arguments and all required arguments have been called

It returns true if all required arguments or any unknown argument had been used

It is additional possible to check if all required arguments has been parsed by:

    parser.foundAllRequierdArgs();
    
### Call Back Function
If one argument gets transferred to the program the library searches the corresponding call back function and executes it.

#### Lambda Callback
It is possible to define an lambda expression to get executed in case of that the corresponding argument has been parsed.

There are two kind of expression:
* simple lambda callback functions
* extended lambda callback functions

##### Simple Lambda callback function
This expression has no access to additional parameter's.
Such expression has following signature

     function<void()>;

example:

    function<void()> callBack = [] {foo = true;};
    parser.addArg("f","foo", "foo argument",callBack);

or shorter:

    parser.addArg("f","foo", "foo argument",[]{foo=true;});
    
    
##### Extended Lambda callback function
Those expressions do have access to additional parameter, but must return the changed index:
    
    /*  return changed index pointing to the last used buffer element
        gets index and char buffer holding the arguemnts and parameters
    */
    function<int(int,char**)> callback = [](int index, char** buffer){
            index ++; cout << buff[index];return index }
    parser.addArg("f","foo", "foo argument",callback)->numberOfParameter(1);        

#### Function Callback
Each call back function must have following signature:

     /**
     *   @param  index  	last used buffer element
     *   @param  buff 		buffer of stored arguments
     *   @param  out    	next unused index
     */
    int callBackFunction(int index, char *buff[]);

This allows the call back function to access the buffer for further buffer elements like an argument.
If the function does access the buffer it had to increase the index variable by the additional used elements and return it.

## Required Arguments 
Sometimes it is necessary that the user has to call the program with one particular argument to get the program doing its job.

In such cases the argument can be marked as 'required'. Does the application get not called with all as “required“ marked
argument’s the method `analyseArgv` does return false.

e.g.

    p->addArg("-f","--foo","foo test argument  required argument example",fooCallBack)->required();


## Defined number of Parameters
It is possible to define the number of additional parameters. For Each Argument the framework does check if there is enough given arguments to fulfil the callback function. 

      parser.addArg(short, long,description,callBack)
            ->numberOfParameter(<number of additional arguments>);
      parser.addArg("p", "port","adjust the network port",callBackPortTestMeth1)
            ->numberOfParameter(1);
      
 If the value is -1 no check gets executed.
 
The default value is -1.      

## Configuration Files
It is possible to swap often used arguments to a configuration file.
Each additional unknown argument’s gets checked whether it is a configfile or not. Is this the case the configfile gets decoded (like usual arguments)?

## Bash auto completion
To install bash auto completion on Linux systems the framework generates out of your argument description a autogen script 
that can be loaded temporary by `source XX.bash` or persist by copy the file into /etc/bash_completion.d/ 

To generate this script call 'your' program with `-instAutoCompl`

## Pre defined set of parameters 
It is possible to allow just a predefine set of additional arguments by adding an `->allowedParameter(numb, set ...)`

Each parsed argument get checked whether the additional parameters do match with the pre-defined strings. Is this not the case the call back function does not get invoked and the 'parser.analyseArgv' method return false 

    /**
     * defines a set of strings of allowed parameter's
     * @param numberOfEnums  number of arguments
     * @param enums first allowed argument
     * @param ... additional parameter's
     * @return
     */
     bool allowedParameter(int numberOfEnums, const char *enums, ...);

e.g.    
    
    // argument registration 
    p->addArg("-l","--logging" ,"define log level",loggingCallBack)
            ->allowedParameter(9,"None","UserInfo" "CriticError", "Error","Message",
                "Info","Debug","DebugL2","DebugL3");
    
    
## Sections
To organize the arguments into groups it is possible to define sets of arguments. Each set does have a head line.

    p->addSection("logging");
    
    
*** 
# Cmake Integration
To include this project to you own project you can use following cmake configuration:

    include(ExternalProject)
    externalProject_add(
            cppargparserlib
            GIT_REPOSITORY      https://github.com/diridari/CPP-Argument-Parser.git
            CONFIGURE_COMMAND   cmake CMakeLists.txt
            BUILD_IN_SOURCE     ON
            INSTALL_COMMAND     make CPPArgvParsLib    # make target

    )
    ExternalProject_Get_Property(cppargparserlib  SOURCE_DIR)
    ExternalProject_Get_Property(cppargparserlib  BINARY_DIR)
    include_directories(${SOURCE_DIR}/include) # include simpleLog/include
    LINK_DIRECTORIES(${BINARY_DIR}) ## link git build lib

    ...
    
    target_link_libraries(<youTarget> cppargparserlib)
    ADD_DEPENDENCIES(<youTarget> CPPArgvParsLib)

    
