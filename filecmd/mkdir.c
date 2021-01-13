#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void Mkdir(char* path) {
  char* name = (char*)malloc(256);
  strcpy(name, path);
  if (mkdir(name, 0755) == -1) fprintf(stderr, "%s\n", strerror(errno));
  free(name);
  name = NULL;
}

int main(int argc, char** argv) {
  if(argc < 2){
    fprintf(stderr, "mkdir: lack of arguments\n");
    return 0;
  }
  for (int i = 1; i < argc && argv[i]; i++) Mkdir(argv[i]);
  return 0;
}