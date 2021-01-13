#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "../src/io.h"

void Cd(char** argv) {
  if (argv[2]) {
    fprintf(stderr, "sh: cd: too many arguments");
    return;
  }
  if (chdir(argv[1]) == -1) Print(strerror(errno), stderr);
}