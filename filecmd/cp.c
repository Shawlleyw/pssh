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

void CpFile(char *src, char *dest) {
  int fd[2], len;
  char str[1024];
  fd[0] = open(src, O_RDONLY);
  if (fd[0] == -1) {
    fprintf(stderr, "cp: %s\n", strerror(errno));
    fflush(stderr);
    return;
  }
  struct stat fl;
  if (stat(src, &fl) == -1) {
    fprintf(stderr, "cp: %s\n", strerror(errno));
    fflush(stderr);
    return;
  }
  if (access(dest, F_OK) == 0) {
    char op;
    printf("File exists, overwrite '%s'? (y/n)", dest);
    fflush(stderr);
    scanf("%c", &op);
    if (op == 'n' || op == 'N') return;
  }
  fd[1] = open(dest, O_CREAT | O_WRONLY, fl.st_mode);
  if (fd[1] == -1) {
    fprintf(stderr, "cp: %s\n", strerror(errno));
    return;
  }
  while ((len = read(fd[0], str, 1024)) > 0) write(fd[1], str, len);
  close(fd[0]);
  close(fd[1]);
}

void Cp(char *argv_src, char *argv_dest) {
  char src[1024];
  char dest[1024];
  char temp[1024];
  memset(src, 0, sizeof(src));
  memset(dest, 0, sizeof(dest));
  memset(temp, 0, sizeof(temp));
  strcpy(src, argv_src);
  strcpy(temp, argv_src);
  strcpy(dest, argv_dest);
  int len_dest = strlen(dest);
  if (dest[len_dest - 1] != '/') dest[len_dest++] = '/';
  strcat(dest, GetLast(temp));
  CpFile(src, dest);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "cp: lack of arguments\n");
    return 0;
  }
  DIR *dirptr = opendir(argv[argc - 1]);
  if (dirptr == NULL) {
    CpFile(argv[1], argv[2]);
  } else {
    closedir(dirptr);
    for (int i = 1; i < argc - 1 && argv[i]; i++) {
      if (argv[i][0] == '>') {
        if (!strcmp(argv[i], ">")) i += 1;
        continue;
      }
      if (argv[i][0] == '<') {
        if (!strcmp(argv[i], "<")) i += 1;
        continue;
      }
      Cp(argv[i], argv[argc - 1]);
    }
  }
  return 0;
}