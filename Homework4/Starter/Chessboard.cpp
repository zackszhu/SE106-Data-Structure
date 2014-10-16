#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chessboard.h"


Chessboard::Chessboard(unsigned int r, unsigned int c): rows(r), cols(c)
{
  board = (int**)malloc(sizeof(int*) * rows);
  if (NULL == board)
    {
      printf("Chessboard memory allocation error.\n");
    }

  for (int i = 0; i < rows; i++)
    {
      board[i] = (int*)malloc(sizeof(int) * cols);
      if (NULL == board[i])
	{
	  printf("chessboard memory allocation error.\n");
	}
    }

  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
	{
	  board[i][j] = Empty;
	}
    }
}

Chessboard::~Chessboard()
{
  for (int i = 0; i < rows; i++)
    {
      free(board[i]);
    }
  free(board);
}
