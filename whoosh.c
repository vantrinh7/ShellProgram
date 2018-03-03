#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int whoosh_exit(char **args);
int whoosh_pwd(char **args);
int whoosh_cd(char **args);
char *builtin_str[] = {"exit","pwd", "cd"};
int (*builtin_func[]) (char **) = { &whoosh_exit, &whoosh_pwd, &whoosh_cd};



int whoosh_exit(char **args) {
  return 0;
}

int whoosh_pwd(char **args) {


}
int whoosh_cd(char **args)
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





void whoosh_loop() {
char* input;
char* get_args[];
int status;

do {
  fprintf("whoosh>"); // print prompt
  input = whoosh_read_input(); // a function call to reads the input
  get_args = whoosh_parse_input(); //  a function call to split the input into arguements
  status = whoosh_init(get_args); // execute those arugments

  free(input);
  free(get_args);
}while(status);

}

char* whoosh_read_input() {
  int input_allocation = 127284;
  char* arr = (char*) malloc(sizeof(char) input_allocation);
  int index = 0;
  int curr_char;


  if(arr == NULL) {
    fprintf(stderr,"malloc failed\n" );
  }

  while(1){
    curr_char = getchar();

    if( curr_char == '\n' || curr_char == EOF){ // if it’s the newline, or EOF, null terminate our current string and return it
      iarr[index] = '0';
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
char** whoosh_parse_input(char* line){

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
      arr = realloc(arr, input_allocation);
      if(tokens == NULL) {
        fprintf(stderr,"malloc failed\n" );
      }
    }
      token = strtok(NULL, " ");

  }

    tokens[index] = NULL;
    return tokens;
  }


}












}












int main(int argc, char* argv[]) {

  whoosh_loop();
  return 0;

}
