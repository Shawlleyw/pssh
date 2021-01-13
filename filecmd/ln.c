#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *GetLast(char *str) {
  char *ret = str;
  for (char *next = strtok(str, "/"); next != NULL; next = strtok(NULL, "/"))
    ret = next;
  return ret;
}

void Link(char *src, char *path, int flag) {
  if (flag) {
    if (symlink(src, path) == -1) fprintf(stderr, "ln: %s\n", strerror(errno));
  } else {
    if (link(src, path) == -1) fprintf(stderr, "ln: %s\n", strerror(errno));
  }
}

void Work(int argc, char **argv, int flag) {
  char path[1024];
  char file_name[1024];
  strcpy(file_name, argv[1]);
  if (argc == 2) {
    strcpy(path, "./");
    strcpy(path, GetLast(file_name));
    Link(argv[1], path, flag);
    return;
  }
  strcpy(path, argv[argc - 1]);
  DIR *dir = opendir(path);
  if (dir == NULL) {
    Link(argv[1], argv[2], flag);
    return;
  }
  char temp[1024];
  for (int i = 1; i < argc - 1; i++) {
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    memset(path, 0, sizeof(path));
    strcpy(path, argv[argc - 1]);
    int len = strlen(path);
    if(path[len - 1] != '/')path[len++] = '/';
    strcpy(temp, argv[i]);
    strcat(path, GetLast(temp));
    //printf("src : %s dest : %s\n", argv[i], path);
    Link(argv[i], path, flag);
  }
  closedir(dir);
}

int main(int argc, char **argv) {
  int flag = 0;
  for (int i = 1; i < argc && argv[i]; i++) {
    if (strcmp(argv[i], "-s")) continue;
    flag = 1;
    for (int j = i; j < argc - 1 && argv[j]; j++) argv[j] = argv[j + 1];
    argc--;
  }
  if (argc < 2) {
    fprintf(stderr, "ln: lack of arguments\n");
    return 0;
  }
  //for (int i = 0; i < argc; i++) printf("%s\n", argv[i]);
  Work(argc, argv, flag);
  return 0;
}