# CPP Argument Parser
## Goal
This library shall help you the receive program arguments in C++
It is possible to define call back function that gets invoked if the corresponding argument get parsed.
For each argument is it possible to define a short version, a long version a help message and if this argument is required to get called by the user.

Each argument is separated by an space or is can be joined by  <”>

If one argument needs e.g. an parameter like -p <portNumber> your callback function has access to further parameters.

![Alt text](doc/wrongArg.PNG?raw=true "example")

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

## Conigfile
It is possible to swap often used arguments to an config file.
Each additional unknown arguments gets checked whether it is an configfile or not. Is this the case the configfile gets decoded (like usual arguments)?
