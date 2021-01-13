#include<stdio.h>
#include<stdlib.h>

#include "testgetline.h"

const int BufSizeMain = 5;

int main() {
  int break_opt = 0;
  //else printf("Init complete\n");
  while (1) {
    if (break_opt) break;
    char **argv = (char **)malloc(BufSizeMain * sizeof(char *));
    if (argv == NULL) {
      fprintf(stderr, "sh: Memory allocation error !");
      continue;
    }
    argv = GetArgv(argv);
    if (argv == NULL) {
      break_opt = 1;
      continue;
    }
    free(argv);
    argv = NULL;
  }
  return 0;
}