#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdlib.h>
#include <string.h>

/*
 * This file defines class "Chessboard", which contains information of the 
 * chessboard. In this homework, you may NOT change the defination in this
 * file, or you will have problems passing the test.
 */


#define Empty 0
#define Queen 1

class Chessboard
{
 public:
  unsigned int rows;
  unsigned int cols;
  int **board;

 public:
  Chessboard(unsigned int r, unsigned int c);
  ~Chessboard();
};

#endif
