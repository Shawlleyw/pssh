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
#include "../src/defines.h"
#include "../src/io.h"

const int BUF_SIZE_ARGV = 5;
const char *DELIM = " \n\t\r\a|<>";
const char *FD_DELIM = "/";

void PrintLine(char *line) { printf(">>%s\n", line); }

void PrintArgv(char **argv, int cnt) {
  printf(">>");
  for (int i = 0; i < cnt && argv[i] != NULL; i++)
    printf("%s ", argv[i]);
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

char temp[1024];
int vis1[1024], vis2[1024];
char *GetLine() {
  if (!InitReadLine()) return NULL;
  signal(SIGINT, SIG_IGN);
  char *line = readline("\033[33;1m$ \033[0m");
  if (line != NULL) {
    int len = strlen(line);
    line[len] = '\0';
    strcpy(temp, line);
    memset(vis1, 0, sizeof(vis1));
    memset(vis2, 0, sizeof(vis2));
  }
  add_history(line);
  return line;
}

int CheckIn(char x) {
  if (x == '\0') return 1;
  int len = strlen(DELIM);
  for (int i = 0; i < len; i++)
    if (x == DELIM[i]) return 1;
  return 0;
}
int Specified(char x) { return x == '|' || x == '<' || x == '>'; }

char *ptr_tok;
char *Token(char *line) {
  int flag = 0;
  if (line != NULL) {
    ptr_tok = line;
    flag = 1;
  }
  char temp;
  while (*ptr_tok != '\0') {
    // putchar(*ptr_tok);
    if (CheckIn(*ptr_tok)) {
      temp = *ptr_tok;
      *ptr_tok = '\0';
      if (Specified(temp)) return ++ptr_tok;
    } else if (flag || (CheckIn(*(ptr_tok - 1)) && !Specified(*(ptr_tok - 1))))
      return ptr_tok++;
    ptr_tok++;
  }
  return NULL;
}

char **GetArgv(char **argv) {
  char *line = NULL;
  char **argv_new;
  line = GetLine();
  if (line == NULL) return NULL;
  // PrintLine(line);
  int cnt = 0, argc = 5, pos;
  for (char *next = Token(line); next != NULL; next = Token(NULL)) {
    pos = next - line;
    if (!vis2[pos - 1] && Specified(temp[pos - 1])) {
      argv[cnt++] = temp + pos - 1;
      vis2[pos - 1] = 1;
      temp[pos] = '\0';
      if (cnt == argc) {
        argc += BUF_SIZE_ARGV;
        argv_new = (char **)realloc(argv, argc * sizeof(char *));
        if (argv_new == NULL)
          return NULL;
        else
          argv = argv_new;
      }
    }
    if (!vis1[next - line] && !CheckIn(*next)) {
      argv[cnt++] = next;
      vis1[next - line] = 1;
      if (cnt == argc) {
        argc += BUF_SIZE_ARGV;
        argv_new = (char **)realloc(argv, argc * sizeof(char *));
        if (argv_new == NULL)
          return NULL;
        else
          argv = argv_new;
      }
    }
  }
  argv[cnt++] = NULL;
  //PrintArgv(argv, cnt);
  return argv;
}