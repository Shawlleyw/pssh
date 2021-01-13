#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../src/io.h"
int tot_size = 0;
typedef struct FileDetails {
  char str[11];
  char usr_name[32];
  char grp_name[32];
  char last_time[128];
  char file_name[64];
  char link_name[128];
  int link;
  long size;
} FD;

char *GetGroupName(gid_t gid, struct group *grp) {
  grp = getgrgid(gid);
  if (grp == NULL) return NULL;
  return grp->gr_name;
}

char *GetUserName(uid_t uid, struct passwd *pwd) {
  pwd = getpwuid(uid);
  return pwd->pw_name;
}

void StoreDetails(char *dir_name, char *path, FD *fdt, int cnt) {
  struct stat state;
  struct passwd pwd;
  struct group grp;
  char pat[1024];
  strcpy(pat, dir_name);
  int len = strlen(pat);
  if (pat[len - 1] != '/') {
    pat[len++] = '/';
    pat[len] = '\0';
  }
  strcat(pat, path);
  if (lstat(pat, &state) == -1) {
    fprintf(stderr, "%s\n", strerror(errno));
    return;
  }
  
  char str_mode[11];
  strcpy(str_mode, "----------");
  str_mode[10] = '\0';
  int mode = state.st_mode;
  // file or dir
  if (S_ISDIR(mode))
    str_mode[0] = 'd';
  else if (S_ISLNK(mode))
    str_mode[0] = 'l';
  // user mode
  if (mode & S_IRUSR) str_mode[1] = 'r';
  if (mode & S_IWUSR) str_mode[2] = 'w';
  if (mode & S_IXUSR) str_mode[3] = 'x';
  // group mode
  if (mode & S_IRGRP) str_mode[4] = 'r';
  if (mode & S_IWGRP) str_mode[5] = 'w';
  if (mode & S_IXGRP) str_mode[6] = 'x';
  // other group mode
  if (mode & S_IROTH) str_mode[7] = 'r';
  if (mode & S_IWOTH) str_mode[8] = 'w';
  if (mode & S_IXOTH) str_mode[9] = 'x';

  char src[1024];
  if (str_mode[0] == 'l' && readlink(pat, src, 1024) == -1) {
    fprintf(stderr, "%s\n", strerror(errno));
    return;
  }
  strcpy(fdt[cnt].link_name, src);
  strcpy(fdt[cnt].grp_name, GetGroupName(state.st_gid, &grp));
  strcpy(fdt[cnt].str, str_mode);
  strcpy(fdt[cnt].last_time, 4 + ctime(&state.st_mtime));
  strcpy(fdt[cnt].usr_name, GetUserName(state.st_uid, &pwd));
  fdt[cnt].link = state.st_nlink;
  fdt[cnt].size = state.st_size;
  tot_size += state.st_blocks;
}

void PrintList(FD *fdt, int cnt, int flag) {
  printf("%d \n", cnt);
  int col_pr = 0;
  printf("total %d\n", tot_size >> 1);
  for (int i = 0; i < cnt; i++) {
    if (flag) {
      printf("%s ", fdt[i].str);
      printf("%2d ", fdt[i].link);
      printf("%-2s ", fdt[i].usr_name);
      printf("%-2s ", fdt[i].grp_name);
      printf("%6ld ", fdt[i].size);
      printf("%.12s ", fdt[i].last_time);
    }
    switch (fdt[i].str[0]) {
      case 'd':
        printf("\033[1;35m");
        col_pr = 1;
        break;
      case 'l':
        printf("\033[1;31m");
        col_pr = 2;
        break;
    }
    if (fdt[i].str[3] == 'x' && !col_pr) printf("\033[1;32m");
    printf("%s", fdt[i].file_name);
    if (fdt[i].str[0] == 'l' && flag) {
      printf(" -> %s", fdt[i].link_name);
    }
    printf("\n");
    printf("\033[0m");
  }
}

int Calcch(char x) {
  if (x >= 'A' && x <= 'Z') return x - 'A' + 'a';
  return (int)x;
}

int CheckCmp(char *x, char *y) {
  int len1 = strlen(x), len2 = strlen(y);
  for (int i = 0; i < len1 && i < len2; i++)
    if (Calcch(x[i]) != Calcch(y[i])) return Calcch(x[i]) < Calcch(y[i]);
  return len1 < len2;
}

int CmpFd(FD x, FD y) { return CheckCmp(x.file_name, y.file_name); }

void swap(FD *x, FD *y) {
  FD temp = *x;
  *x = *y;
  *y = temp;
}

void sort(FD *fdt, int cnt) {
  for (int i = 0; i < cnt; i++)
    for (int j = 0; j + i < cnt - 1; j++)
      if (CmpFd(fdt[j + 1], fdt[j])) swap(fdt + j, fdt + j + 1);
}

void Ls(char *dir_name, int flag) {
  int cnt = 0;
  tot_size = 0;
  DIR *dir_ptr = opendir(dir_name);
  if (dir_ptr == NULL) {
    fprintf(stderr, "ls: %s: %s\n", dir_name, strerror(errno));
    return;
  }
  FD *fdt = (FD *)malloc(64 * sizeof(FD));
  if (fdt == NULL) {
    fprintf(stderr, "ls: %s\n", strerror(errno));
    return;
  }
  for (struct dirent *file_ptr = readdir(dir_ptr); file_ptr != NULL;
       file_ptr = readdir(dir_ptr)) {
    if (file_ptr->d_name[0] == '.') continue;
    strcpy(fdt[cnt].file_name, file_ptr->d_name);
    StoreDetails(dir_name, file_ptr->d_name, fdt, cnt);
    cnt++;
  }
  sort(fdt, cnt);
  PrintList(fdt, cnt, flag);
  closedir(dir_ptr);
  free(fdt);
  fdt = NULL;
}

int main(int argc, char **argv) {
  int flag = 0, exist = 0, cnt_file = 0;
  for (int i = 1; i < argc; i++)
    if (!strcmp(argv[i], "-l")) flag = 1;
  for (int i = 1; i < argc && argv[i] != NULL; i++) {
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    cnt_file++;
  }
  for (int i = 1; i < argc && argv[i] != NULL; i++) {
    if (!strcmp(argv[i], "-l")) continue;
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    exist = 1;
    if (cnt_file - flag > 1) printf("\033[1;32m%s:\n\033[0m", argv[i]);
    Ls(argv[i], flag);
  }
  if (!exist) Ls(".", flag);
  fflush(stdout);
  return 0;
}