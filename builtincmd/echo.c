#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../src/io.h"

void Echo(char** argv) {
  int valid_argv = 0;
  for (int i = 1; argv[i] != NULL; i++) {
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    valid_argv++;
  }
  for (int i = 1; argv[i] != NULL; i++) {
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    printf("%s", argv[i]);
    if(--valid_argv)putchar(' ');
  }
  putchar('\n');
  Flush();
}