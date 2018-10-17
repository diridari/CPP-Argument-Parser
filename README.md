# CPP Argument Parser
##Goal
This library shall help you the receive program arguments in C++
It is possible to define call back function that gets invoked if the corresponding argument get parsed.
For each argument is it possible to define an short version, a long version a help message and if this argument is required to get called by the user.

Each argument is separated by an space or is can be joined by  <”>

If one argument needs e.g. an parameter linke -p <portNumber> your callback function has access to further parameters.

## Usage
### Call Back Functions
Each callback function needs following signature:
/**
*   @param I  		current index
*    @param buff 	buffer of stored arguments
*  @param  out   next unused index
*/
	int yourCallBack(int i,char *buff[]);
### Register an Argument
To register an callback function:
argvParser parser("your program description"); // program description
    parser.addArg("shortARG", "longARG","description of argument",callBackD,false);


### Check all Arguments
To check all Arguments use :
parser.analyseArgv(args,argv);
it returns true if all required arguments or any unknown argument had been used
