#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int wExit(char **args);
int wPwd(char **args);
int wCd(char **args);
void loop();
char* readInput();
char** parseInput(char* line);

char *builtinStr[] = {"exit","pwd", "cd"};
int (*builtinFunc[]) (char **) = { &wExit, &wPwd, &wCd};

int main (int argc, char *argv[]) {
  loop();
  return 0;
}

int wExit(char **args) {
  return 0;
}

int wPwd(char **args) {

  
  return 0;
}

int wCd(char **args)
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




