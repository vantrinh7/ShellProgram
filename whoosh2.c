#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

void loop();
char* readInput();
char** parseInput(char* line);
int wExit(char *args);
int wPwd(char *args);
int wCd(char *args[]);
void printPath();
void setPath(char* args);
void runFile(char** args);
int isFileExisting(char* path);
void reportError();

#define MAX_LINE_LEN 128

//char *builtinStr[] = {"exit","pwd", "cd", "printpath", "setpath"};
//int (*builtinFunc[]) (char **) = { &wExit, &wPwd, &wCd, &printPath, &setPath};

char* path = "/bin";

int main (int argc, char *argv[]) {
  //loop();
  
  setPath("/usr/games");
  printPath();

  char* arr[] = {"/usr/games/gnome-sudoku", NULL};
  runFile(arr);

  return 0;
}

/**
 * Method to print out the path (/bin is default)
 * Makes a copy of path so as not to modify path variable
 *
 **/
void printPath() {
  // Allocate memory for a copy of path
  // and report error during string copy
  char* thePath;
  thePath = malloc(sizeof(char *)*strlen(path)); 
  char* dest1 = strcpy(thePath, path); 
  if (dest1 == NULL) {
    reportError();
    exit(1);
  }
  
  // Concatenate path copy with end line character
  // and print out. Report error when concatenate
  char* dest2 = strcat(thePath, "\n");
  if (dest2 != NULL) {
    printf("%s", thePath);     
  } else {
    reportError();
    exit(1);
  } 
  // Free the variable
  free(thePath);
}

void setPath(char* args) {
  if (args != NULL) {
    path = args;
  } else {
    reportError();
    exit(1);
  }
}

int isFileExisting(char* pathToFile) {
  struct stat buffer;
  if (stat(pathToFile, &buffer) == 0) {
    return 0;
  } else if (stat(pathToFile, &buffer) == -1) {
    return -1;
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
  // If pid is 0, this is the child, run the executable
  else if (pid == 0) {
    if (isFileExisting(args[0]) == 0) {
      if (execv(args[0], args) == -1) {
	perror("execve() ERROR ");
      }
    }
    else {
      fprintf(stderr, "Error: File does not exist\n");
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

int wExit(char *args) {
  return 0;
}

int wPwd(char *args) {
  
  return 0;
}

int wCd(char *args[])
{
  if (args[1] == NULL) {
    fprintf(stderr, "whoosh: expected argument to \"cd\"\n");
  }
  else {
    if (chdir(args[1]) != 0) {
      // Comment out as an example of merge conflict
      // perror("whoosh");
    }
  }
  return 1;
}


void loop() {
  //char* input;
  //char* get_args[];
  int status = 0;

  do {
    printf("whoosh> "); // print prompt
    //input = readInput(); // a function call to reads the input
    //get_args = parseInput(); //  a function call to split the input into arguements
    //status = whoosh_init(get_args); // execute those arugments

    //free(input);
    //free(get_args);
  }while(status);

}

char* readInput() {
  int input_allocation = 127284;
  char* arr = (char*) malloc(sizeof(char)*input_allocation);
  int index = 0;
  int curr_char;


  if(arr == NULL) {
    fprintf(stderr,"malloc failed\n" );
  }

  while(1){
    curr_char = getchar();

    if( curr_char == '\n' || curr_char == EOF){ // if it’s the newline, or EOF, null terminate our current string and return it
      arr[index] = '0';
      return arr;
    }
    else { // otherwise, add the character to our existing string
      arr[index] = curr_char;
    }
    index++;

    if(index >= input_allocation) { // if outbounds, reallocate
      arr = realloc(arr, input_allocation);
      if(arr == NULL) {
        fprintf(stderr, "malloc failed\n" );
      }
    }
  }
}

char** parseInput(char* line){
  int input_allocation = 64;
  char ** tokens = malloc(sizeof(char*) * input_allocation);
  char* token;
  int index = 0;

  if(tokens == NULL) {
    fprintf(stderr, "malloc failed\n" );
  }

  token = strtok(line, " " );
  while(token != NULL) {
    tokens[index] = token;
    index++;

    if(index >= input_allocation) { // if outbounds, reallocate
      // arr = realloc(arr, input_allocation);
      if(tokens == NULL) {
        fprintf(stderr,"malloc failed\n" );
      }
    }
    token = strtok(NULL, " ");
  }
  tokens[index] = NULL;
  return tokens;
}

void reportError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}



