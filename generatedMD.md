

# Extendend Example
## Required arguments:
 * --foo
## Application Arguments
#### --test
test argument

It is possible to add addition information for one command. Now the user can ype <-h -t> and see this additional informa
tion

    short: -t
    long : --test

#### --foo
foo test argument  required argument example

prints the next element to the cli

    short: -f
    long : --foo

number of additional parameter: 1

 This argument is requires
#### --print
echo text

echo the next argument

    short: -p
    long : --print

number of additional parameter: 1

 #### --enums
enum example

here is just a pre defined set of arguments allowed

    short: -e
    long : --enums

number of additional parameter: 1

 allowed parameter:  abc def xyz
#### --open
example to complete a file/dir



    short: -o
    long : --open

number of additional parameter: 1

 #### --noHighlight
disabledCli highlighting



    short: -nh
    long : --noHighlight

### logging
#### --logging
enable logging



    short: -l
    long : --logging

#### --logFile
generate logfile



    short: -logf
    long : --logFile
