#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
const int ArgcSize = 5;
char **GetArgv(char **argv, char * line, char delim, int cnt) {
  int argc = 5, len = strlen(line), pre = 0;
  for (int i = 0; i < len; i++) {
    if(line[i] == delim){
      line[i] = '\0';
      if(pre < len && line[pre] != delim && argv[cnt - 1] != line + pre)argv[cnt++] = line + pre;
      pre = i + 1;
    }
    if (cnt == argc) {
      argc += ArgcSize;
      argv = (char **)realloc(argv, argc * sizeof(char *));
      if (argv == NULL)
        return NULL;
    }
  }
  if(argv[cnt - 1] != line + pre && line[pre] != delim && pre < len)argv[cnt++] = line + pre;
  if (cnt == argc) {
      argc += ArgcSize;
      argv = (char **)realloc(argv, argc * sizeof(char *));
      if (argv == NULL)
        return NULL;
    }
  argv[cnt++] = NULL;
  return argv;
}


char **AppendArgv(int argc, char **argv, char **ex_argv, char delim, char * str) {
  int cnt = 0; int Size = 5;
  for (int i = 0; i < argc && argv[i]; i++) {
    ex_argv[cnt++] = argv[i];
    if (cnt == Size) {
      Size += ArgcSize;
      ex_argv = (char **)realloc(ex_argv, Size * sizeof(char *));
    }
  }
  char ch; int len = 0;
  while ((ch = getchar()) != EOF) str[len++] = ch;
  str[len++] = '\0';
  ex_argv = GetArgv(ex_argv, str, delim, cnt);
#ifdef DEBUG
  fprintf(stderr, "ex_argv : %d\n", cnt);
  for(int i = 0; ex_argv[i]; i++)
    fprintf(stderr, "%d: %s\n", i, ex_argv[i]);
#endif
  return ex_argv;
}
pid_t pid;
void Interrupt() {
  // Print("Process Interrupt", stderr);
  if (kill(pid, SIGABRT) == -1) fprintf(stderr, "%s", strerror(errno));
  fprintf(stderr, "\n");
}
void Execute(char **argv) {
  //for(int i = 0; argv[i]; i ++)fprintf(stderr, "here : %s\n",argv[i]);
  if ((pid = fork()) < 0) {
    fprintf(stderr, "xargs: %s\n", strerror(errno));
    return;
  } else if (pid == 0) {
    if (execvp(argv[0], argv) == -1)
      fprintf(stderr, "xargs: %s\n", strerror(errno));
  } else {
    int ret = 0;
    signal(SIGINT, Interrupt);
    wait(&ret);
  }
}
extern char *optarg;
extern int optind;

int main(int argc, char **argv) {
  char str[1024];
  char delim = '\n';
  char **ex_argv = (char **)malloc(ArgcSize * sizeof(char *));
#ifdef DEBUG
  fprintf(stderr, "xargs :");
  for(int i=0; i < argc; i++)
    fprintf(stderr, "%s ",argv[i]);
  fprintf(stderr, "\n");
#endif
  if(getopt(argc, argv, "d:") != -1) delim = optarg[0];
  if (argc == optind) argv[argc++] = "echo";
  
  argv = AppendArgv(argc, argv, ex_argv, delim, str);
  Execute(argv + optind);
  if(argv != NULL){
    free(argv);
    argv = NULL;
  }
  return 0;
}