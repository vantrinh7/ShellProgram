# Shell Program

TO-DO list:

Sabrina:
- Starting up
- Parsing the command line
- exit
- pwd
- cd

Van:
- setpath
- printpath
- Running executable programs
- ERROR CHECKING
- Miscellaneous

Both:
- Correct memory management
- Comments
- Coding style

This program functions as a shell - a command-line interface for access to the operating system's services. It runs inside the Terminal application on Mac and Linux. 

This shell is an interactive loop. It repeatedly prints a prompt "whoosh> ", detects user input, parses the input, executes the command specified, and waits for the command to finish. The shell has the following functions:

### `cd`
This is a *change directory* command. When user enters `cd` followed by a path, the shell changes the working directory to the specified path. When user enters `cd` without arguments, the shell changes to the path stored in the $HOME environment variable.

### `pwd`
This is a *print working directory* command. When user types `pwd`, the shell prints out the current directory.

### `exit`
This command causes the shell program to terminate.

### `printpath`
This command prints out the path variable. It is a variable that contains a list of directories that user gives the shell. For example, a possible path variable is `/usr/games /bin`. This means user has given shell two directories: `/usr/games` and `/bin`. In this shell, the default path is `/bin`.

### `setpath`
This command changes the path variable to a given value. For example, `setpath /usr/games` changes path variable to `/usr/games`. If user provides no value after `setpath`, the path variable will be set to null.

### Execute external program
When user enters a command that is not one of the above, the shell searches the paths stored in path variable for a file with that name. If it finds a matching file, it will fork a process to run the program in, execute the program,
and wait for it to complete before printing another prompt. If not, it gives an error message. As long as a correct path is given, the shell can run any program, ranging from clock application to games programs. 

### Defensive error checking  
The shell was programmed defensively. In certain situations, the shell give the message `An error has occured`. In most situations, the shell does not core dump, hang indefinitely or prematurely terminate. An important note: this program assumes that the maximum length of a line of input to the shell is 128 bytes.
