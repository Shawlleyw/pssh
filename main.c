#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtincmd/cd.h"
#include "builtincmd/echo.h"
#include "builtincmd/exit.h"
#include "builtincmd/export.h"
#include "builtincmd/kill.h"
#include "builtincmd/pwd.h"
#include "src/defines.h"
#include "src/getline.h"
#include "src/io.h"
const int BufSizeMain = 16;

pid_t pid;
int bg_run = 0;
void Interrupt() {
  // Print("Process Interrupt", stderr);
  if (kill(pid, SIGABRT) == -1) fprintf(stderr, "%s", strerror(errno));
  fprintf(stderr, "\n");
}
int Execute(char **argv, int bg) {
  EXIT_CODE opt = SUCCESS;
  if (bg) ++bg_run;
  if ((pid = fork()) == -1) {
    Print(strerror(errno), stderr);
    return 0;
  }
  if (pid == 0) {
    if (execvp(argv[0], argv) == -1) {
      Print(strerror(errno), stderr);
      opt = ERROR;
    }
    if (bg) {
      printf("[%d] complete\t", bg_run);
      for (int i = 0; argv[i]; i++) printf("%s ", argv[i]);
      printf("\n");
    }
    Exit(opt);
  } else {
    if (bg) {
      printf("[%d] %d\t", bg_run, pid);
      for (int i = 0; argv[i]; i++) printf("%s ", argv[i]);
      printf("\n");
    } else {
      int status = 0;
      signal(SIGINT, Interrupt);
      wait(&status);
      signal(SIGINT, SIG_IGN);
    }
  }
  return 1;
}

void Work(char **argv, int fin) {
  if (argv[0] == NULL) return;
#ifdef DEBUG
  for (int i = 0; argv[i]; i++) fprintf(stderr, "%s ", argv[i]);
  fprintf(stderr, "\n");
#endif

  int pipe_pos = -1, bg = 0, type = 0;
  int fio[2], fildes[2] = {-1, -1}, redirc[2] = {-1, -1};
  for (int i = 0; argv[i] != NULL; i++)
    if (!strcmp(argv[i], "&")) {
      bg = 1;
      for (int j = i; argv[j] != NULL; j++) argv[j] = argv[j + 1];
    }
  for (int i = 0; argv[i] != NULL; i++)
    if (!strcmp(argv[i], "|")) {
      pipe_pos = i;
      argv[i] = NULL;
      break;
    }
  if (pipe_pos != -1) {
    if (pipe(fildes) == -1) {
      Print(strerror(errno), stderr);
      return;
    }
    type = 1;
    argv[pipe_pos] = NULL;
  }
  if (argv[0] == NULL) {
    fprintf(stderr, "sh: command error\n");
    return;
  }
  for (int i = 1; argv[i]; i++) {
    if (argv[i][0] == '<') {
      if (fin != -1) {
        COL_DGR();
        Print("sh: Command error ", stderr);
        COL_CLEAR();
        return;
      }
      if (!strcmp(argv[i], "<")) {
        redirc[0] = open(argv[i + 1], O_RDONLY);
        for (int j = i; argv[j + 1]; j++) argv[j] = argv[j + 2];
        i--;
      } else
        redirc[0] = open(argv[i] + 1, O_RDONLY);
      if (redirc[0] == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
      }
      type |= 1;
    }
    if (argv[i][0] == '>') {
      if (fildes[1] != -1) {
        COL_DGR();
        Print("sh: Command error ", stderr);
        COL_CLEAR();
        return;
      }
      if (!strcmp(argv[i], ">")) {
        redirc[1] = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        for (int j = i; argv[j + 1]; j++) argv[j] = argv[j + 2];
        i--;
      } else
        redirc[1] = open(argv[i] + 1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (redirc[1] == -1) {
        COL_DGR();
        fprintf(stderr, "%s\n", strerror(errno));
        COL_CLEAR();
        return;
      }
      fildes[1] = redirc[1];
      type |= 2;
    }
  }

  if (redirc[0] != -1) {
    if ((fio[0] = dup(0)) == -1) {
      COL_DGR();
      fprintf(stderr, "%s\n", strerror(errno));
      COL_CLEAR();
      return;
    }
    if (dup2(redirc[0], 0) == -1) {
      COL_DGR();
      fprintf(stderr, "%s\n", strerror(errno));
      COL_CLEAR();
      return;
    }
  }

  if (fildes[1] != -1) {
    if ((fio[1] = dup(1)) == -1) {
      COL_DGR();
      fprintf(stderr, "%s\n", strerror(errno));
      COL_CLEAR();
      return;
    }
    if (dup2(fildes[1], 1) == -1) {
      COL_DGR();
      fprintf(stderr, "%s\n", strerror(errno));
      COL_CLEAR();
      return;
    }
  } else
    fildes[1] = 1;
  /*
    for(int i = 0; argv[i]; i++)
    fprintf(stderr, "%s ",argv[i]);
    fputc('\n',stderr);
  */
  if (!strcmp(argv[0], "cd"))
    Cd(argv);
  else if (!strcmp(argv[0], "echo"))
    Echo(argv);
  else if (!strcmp(argv[0], "pwd"))
    Pwd(argv);
  else if (!strcmp(argv[0], "exit"))
    Exit(SUCCESS);
  else if (!strcmp(argv[0], "kill"))
    Kill(argv);
  else if (!strcmp(argv[0], "export"))
    Export(argv);
  else
    Execute(argv, bg);
    
  if (type & 1) {
    close(redirc[0]);
    dup2(fio[0], 0);
  }
  if (pipe_pos != -1) {
    close(fildes[1]);
    dup2(fio[1], 1);
    fio[0] = dup(0);
    dup2(fildes[0], 0);
    Work(argv + pipe_pos + 1, 0);
    close(fildes[0]);
    dup2(fio[0], 0);
  } else if (type & 2) {
    close(fildes[1]);
    dup2(fio[1], 1);
  }
}

int ShInit() {
  char *shpath = "/home/wsy/Documents/sh/.sh";
  FILE *fp = fopen(shpath, "r");
  if (fp == NULL) {
    printf("sh: %s\n", strerror(errno));
    return 0;
  }
  char str[1024];
  while (~fscanf(fp, "%s", str))
    if (SetEnv(str) == 0) {
      fprintf(stderr, "sh: .sh configuration error\n");
      fclose(fp);
      return 0;
    }
  fclose(fp);
  return 1;
}

int main() {
  int break_opt = 0;
  if (!ShInit()) {
    puts("sh: read initial environmental viriables failure");
    return 0;
  }
  // else printf("Init complete\n");
  while (1) {
    if (break_opt) break;
    char **argv = (char **)malloc(BufSizeMain * sizeof(char *));
    if (argv == NULL) {
      Print("sh: Memory allocation error !", stderr);
      continue;
    }
    argv = GetArgv(argv);
    if (argv == NULL) {
      break_opt = 1;
      continue;
    }
    Work(argv, -1);
    free(argv);
    argv = NULL;
  }

  return 0;
}