# CPP Argument Parser
Arguments are separated with (at least one) spaces.
If not defined the argument parser allows none additional characters!

e.g. ` -s /opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.0.1/Servers/ST-LINK_gdbserver/ST-LINK_gdbserver -c arm-none-eabi-gdb -elf /home/user/git/EmbeddedTesting/EmbeddedTesting/Debug/EmbeddedTesting.elf -p 61234 `
## Elf-File location (required)
location of the elf file which is flashed to the target.
This file is required because the gdb-client reads e.g function names from this file.

If the target is not flashed jet : the application is able to flash the target before testing.
To flash the target use additional "-f "

`-elf <location>` path to file

## Client command (required)
The Application requires an GDB-Client child process to communicate with the Target.
it is not necessary(and possible) to give additional gbb-client arguments. If additional arguments are required must e.g. an script gets created to call the gdb-client

`-c <command to call>`or `--client <command to call>` command to call the gdb-client

## GDB-Port
To adjust the communication port from the gdb-Server to the gdb-client

`-p <port>` or `--port <port>`

## Server command
If the application shall start an gdb-server on its own use this command with an location to an script to start the gdb-Server.

`-s <command>` or `--server <command>`

## Define Test-Level
Adjust the amount of information about the executed test.
The more information are available the (significant) slower is the test execution.

`-t <level>` or `--testLevel <level>`

where level is:
* "justSuccess" : boolean information about the test-status
* "testName" : (default) + name and group of the executed test
* "testResults" + result and expected value of an failed test
* "lineNumber" + number of the failed test

## Flash target
Flash the elf file before execute tests.

`-f` or `--flash`

## Generate XML-Output-File
Generate an XML-Output-file with each tests and if given the result and line information.

`-x <filename>` or `--xml <filename>`

##logfile
This application generates log files with the simple-Logging-library github.com/diridari/SimpleLogging
It is able to log to the cli and to an logfile.
### Log-Level
The loglevel can be adjusted by:

`-l <level>` or`--logLevel <level>` to set the loglevel for file and cli rquals
`-lls <level1> <level2>` or `--logLevelSeparate<level1> <level2>` to adjust the level seperate level1 = cli , level2 =

Level can be:
* None = -1, 	        // Noting  just for tests
* UserInfo = 0,	    // Information for Users
* CriticError = 1,
* Error = 2,
* Message = 3,
* Info = 4,
* Debug = 5,
* DebugL2 = 6,
* DebugL3 = 7

### Log-filename
The name of the logfile is "log.log". This can be clanged by:

`-lfn <name>` or `--logFileName <name>`


