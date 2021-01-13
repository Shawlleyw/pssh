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

void Move(char *src, char *dest) {
  char path[1024], temp[1024];
  memset(path, 0, sizeof(path));
  strcpy(path, dest);
  strcpy(temp, src);
  int len = strlen(path);
  if (path[len - 1] != '/') path[len++] = '/';
  strcat(path, GetLast(temp));
  //printf("%s\n%s", path, src);
  if (access(path, F_OK) == 0) {
    char op;
    printf("File exists, overwrite '%s'? (y/n)", path);
    scanf("%c", &op);
    if (op == 'n' || op == 'N') return;
  }
  if (rename(src, path) == -1) fprintf(stderr, "mv: %s\n", strerror(errno));
}

void Rename(int argc, char **argv) {
  if (argc > 3) {
    fprintf(stderr, "mv: Too many arguments\n");
    return;
  }
  char new[1024];
  strcpy(new, argv[2]);
  if (access(new, F_OK) == 0) {
    char op;
    printf("File exists, overwrite '%s'? (y/n)", new);
    scanf("%c", &op);
    if (op == 'n' || op == 'N') return;
  }
  if (rename(argv[1], new) == -1) fprintf(stderr, "mv: %s\n", strerror(errno));
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "mv: lack of arguments\n");
    return 0;
  }
  DIR *dirptr = opendir(argv[argc - 1]);
  if (dirptr == NULL)
    Rename(argc, argv);
  else {
    for (int i = 1; i < argc - 1 && argv[i]; i++) Move(argv[i], argv[argc - 1]);
    closedir(dirptr);
  }
  return 0;
}