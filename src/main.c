#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 10
#define MAX_PATH_LEN 256
struct InputStruct{
  char command[100];
  char *args[MAX_ARGS];
  int arg_counts;
};

const char *builtin_command[] = {"exit","echo","type","a"};

void handleExitCommand(struct InputStruct* input_args ){
  exit(0);
};

void handleEchoCommand(struct InputStruct* input_args){
  for(int i = 0; i < input_args->arg_counts; i ++){
    printf("%s ",input_args->args[i]);
    if(i == input_args->arg_counts - 1){
      printf("\n");
    }
  }
}

void handleTypeCommand(struct InputStruct* input_args){
  // Handle builtin command
 for(int i = 0; i < (sizeof(builtin_command)/(sizeof(char*))); i++){
  if(strcmp(input_args->args[0],builtin_command[i]) == 0){
    printf("%s is a shell builtin\n",input_args->args[0]);
    return;
  }
  // Handle PATH
  const char *path_env_variable = getenv("PATH");

  if(path_env_variable == NULL){
    printf("Path env not found");
  } else {

    const *path_copy = strdup(path_env_variable);
    if(path_copy == NULL){
      printf("Failed to allocate memory for PATH copy");
      return;
    }
    int found_path = 0;
    char *dir = strtok(path_copy,":");
    while(dir != NULL){
      char full_path[MAX_PATH_LEN];
      int path_length = snprintf(full_path,sizeof(full_path),dir,input_args->args[0]);

      if(path_length < 0 || path_length >= sizeof(full_path)){
        // error in formating or path too long for buffer
        fprintf(stderr,"Error creating full path or path too long.\n");
      }
      if(access(full_path,X_OK) == 0){
        printf("%s is %s",input_args->args[0],full_path);
        found_path = 1;
        break;
      }
      dir = strtok(NULL,":");
    }
    free(path_copy);
    if(!found_path){
      printf("%s: not found\n",input_args->args[0]);
    }
  }

 }
 printf("%s: not found\n",input_args->args[0]);
}

struct InputStruct parse_input(char* original_input){
  struct InputStruct result;
  result.command[0] = '\0';
  result.args[0] = NULL;

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
        result.args[result.arg_counts] = tkn;
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
        } else if(strcmp(parsed.command, "echo") == 0){
          handleEchoCommand(&parsed);
        } else if(strcmp(parsed.command,"type") == 0){
          handleTypeCommand(&parsed);
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
