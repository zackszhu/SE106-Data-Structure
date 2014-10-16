#include <stdio.h>
#include <stdlib.h>
#include "Chessboard.h"
#include "KQueens.h"
#include <iostream>
using namespace std;

int main()
{
	int m, n;
	cin >> m >> n;
	Chessboard *c = new Chessboard(m, n);
	cout << KQueens(c) << endl;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cout << c->board[i][j];
		}
		cout << endl;
	}
  	return 0;
}
