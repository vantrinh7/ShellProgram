#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

void loop();
char* readInput();
char** parseInput(char* line);
int wExit(char *args);
int wPwd(char *args);
int wCd(char *args[]);
int printPath();
int setPath(char *args);
void reportError();

#define MAX_LINE_LEN 128

//char *builtinStr[] = {"exit","pwd", "cd", "printpath", "setpath"};
//int (*builtinFunc[]) (char **) = { &wExit, &wPwd, &wCd, &printPath, &setPath};

char* path = "/bin\n";

int main (int argc, char *argv[]) {
  //loop();
  printPath();
  if (argv[1] != NULL) {
  setPath(argv[1]);
  printPath();
  }  
  return 0;
}

int printPath() {
  if (path != NULL) {
    printf("%s", path);
    return 0;
  } else {
    reportError();
  }
  return -1;
}

int setPath(char *args) {  
  // Concatenate path given with end line character
  char* dest = strcat(args, "\n");
  if (dest != NULL) {
    path = args;
  } else {
    reportError();
  }
  return 0;
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



