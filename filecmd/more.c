#include <curses.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>

int col = 72, line = 26;
const int BigNum = 1 << 20;
void GetTerminalSize() {
  if (setupterm(NULL, 1, NULL) == -1) {
    fprintf(stderr, "less: default terminal size: lines=26 columns=72\n");
    return;
  }
  int row = tigetnum("lines");
  int column = tigetnum("cols");
  if (row == -2 || column == -2) {
    fprintf(stderr, "less: default terminal size: lines=26 columns=72\n");
    return;
  }
  line = row;
  col = column;
  // printf("line: %d col: %d\n", line, col);
  char *clear = tigetstr("clear");
  char *cursor = tigetstr("cup");
  putp(clear);
  putp(tparm(cursor, 0, 0));
}

void CloseInput(struct termios *term, int fd) {
  term->c_lflag &= ~(ECHO | ICANON);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  tcsetattr(fd, TCSAFLUSH, term);
}

void RestartInput(struct termios *term, int fd) {
  if (tcsetattr(fd, TCSANOW, term) == -1)
    fprintf(stderr, "%s\n", strerror(errno));
}

int SetPos(short x) {
  switch (x) {
    case 'j':
      return 1;

    case '\n':
      return 1;

    case ' ':
      return line - 1;

    case 'q':
      return 0;

    case KEY_DOWN :
      return 1;

    default:
      return BigNum;
  }
}

void Display(FILE *in, FILE *out) {
  char str[1024];short opt;
  memset(str, 0, sizeof(str));
  int res = line - 1;
  while (fgets(str, 1024, in) != NULL) {
    fputs(str, stdout);
    res--;
    if (res == 0) {
      while (SetPos(opt = fgetc(out)) == BigNum);
      if (SetPos(opt) == 0) break;
      res += SetPos(opt);
    }
  }
}

void Work(FILE *fp) {
  struct termios *old_term, *new_term;
  FILE *ftty = fopen("/dev/tty", "w+");
  old_term = (struct termios *)malloc(sizeof(struct termios));
  new_term = (struct termios *)malloc(sizeof(struct termios));
  tcgetattr(fileno(ftty), old_term);
  *new_term = *old_term;
  CloseInput(new_term, fileno(ftty));
  Display(fp, ftty);
  fputc('\n', ftty);
  RestartInput(old_term, fileno(ftty));
  fflush(ftty);
  fclose(ftty);
  free(old_term);
  old_term = NULL;
}

void Deal(char *path) {
  FILE *fp = fopen(path, "r+");
  if (fp == NULL) {
    fprintf(stderr, "more: %s: %s\n", path, strerror(errno));
    return ;
  }
  Work(fp);
  fclose(fp);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "more: missing file name\n");
    return 0;
  }
  int cnt_file = 0;
  for(int i = 1; i < argc && argv[i]; i ++){
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
  cnt_file --;
  GetTerminalSize();
  for(int i = 1; i < argc && argv[i]; i++){
    if (argv[i][0] == '>') {
      if (!strcmp(argv[i], ">")) i += 1;
      continue;
    }
    if (argv[i][0] == '<') {
      if (!strcmp(argv[i], "<")) i += 1;
      continue;
    }
    if(cnt_file)puts(":::::::::::::::::::::::::::");
    fflush(stdout);
    Deal(argv[i]);
  }
  return 0;
}