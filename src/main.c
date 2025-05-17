#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  while(1){
    printf("$ ");

    // Wait for user input
    char input[100];

    if(fgets(input, 100, stdin) != NULL){
      //process input, remove new line if found
      int len = strlen(input);
      if(len > 0 && input[len - 1] == '\n'){
        input[len - 1] = '\0';
      }
      printf("%s: command not found\n", input);
    } else {
      printf("Input error\n");
      return 1;
    }
  }
  return 0;
}
