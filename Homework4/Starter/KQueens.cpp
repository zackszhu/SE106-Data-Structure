#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "KQueens.h"
#include "Chessboard.h"
using namespace std;

/*
 * You should modify this function. Before you start, make
 * sure you have a clear idea of what you are doing.
 * Good Luck!
 */

class Game {
	bool *leftDown;
	bool *rightDown;
	int m, n, k;

public:
	int *row;

public:
	Game(Chessboard *c);
	bool work(int _colomn, int _k, int _depth);
	int getMaxK();
};

Game::Game(Chessboard *c) 
: 	m(c->rows), n(c->cols),
	row(new int[c->rows]),
	leftDown(new bool[c->rows + c->cols - 2]),
	rightDown(new bool[c->rows + c->cols - 2]) {
		memset(row, -1, sizeof(int) * m);

	}

bool Game::work(int _colomn, int _k, int _depth) {
	if (_depth == _k)
		return true;
	if (n - _colomn < _k - _depth)
		return false;
	for (int i = 0; i < m; i++) {
		if (row[i] == -1 && !leftDown[_colomn + i] && !rightDown[_colomn - i + m - 1]) {
			row[i] = _colomn;
			leftDown[_colomn + i] = true;
			rightDown[_colomn - i + m - 1] = true;
			if (work(_colomn + 1, _k, _depth + 1))
				return true;
			else {
				row[i] = -1;
				leftDown[_colomn + i] = false;
				rightDown[_colomn - i + m - 1] = false;
			}
		}
	}
	return work(_colomn + 1, _k, _depth);
}

int Game::getMaxK() {
	return m < n ? m : n;
}

int KQueens(Chessboard *c)
{
	Game g(c);
	int maxK = g.getMaxK();
	for (int i = maxK; i > 0; i--) {
		if (g.work(0, i, 0)) {
			for (int j = 0; j < c->rows; j++)
				c->board[j][g.row[j]] = 1;
			return i;
		}
	}
  	return -1;
}
