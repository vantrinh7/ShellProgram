# Shell Program

This program functions as a shell - a command-line interface for access to services provided by the operating system. It runs inside the Terminal application on Mac and Linux. 

This shell is an interactive loop. It repeatedly prints a prompt "whoosh> ", detects an user input, parses the input, executes the command specified, and waits for the command to finish. 

The program has the following functions:

### `cd`
This is a *change directory* command. When user enters `cd` followed by a path, the shell changes the working directory to the specified path. When user enters `cd` without arguments, the shell changes to the path stored in the $HOME environment variable.

<br>

### `pwd`
This is a *print working directory* command. When user types `pwd`, the shell prints out the current directory.


### `exit`
This command causes the shell program to terminate.


### `printpath`
This command prints out the path variable. It is a variable that contains a list of directories that user gives the shell. For example, a possible path variable is `/usr/games /bin`. This means user has given shell two directories: `/usr/games` and `/bin`. When the program starts, the default path is `/bin`.


### `setpath`
This command changes the path variable to a given value. For example, `setpath /usr/games` changes path variable to `/usr/games`. If user provides no value after `setpath`, the path variable will be set to null.


### Execute external programs
When user enters a command that is not one of the above, the shell searches the paths stored in path variable for a file with that name. If the shell finds a matching file, it will fork a process to run the program in, execute the program,
and wait for that program to complete before printing another prompt. If not, it gives an error message. As long as a correct path is given, the shell can run any program, ranging from simple clock program to more complicated gaming applications. 


### Defensive error checking  
The shell was programmed defensively. When there is an error, the shell gives the message `An error has occured`. In most cases, it does not core dump, hang indefinitely or prematurely terminate. An important note: the shell assumes that the maximum length of a line of input is 128 bytes.
<br>
