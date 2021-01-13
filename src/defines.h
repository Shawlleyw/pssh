#ifndef SH_SRC_DFS_H_
#define SH_SRC_DFS_H_
void COL_BLACK();
void COL_RED();
void COL_GREEN();
void COL_YELLOW();
void COL_BLUE();
void COL_PURPLE();
void COL_DGR();
void COL_WHITE();
void COL_CLEAR();
enum EXIT_STATUS_CODE {
  SUCCESS = 0,
  ERROR = 1,
  MISUSE = 2,
  INEXECUTABLE = 126,
  NOT_FIND = 127
};

typedef enum EXIT_STATUS_CODE EXIT_CODE;

#endif