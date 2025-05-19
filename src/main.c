#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 10

struct InputStruct{
  char command[100];
  char *arg[MAX_ARGS];
  int arg_counts;
};

void handleExitCommand(struct InputStruct* input_args ){
  exit(0);
};

struct InputStruct parse_input(char* original_input){
  struct InputStruct result;
  result.command[0] = '\0';
  result.arg[0] = NULL;

  char *tkn;
  int token_index = 0;

  tkn = strtok(original_input," ");
  while (tkn!= NULL){
    //debug
    // printf("Token: %s\n",tkn);

    if(token_index == 0){
      // This would be the first token, will be treated as command
      strncpy(result.command,tkn,sizeof(result.command)-1);
      // Ensure null terminator
      result.command[sizeof(result.command) - 1] = '\0';
      result.arg_counts = 0;
    } else {
      if (result.arg_counts < MAX_ARGS){
        result.arg[result.arg_counts] = tkn;
        result.arg_counts++;
      } else {
        printf("Too many arguments");
        break;
      }
    }
    token_index++;
    tkn = strtok(NULL," ");
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  while(1){
    printf("$ ");

    // Wait for user input
    // struct InputStruct input;
    char input[100];

    if(fgets(input, 100, stdin) != NULL){
      //process input, remove new line if found
      int len = strlen(input);
      if(len > 0 && input[len - 1] == '\n'){
        input[len - 1] = '\0';
      }

      struct InputStruct parsed = parse_input(input);
      if(parsed.command[0] != '\0'){
        if(strcmp(parsed.command,"exit") == 0){
          handleExitCommand(&parsed);
        } else {
          printf("%s: command not found\n",parsed.command);
        }
      }
    } else {
      printf("Input error\n");
      return 1;
    }
  }
  return 0;
}
