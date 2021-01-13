#include <errno.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>

void Remove(char *path) {
  char * cwd = (char *)malloc(256);
  getcwd(cwd, 128);
  int len = strlen(cwd);
  if(cwd[len - 1] != '/')cwd[len++] = '/';
  cwd[len] = '\0';
  strcat(cwd, path);
  DIR * dir = opendir(cwd);
  if(dir != NULL){
    closedir(dir);
    fprintf(stderr, "rm: %s is a directory\n", path);
    return;
  }
  if (remove(cwd) == -1) fprintf(stderr, "rm: %s\n", strerror(errno));// s link
  free(cwd);
  cwd = NULL;
}

int main(int argc, char **argv) {
  if(argc < 2){
    fprintf(stderr, "rm: lack of arguments\n");
    return 0;
  }
  for (int i = 1; i < argc && argv[i]; i++) Remove(argv[i]);
  return 0;
}