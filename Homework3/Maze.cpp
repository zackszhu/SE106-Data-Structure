/*
 * [TODO] delte this comment
 * Our test compiler is g++,
 * you can use '$ g++ -v' to see version, suggest 4.8 or newest,
 * add any header file you need from standard C++ library
 * and create any class or function you need to use
 */

/*
 * Use standard I/O to read test file and output answer.
 * There are three test file: testmap_1.txt, testmap_2.txt, testmap_3.txt,
 * and your output should like:
 * testmap_1: (1,1) (1,2) (1,3) (2,3) ...
 * More detail on course websit
 */
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Point {
	int x, y;
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	bool operator == (const Point &that) {
		return x == that.x && y == that.y;
	}
};

int dx[4] = {1, 0, 0, -1};
int dy[4] = {0, 1, -1, 0};

void workMaze(Point _now, Point _end,
			  const vector< vector<int> > &_map, 
			  vector<Point> &_path, 
			  vector< vector<bool> > &_walked, 
			  bool &_found) {
	_path.push_back(_now);
	// cout << _now.x << " " << _now.y << endl;
	_walked[_now.x][_now.y] = true;
	if (_now == _end){
		// cout << "found!" << endl;
		_found = true;
		return;
	}
	for (int i = 0; i < 4 && !_found; i++) {
		int x = _now.x + dx[i];
		int y = _now.y + dy[i];
		// cout << x << " " << y << " " << _map[x][y] << " " << _walked[x][y] << endl;
		if (_map[x][y] == 0 && _walked[x][y] == false) {
			workMaze(Point(x, y), _end, _map, _path, _walked, _found);
		}
	}
	if (_found)
		return;
	_path.pop_back();
	_walked[_now.x][_now.y] = false;
	return;
}

int main()
{
	string filename[3] = {"testmap_1", "testmap_2", "testmap_3"};
	for (int i = 0; i < 3; i ++) {
		string fName = filename[i] + ".txt";
		ifstream input(fName.c_str());
		int n, m;
		Point start, end;
		input >> n >> m;
		cout << filename[i] << ": ";
		vector< vector<int> > mazeMap(m + 2, vector<int>(n + 2));
		vector< vector<bool> > walked(m + 2, vector<bool>(n + 2, false));
		input.ignore(1024, '\n');
		for (int i = 0; i < m + 2; i++) {
			mazeMap[i][0] = 1;
			mazeMap[i][n + 1] = 1;
		}
		for (int i = 0; i < n + 2; i++) {
			mazeMap[0][i] = 1;
			mazeMap[m + 1][i] = 1;
		}
		for (int i = 0; i < m; i++) {
			string lineStr;
			getline(input, lineStr);
			for (int j = 0; j < n; j++) {
				char ch = lineStr[j];
				if (ch == '+' || ch == '-' || ch == '|') {
					mazeMap[i + 1][j + 1] = 1;
				}
				else {
					mazeMap[i + 1][j + 1] = 0;
					if (ch == 'S')
						start = Point(i + 1, j + 1);
					if (ch == 'E')
						end = Point(i + 1, j + 1);
				}
			}
		}
		// for (int i = 0; i < m + 2; i++) {
		// 	for (int j = 0; j < n + 2; j++) {
		// 		cout << mazeMap[i][j];
		// 	}
		// 	cout << endl;
		// }
		vector<Point> path;
		bool found(false);
		workMaze(start, end, mazeMap, path, walked, found);
		for (vector<Point>::iterator it = path.begin() + 1; it != path.end(); it++)
			cout << "(" << it->y - 1 << "," << it->x - 1 << ") ";
		cout << endl;
	}
	return 0;
}
