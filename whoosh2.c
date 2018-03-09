#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

void printPrompt();
char* readInput();
char** parseInput(char* line);
int execCommands(char** args);
int wExit(char* args[]);
int wPwd(char* args[]);
int wCd(char* args[]);
int printPath();
void setPath(char* args);
void runFile(char** args);
void reportError();
#define MAX_LINE_LEN 128
char* path = "/bin";
char* commands_str[] = { "exit","pwd","cd", "printpath", "setpath"};
int (*command_func[]) (char**) = {&wExit, &wCd, &wPwd, &printPath, &setPath};


void printPrompt() {
  char* input;
  char** inputArray;
  int isContinuing = 1;

  while (isContinuing) {
    printf("whoosh> "); // print prompt
    input = readInput(); // a function call to reads the input
    inputArray = parseInput(input); //  a function call to split the input into arguements
    isContinuing = execCommands(inputArray); // execute those arugments
    free(input);
    free(inputArray);
  }
}

int num_built_commands() {
  return sizeof(commands_str)/ sizeof(char*);
}


int execCommands(char* args[]) {
  //char *commands[] = {"exit","pwd", "cd", "printpath", "setpath"};
  if( args[0] == NULL) {
    return 1;
  }
  int i;
  for(i = 0; i < num_built_commands(); i++) {
    if( strcmp(args[0], commands_str[i]) == 0) {
      return (*command_func[i])(args);
    }
  }
  runFile(args);
}

char* readInput() {
  char *line = NULL;
  size_t lineLen = 0;

  // Read line from input stream
  ssize_t readResult;
  if (stdin != NULL) {
  readResult = getline(&line, &lineLen, stdin);
  } else {
    fprintf(stderr, "Error: stdin is null\n");
    exit(1);
  }
  // Report error if reading result is -1
  if ((readResult == -1)) {
    fprintf(stderr, "Error: cannot read line\n");
    exit(1);
  } else if (readResult > MAX_LINE_LEN) {
    fprintf(stderr, "Error: Line too long - Continue processing\n");
  }
  return line;
}

char** parseInput(char* line){
  int arraySize = MAX_LINE_LEN;
  char ** tokens = calloc(arraySize, sizeof(char *));
  char* delim = " \n\t"; //Separate based on space, end line and tab characters
  int index = 0;

  if(tokens == NULL) {
    fprintf(stderr, "ERROR: Malloc failed\n" );
  }
  // On first call, strtok returns pointer to
  // the firt part of line separated by delimiter
  tokens[0] = strtok(line, delim);

  // On subsequent calls, strtok takes NULL as value to string
  // While previous token is not null, keep chopping the line
  //while (index < arraySize) {
    while(tokens[index] != NULL) {
      index++;
      tokens[index] = strtok(NULL, delim);

      /* if(index >= arraySize) { // if outbounds, reallocate */
      /* 	arraySize = arraySize + MAX_LINE_LEN; */
      /* 	tokens = realloc(tokens, sizeof(char *) * arraySize); */
      /* 	if(tokens == NULL) { */
      /* 	  fprintf(stderr,"ERROR: malloc failed\n" ); */
      /* 	} */
      /* }  */
    }
    //}

  // Give last token a null value
  tokens[index] = NULL;
  return tokens;
}

/**
 * Method to print out the path (/bin is default)
 * Makes a copy of path so as not to modify path variable
 *
 **/
int printPath() {
  // Allocate memory for a copy of path
  // and report error during string copy
  char* thePath;
  thePath = malloc(sizeof(char *)*strlen(path));
  char* dest1 = strcpy(thePath, path);
  if (dest1 == NULL) {
    reportError();
    exit(1);
    return 0;
  }

  // Concatenate path copy with end line character
  // and print out. Report error when concatenate
  char* dest2 = strcat(thePath, "\n");
  if (dest2 != NULL) {
    printf("%s", thePath);
  } else {
    reportError();
    exit(1);
    return 0;
  }
  // Free the variable
  free(thePath);
  return 1;
}

void setPath(char* args) {
  if (args != NULL) {
    path = args;
  } else {
    reportError();
    exit(1);
  }
}

void runFile(char** args) {
  pid_t pid;

  pid = fork();
  // If pid is negative, report error
  if (pid < 0) {
    fprintf(stderr, "Can't fork a process\n");

  }
  // If pid is 0, this is the child
  else if (pid == 0) {

    // Check if file exists using stat
    struct stat buffer;
    if (stat(args[0], &buffer) == 0) {

      // Run the executable
      // If file cannot be executed, report error and continue processing
      if (execv(args[0], args) == -1) {
	perror("Report error and continue processing ");
      }
    }
    // If file doesn't exist, report error and continue processing
    else {
      fprintf(stderr, "Report error and continue processing\n");
    }
  }
  // If pid is positive, this is the parent
  // Wait for child to complete
  else {
    pid_t waited;
    int status;

    waited = wait(&status);
    if (waited == -1) {
      perror("wait() ERROR ");
    }
    else {
      // print its exit code
      fprintf(stdout,"I am the parent. My child exited with code %d\n",
	      WEXITSTATUS(status));
    }
  }
}

int wExit(char* args[]) {
  if(strcmp(args[0], "exit") == 0) {
    exit(0);
  }
}

int wPwd(char *args[]) {
  if( strcmp (args[0], "pwd") == 0) {
    char buff[PATH_MAX + 1];
    char* cwd;

    cwd = getcwd(buff, PATH_MAX + 1);
    if(cwd != NULL) {
      printf("%s\n",cwd);
    } else{
      reportError();
    }
      return 0;
  }
  return 1;

}

int wCd(char *args[]){
 //  if (strcmp(args[0], "cd") == 0) {
 //    char* dir;
 //    if( num_args == 1){
 //      dir = getenv("HOME");
 //      if(chdir(dir) != 0) {
 //        reportError();
 //      }
 //    }
 //  else {
 //    dir = args[1];
 //    if (chdir(dir) != 0){
 //      reportError();
 //    }
 //  }
 //  return 0;
 // }
 // return 1;
 if (args[1] == NULL) {
   fprintf(stderr, "whoosh: expected argument to \"cd\"\n");
 }
 else {
   if (chdir(args[1]) != 0) {
     // Comment out as an example of merge conflict
    perror("whoosh");
   }   
 }
 return 1;
}

void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char**argv) {
printPrompt();

return 0;


}
