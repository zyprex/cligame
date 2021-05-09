/*
 * 2048 game
 */
/* screen clear command */
#ifdef _WIN64
#  define CLR "cls"
#else
#  define CLR "clear"
#endif
/* debug print */
/* #define DEBUG */
#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LINE_LEN 4
typedef int Board[4][4]; /* game board */
typedef int* Line[LINE_LEN];

void printBoard(Board board);
int isFullBoard(Board board);
void gen2or4(Board board);
void move(Board board);
void printLine(int* line);
void savePrevBoard(Board board, Board prev_board);
int isBoardChanged(Board board, Board prev_board);
int lineMove(Line line);
void lineMoveUp(Board board);
void lineMoveDown(Board board);
void lineMoveLeft(Board board);
void lineMoveRight(Board board);

void printBoard(Board board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%5d", board[i][j]);
    }
    putchar(10);
  }
}

int isFullBoard(Board board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if(board[i][j] == 0)
        return 0;
    }
  }
  return 1;
}

int canMove(Board board) {
  int cgrid; /* current grid */
  if (isFullBoard(board)) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        cgrid = board[i][j];
        if (i - 1 >= 0 && cgrid == board[i - 1][j]) return 0;
        if (j - 1 >= 0 && cgrid == board[i][j - 1]) return 0;
        if (i + 1 < 4  && cgrid == board[i + 1][j]) return 0;
        if (j + 1 < 4  && cgrid == board[i][j + 1]) return 0;
      }
    }
  }
  return 1;
}

void gen2or4(Board board) {
  int newval = rand() % 2 ?  2 : 4, row, col;
  D(printf("newval: %d ", newval));
  while (1) {
    row = rand() % 4;
    col = rand() % 4;
    if (board[row][col] == 0) {
      D(printf("+(%d,%d)\n", row, col));
      board[row][col] = newval;
      break;
    }
  }
}

void move(Board board) {
  switch (getchar()) {
    case 'w': lineMoveUp(board);    break;
    case 's': lineMoveDown(board);  break;
    case 'a': lineMoveLeft(board);  break;
    case 'd': lineMoveRight(board); break;
    case 'q': exit(0); break;
    default: break;
  }
  getchar();
}

void printLine(int* line) {
  for (int i = 0; i < 4; ++i) {
    printf("%d,", line[i]);
  }
  putchar(10);
}

void savePrevBoard(Board board, Board prev_board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      prev_board[i][j] = board[i][j];
    }
  }
}

int isBoardChanged(Board board, Board prev_board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if(board[i][j] != prev_board[i][j])
        return 1;
    }
  }
  return 0;
}

int lineMove(Line line) {
  int min_index = 0;
  for (int i = 1; i < LINE_LEN; ++i) {
    if (*line[i] != 0) {
      int j; /* find first non-zero grid */
      for (j = i - 1; j >= min_index && *line[j] == 0; --j) {}
      D(printf("i: %d, j: %d, min_index: %d\n", i, j, min_index));
      /* special case: all front grid is zero */
      if (j < min_index) {
        *line[min_index] = *line[i];
        *line[i] = 0;
      }
      /* mix case */
      else if (*line[j] == *line[i]) {
        *line[j] += *line[i];
        *line[i] = 0;
        min_index = j + 1;
      }
      /* shift case */
      else if (*line[j + 1] == 0) {
        *line[j + 1] = *line[i];
        *line[i] = 0;
      }
      D(printLine(*line));
    }
  }
  return 0;
}

void lineMoveUp(Board board) {
  /* each column move up, see resolved procedure here */
  /* Line col0 = {&board[0][0], &board[1][0], &board[2][0], &board[3][0]}; */
  /* lineMove(col0); */
  /* Line col1 = {&board[0][1], &board[1][1], &board[2][1], &board[3][1]}; */
  /* lineMove(col1); */
  /* Line col2 = {&board[0][2], &board[1][2], &board[2][2], &board[3][2]}; */
  /* lineMove(col2); */
  /* Line col3 = {&board[0][3], &board[1][3], &board[2][3], &board[3][3]}; */
  /* lineMove(col3); */
  Line cline;
  for (int i = 0; i < LINE_LEN; ++i) {
    for (int j = 0; j < LINE_LEN; ++j) {
      cline[j] = &board[j][i];
    }
    lineMove(cline);
  }
}

void lineMoveDown(Board board) {
  Line cline;
  for (int i = 0; i < LINE_LEN; ++i) {
    for (int j = 0; j < LINE_LEN; ++j) {
      cline[j] = &board[LINE_LEN - 1 - j][i];
    }
    lineMove(cline);
  }
}

void lineMoveLeft(Board board) {
  Line cline;
  for (int i = 0; i < LINE_LEN; ++i) {
    for (int j = 0; j < LINE_LEN; ++j) {
      cline[j] = &board[i][j];
    }
    lineMove(cline);
  }
}

void lineMoveRight(Board board) {
  Line cline;
  for (int i = 0; i < LINE_LEN; ++i) {
    for (int j = 0; j < LINE_LEN; ++j) {
      cline[j] = &board[i][LINE_LEN -  1 - j];
    }
    lineMove(cline);
  }
}

int main(int argc, char *argv[]) {
  /* test lineMove */
  /* int testline[4] = {8, 2, 4, 4}; */
  /* int testline[4] = {2, 2, 4, 4}; */
  /* int testline[4] = {2, 2, 8, 4}; */
  /* int testline[4] = {2, 2, 2, 2}; */
  /* int testline[4] = {1, 2, 2, 4}; */
  /* Line line = {&testline[0], &testline[1], &testline[2], &testline[3]}; */
  /* lineMove(line); */
  /* printLine(testline); */
  /* return 0; */
  srand((unsigned)time(0));
  Board board = {0};
  Board prev_board = {{1}};
  while (1) {
    system(CLR);
    puts("---Type w/s/a/d <Enter> , move Up/Down/Left/Right, or q <Enter> to quit---");
    if (!isFullBoard(board) && isBoardChanged(board, prev_board)) {
      gen2or4(board);
    }
    savePrevBoard(board, prev_board);
    printBoard(board);
    move(board);
    /* game over due to no more place and can't move */
    if (!canMove(board)) {
      puts("game over!");
      break;
    }
  }
  return 0;
}

/* vim:set fdm=syntax tw=80: */
