#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include<dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
const int BUF_SIZE = 1024;

void Cat(char* path) {
  if (path[0] == '<') path++;
  int fd = 0, len = 0;
  if (path != NULL) fd = open(path, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "cat: %s: %s\n", path, strerror(errno));
    return;
  }
  char str[1025];
  str[1024] = '\0';
  while ((len = read(fd, str, 1024)) > 0) write(1, str, len);
  close(fd);
}

void Deal() {
  char str[1024];
  int len;
  while ((len = read(0, str, 1024)) > 0) write(1, str, len);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    Deal();
    return 0;
  }
  int cnt = 0;
  DIR * dir ;
  for (int i = 1; i < argc && argv[i]; i++) {
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (!strcmp(argv[i], "<")) continue;
    if((dir = opendir(argv[i])) != NULL){
      closedir(dir);
      fprintf(stderr, "cat: %s: is a directory\n", argv[i]);
    }
    else Cat(argv[i]);
  }
  //fflush(stdout);
  return 0;
}