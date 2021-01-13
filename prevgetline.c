#include <errno.h>
#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../builtincmd/exit.h"
#include "defines.h"
#include "io.h"

const int BUF_SIZE_ARGV = 5;
const char *DELIM = " \n\t\r\a";
const char *FD_DELIM = "/";

void PrintLine(char *line) { printf(">>%s\n", line); }

void PrintArgv(char **argv, int cnt) {
  printf(">>");
  for (int i = 0; i < cnt && argv[i] != NULL; i++) printf("%s ", argv[i]);
  puts("");
}

int check(char x) {
  int len = strlen(DELIM);
  for (int i = 0; i < len; i++)
    if (x == DELIM[i]) return 0;
  return 1;
}
char *GetLast(char *str) {
  char *ret = str;
  for (char *next = strtok(str, FD_DELIM); next != NULL;
       next = strtok(NULL, FD_DELIM))
    ret = next;
  return ret;
}

int InitReadLine() {
  struct passwd *pwd = getpwuid(getuid());
  if (pwd == NULL) {
    fprintf(stderr, "Getting user name error : %s\n", strerror(errno));
    return 0;
  }

  char *path = (char *)malloc(1024);
  getcwd(path, 1024);
  if (path == NULL) {
    COL_DGR();
    fprintf(stderr, "Getting current working directory error : %s\n",
            strerror(errno));
    COL_CLEAR();
    return 0;
  }
  char *hostname = (char *)malloc(1024);
  if (gethostname(hostname, 1024) == -1) {
    COL_DGR();
    fprintf(stderr, "Getting hostname error\n");
    COL_CLEAR();
    return 0;
  }
  char *dir = GetLast(path);
  COL_YELLOW();
  fprintf(stdout, "[%s@%s", pwd->pw_name, hostname);
  COL_WHITE();
  fprintf(stdout, " \033[37;1m%s", dir);
  COL_YELLOW();
  fprintf(stdout, "]");
  COL_CLEAR();
  free(path);
  free(hostname);
  path = NULL;
  hostname = NULL;
  return 1;
}
void CtrlD() { Exit(SUCCESS); }
char *GetLine() {
  if (!InitReadLine()) return NULL;
  signal(SIGINT, SIG_IGN);
  char *line = readline("\033[33;1m$ \033[0m");
  add_history(line);
  return line;
}


int CheckIn(char x) {
  int len = strlen(DELIM);
  for (int i = 0; i < len; i++)
    if (x == DELIM[i]) return 1;
  return 0;
}
int Specified(char x) { return x == '|' || x == '<' || x == '>'; }


char **GetArgv(char **argv) {
  char *line = NULL;
  char **argv_new;
  line = GetLine();
  if (line == NULL) return NULL;
  //PrintLine(line);
  int cnt = 0, argc = 5;
  for (char *next = strtok(line, DELIM); next != NULL;
       next = strtok(NULL, DELIM)) {
    // while(!check(*next))next = strtok(NULL, DELIM);
    // printf("%d\n",cnt);
    argv[cnt++] = next;
    if (cnt == argc) {
      // printf("Before realloc : %p\n", argv);
      argc += BUF_SIZE_ARGV;
      argv_new = (char **)realloc(argv, argc * sizeof(char *));
      if (argv_new == NULL)
        return NULL;
      else
        argv = argv_new;
      // printf("After realloc : %p\n", argv);
    }
  }
  argv[cnt++] = NULL;
  //PrintArgv(argv, cnt);
  return argv;
}