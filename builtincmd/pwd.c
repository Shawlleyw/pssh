#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../src/io.h"

void Pwd(char **argv) {
  char *path = (char *)malloc(1024);
  if (getcwd(path, 1024) == NULL) {
    fprintf(stderr, "sh: pwd: %s", strerror(errno));
    return;
  }
  printf("%s\n", path);
  free(path);
  path = NULL;
  fflush(stdout);
}