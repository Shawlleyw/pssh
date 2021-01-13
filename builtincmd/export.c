#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char **environ;

int SetEnv(char * env){
  int i, flag = 0;
  char* s = env;
  for (i = 0; s[i]; i++)
    if (s[i] == '=') {
      s[i++] = '\0';
      flag = 1;
      break;
    }
  if(!flag){

  }
  if (setenv(env, env + i, 1) == -1){
    fprintf(stderr, "sh: export: %s is not a valid argument\n", env);
    return 0;
  }
  return 1;
}

void Export(char** argv) {
  if(argv[1] == NULL){
    for(int i = 0; environ[i]; i++)
      printf("declare -x %s\n", environ[i]);
      return;
  }
  for(int i = 1; argv[i]; i++)
    SetEnv(argv[i]);
}