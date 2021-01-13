#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>
#include <sys/stat.h>

void Touch(char* path) {
  char* name = (char*)malloc(256);
  strcpy(name, path);
  FILE* fp = fopen(name, "a+");
  if (fp == NULL)
    fprintf(stderr, "touch: %s\n", strerror(errno));
  else
    fclose(fp);
  free(name);
  name = NULL;
}

void Work(char * path){
  DIR * dir = opendir(path);
  if(dir == NULL) Touch(path);
  else closedir(dir);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "touch: lack of arguments\n");
    return 0;
  }
  for (int i = 1; i < argc && argv[i]; i++) Work(argv[i]);
  return 0;
}