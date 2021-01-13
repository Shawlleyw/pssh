#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../src/io.h"

int trans(char* str) {
  int x = 0, len = strlen(str);
  for (int i = 0; i < len; i++) x = x * 10 + str[i] - '0';
  return x;
}

int Kill(char** argv) {
  int pid = trans(argv[1]), sig = trans(argv[2]);
  if (kill(pid, SIGABRT) == -1) {
    Print(strerror(errno), stderr);
    return 0;
  }
  return 1;
}