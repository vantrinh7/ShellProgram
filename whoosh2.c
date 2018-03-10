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
int wExit(char** args);
int wPwd(char* args[]);
int wCd(char* args[]);
int printPath();
int setPath(char** args);
int runFile(char** args);
int runExecutable(char* thePath, char** args);
void reportError();
#define MAX_LINE_LEN 128

char** path;
int pathArrSize;

int main (int argc, char *argv[]) {
  /* char* arr[] = {"/usr/games/gnome-sudoku", NULL}; */
  /* runFile(arr); */

  /* char* input = readInput(); */
  /* char** inputArray = parseInput(input); */
  /* /\* execCommands(inputArray); *\/ */

  /* setPath(inputArray); */

  /* char* arr[] = {"/bin", "/usr/games", "/random", "f", "g"}; */
  /* pathArrSize = sizeof(arr)/sizeof(char*); */
  /* path = calloc(6, sizeof(char *)); // Add 1 Then -1 because throw out the first argument */  
  /* for (int i = 0; i < sizeof(arr)/sizeof(char *); i++) { */
  /*   path[i] = arr[i]; */
  /* } */
  /* setPath(arr); */
  
  /* printPath(); */
  
  /* free(input); */
  /* free(inputArray); */
  /* free(path); */
  
  printPrompt(); 
  return 0;
}


void printPrompt() {
  char* input;
  char** inputArray;
  int isContinuing = 1;

  while (isContinuing) {
    printf("whoosh> "); // print prompt
    input = readInput(); // a function call to reads the input
    inputArray = parseInput(input); //  a function call to split the input into arguements
    isContinuing = execCommands(inputArray); // execute those arugments
    /* int i = 0; */
    /* while (i < 5) { */
    /*   if (inputArray[i] != NULL) { */
    /* 	printf("Input %d is: %s\n", i, inputArray[i]); */
    /*   } else { */
    /* 	printf("Input %d is: %s\n", i, "null"); */
    /*   } */
    /*   i++; */
    /* } */
    
    // Free input
    free(input);
    free(inputArray);    
  }
  if (path != NULL) {
    // Free each element in path array
    for (char** arr = path; *arr; arr++) {
      free(*arr);
    }
    // Free the path array
    free(path);
  }
}

int execCommands(char** args) {
  
  char *commands[] = {"exit","pwd", "cd", "printpath", "setpath"};
  for (int i = 0; i < sizeof(commands)/sizeof(char *); i++) {
    if (strcmp(args[0], commands[i]) == 0) {
      switch(i) {
      case 0: // exit command
	return wExit(args);	
	break;
      case 1: // pwd command
	break;
      case 2: // cd command
	break;
      case 3: // printPath command
	return printPath();
	break;
      case 4: // setPath command
	return setPath(args);	
	break;
      }
    }
  }
  // If command is not one of the built-ins
  // Check to see if it's an external program and run it
  
  return runFile(args);
}

int runFile(char** args) {
  int isFileRun = 0;
  
  // If path is null, give it default value
  if (path == NULL) {
    path = calloc(2, sizeof(char *));
    pathArrSize = 1;
    path[0] = malloc(sizeof(char*));
    char* des1 = strcpy(path[0],"/bin");
    if (des1 == NULL) {
      reportError();
    }    
  }
  // Look into each element in path array
  for (int i = 0; i < pathArrSize; i++) {
 
    // Concatenate user command and path element into a variable
    char* thePath = malloc(sizeof(char)*MAX_LINE_LEN);
    strcpy(thePath, path[i]);
    strcat(thePath, "/");
    strcat(thePath, args[0]);

    // Check if file exists using the path variable
    // If it does, run the file. If not, keep looking
    struct stat buffer;
    if (stat(thePath, &buffer) == 0) {
      isFileRun = runExecutable(thePath, args);
    }    
    free(thePath);    
  }
  // If no file is found, report error
  if (isFileRun == 0){
    reportError();
  }
  
  return 1;
}

int runExecutable(char* thePath, char** args) {
  pid_t pid;
  int isFileRun = 1;

  pid = fork();
  // If pid is negative, report error
  if (pid < 0) {
    reportError();
  }
  // If pid is 0, this is the child
  else if (pid == 0) {
   
    // Run the executable
    // If file cannot be executed, report error and continue processing
    if (execv(thePath, args) == -1) {
      reportError();
      isFileRun = 0;
    }
  }
  // If pid is positive, this is the parent
  // Wait for child to complete
  else {
    pid_t waited;
    int status;

    waited = wait(&status);
    if (waited == -1) {
      reportError();
    }
  }
  return isFileRun;
}

int setPath(char** args) {
  char *des1;
  
  // If arguments are not null
  if (args[1] != NULL){
    
    // If path is not null
    if (path != NULL) {
      // Free the memory previously allocated for path
      for (char** arr = path; *arr; arr++) {
	free(*arr);
      }
      free(path);
    }
    
    // Allocate new memory for path (add 1 to size for later function)
    path = calloc(pathArrSize + 1, sizeof(char*)); 
    for (int i = 0; i < pathArrSize; i++) {

      // Allocate memory for each element in path
      // Copy from args into path, ignore the arg[0]
      if (args[i + 1] != NULL) {
	path[i] = malloc(sizeof(char*)*MAX_LINE_LEN);
	des1 = strcpy(path[i], args[i + 1]);
	if (des1 == NULL) {
	  reportError();
	}   
      }
    }   
  } else {
    // If arguments are null, report error
    reportError();
    return 0;
  }
  
  return 1;
}

/**
 * Method to print out the path (/bin is default)
 * Makes a copy of path so as not to modify path variable
 *
 **/
int printPath() {
  char *des1, *des2;
  
  // If path is null, give it default value
  if (path == NULL) {
    path = calloc(2, sizeof(char *));
    path[0] = malloc(sizeof(char*));
    des1 = strcpy(path[0],"/bin");
      if (des1 == NULL) {
	reportError();
    }    
  }
  
  // Allocate memory for a char variable that will be printed out
  char* thePath =  malloc(sizeof(char)*MAX_LINE_LEN);

  // Copy the first element of path array into the variable
  des1 = strcpy(thePath, path[0]);
  if (des1 == NULL) {
    reportError();
  }
 
  // Concatenate the rest 
  for (int i = 0; i < pathArrSize; i++) {
    // If next element is not null, concatenate 
    if (path[i + 1] != NULL) {
      des1 = strcat(thePath, " ");
      des2 = strcat(thePath, path[i + 1]);
      if ((des1 == NULL) || (des2 == NULL)) {
	reportError();
      }
    }
  }
  // Then concatenate end line character    
  des1 = strcat(thePath, "\n");
  if (des1 == NULL) {
    reportError();
  }
  // Print out path
  printf("%s", thePath);
  
  // Free the variable
  free(thePath);
  return 1;
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
  char** tokens = calloc(arraySize, sizeof(char *));
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

  // Save latest number of array size to be used for setpath
  if (strcmp(tokens[0], "setpath") == 0) {
    pathArrSize = index - 1; // Subtract first argument
  }
  return tokens;
}

int wExit(char** args) {
  /* if( strcmp (args[0], "exit") == 0) { */
  /*   exit(1); */
  /* } */
  return 0;
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

