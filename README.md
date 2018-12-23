# CPP Argument Parser
## Goal
This library shall help you the receive program arguments in C++.

It is possible to define call back function that gets invoked if the corresponding argument get parsed.

For each argument is it possible to define a short version, a long version a help message and if this argument is required to get called by the user.

Each argument is separated by an space or is can be joined by  <”>

If one argument needs e.g. an parameter like -p <portNumber> your callback function has access to further parameters.

## Features
* easy argument registration
* help message generator
* mark arguments as required
* add section to order the arguments
* linux bash auto completion 

![Alt text](doc/wrongArg.PNG?raw=true "example")

source:

    argvParser *parser = new argvParser("example Programm\n\t this application intens to be an example ");
                // short | long | description | callBackFunction | required default = false
    parser->addArg("-t","--test","test argument",testCallBacl);
    parser->addArg("-f","--foo","foo test argument ",fooCallBack,true);
    parser->addArg("-p","--print","echo text",printCallBack);
    if(!parser->analyseArgv(argvs,argv)){
        parser->printHelpMessage();
    }
    if(!p->foundAllRequierdArgs()){
        cout << "you have not entert at least one required argument"<<endl;
    }

## Usage

### Register an Argument
To register an callback function:

     argvParser parser("your program description"); // program description
     // short version | long version | description of argument | callBackFunction | required to use this argument default = false
     parser.addArg("shortARG", "longARG", "description of argument",callBackD, false);


### Check all Arguments
To check all Arguments use:

    parser.analyseArgv(args,argv); // returns true if no unknown argumends and all required arguments have been called

it returns true if all required arguments or any unknown argument had been used

### Call Back Function
if one argument gets transferred to the program the library searches the corresponding call back function and execute it.
Each call back function must have following signature:

     /**
     *   @param  index  	current index
     *   @param  buff 		buffer of stored arguments
     *   @param  out    	next unused index
     */
    int callBackFunction(int index, char *buff[]);

this allows the call back function to access the buffer for further buffer elements like a argument.
If the function does access those buffer it had to increase the index variable by the additional used elements and return it.
## Defined number of Parathers
It is possible to define the number of additional parameters. For Each Argument the framework does check if there is enough given arguments to fulfil the callback function. 

      parser.addArg("p", "port","adjust the network port",callBackPortTestMeth1,<number of additional arguments>);
      parser.addArg("p", "port","adjust the network port",callBackPortTestMeth1,1);
      
 If the value is -1 no check gets executet.
 
The default value is -1.      

## Configfile
It is possible to swap often used arguments to an config file.
Each additional unknown arguments gets checked whether it is an configfile or not. Is this the case the configfile gets decoded (like usual arguments)?
