# CPP Argument Parser [![Build Status](https://travis-ci.org/diridari/CPP-Argument-Parser.svg?branch=master)](https://travis-ci.org/diridari/CPP-Argument-Parser)
## Goal
This library shall help you the receive program arguments in C++.

It is possible to define call back function that gets invoked if the corresponding arguments get parsed.

For each argument it is possible to define a short version, a long version a help message. Additional it is possible to mark one argument as required, an pre-defined set of additional parameters, the number of expected parameter’s per argument e.g one additional parather for open, the filepath: `-o <file>`. 

Each argument is separated by an space or is can be joined by <”>

If one argument needs e.g. a parameter like -p <portNumber> your callback function has access to further arguments.

## Features
* os support : Linux, Windows, MacOs(not tested)
* Easy argument registration
* Help message generator
* Mark arguments as required
* Add section to order the arguments
* Linux Bash auto completion
* Organize arguments in sections
* Argument validation
* Define argument parameter as file path or as pre-defined set

***

![Alt text](doc/wrongArg.jpg?raw=true "example")

source:

    // define program description
    argvParser *p = new argvParser("extended example program\n\t this application intends to be an example ");
    // define program arguments
    p->addArg("-t","--test","test argument",testCallBacl);
    
    p->addArg("-f","--foo","foo test argument  required argument example",fooCallBack)
            ->required()->numberOfParameter(1);
            
    p->addArg("-e", "--enums", "enum example", enumCallBack)->numberOfParameter(1)
            ->allowedParameter(3, "abc", "def", "xyz");
    p->addSection("logging");
    p->addArg("-l","--logging" ,"enable logging",loggingCallBack);
    // check if all arguments are valid
    if(!p->analyseArgv(argvs,argv)){
        p->printHelpMessage(!disableCliH);
    }
    // check if all required arguments have been parsed
    if(!p->foundAllRequierdArgs()){
        cout << "you have not entered at least one required argument  \n\t -f has been marked as an required argument" 
         <<"try it         with -f"<<endl;
    }
    
## Usage

### Register an Argument
To register a callback function:

     argvParser parser("your program description"); // program description
     // short version | long version | description of argument | callBackFunction | required to use this argument default = false
     parser.addArg("shortARG", "longARG", "description of argument",callBackFunction);

It is possible to add additional configuration after the addArg method like:
* mark as required
* define number of parameters
* auto completion definitions(as file or as set of pre-defined parameters)

e.g.

     parser.addArg("short","long", "description",callBackFunction)->required()->asFile->[....];
     
### Check all Arguments
To check all Arguments use:

    parser.analyseArgv(args,argv); // returns true if no unknown argumends and all required arguments have been called

it returns true if all required arguments or any unknown argument had been used

### Call Back Function
If one argument gets transferred to the program the library searches the corresponding call back function and executes it.
Each call back function must have following signature:

     /**
     *   @param  index  	current index
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
    
    
