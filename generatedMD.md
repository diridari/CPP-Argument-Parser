O:\Dokumente\Git\CPP-Argument-Parser\cmake-build-debug\extendedExmple.exe
# test app
## Required arguments:
 * --foo
## Application Arguments
### --test
test argument


    short: -t
    long : --test

### --foo
foo test argument  required argument example
prints the next element to the cli

    short: -f
    long : --foo

number of additional parameter: 1

 ### --print
echo text
echo the next argument

    short: -p
    long : --print

number of additional parameter: 1

 ### --enums
enum example
here is just a pre defined set of arguments allowed

    short: -e
    long : --enums

number of additional parameter: 1

 allowed parameter:  abc def xyz
### --open
example to complete a file/dir


    short: -o
    long : --open

number of additional parameter: 1

 ### --noHighlight
disabledCli highlighting


    short: -nh
    long : --noHighlight

### --logging
enable logging


    short: -l
    long : --logging

### --logFile
generate logfile


    short: -logf
    long : --logFile


Process finished with exit code 0
