#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class City {
public:
	City(int, int);
	void output(ofstream&, bool);
private:
	City() {} // no default constructor
	vector< vector<int> > cityMap, bestMap;
	int width, height, maxScore, score;
	bool isAvailable(int, int); // check if this building can be build here
	bool isValid(); // check if the map is valid
	void build(int, int); // recursion 
};

City::City(int _m, int _n):
 	height(_m), width(_n),
	cityMap(_n + 2, vector<int>(_m + 2)),
	maxScore(0), score(0) {
	// cout  << "Start Building!" << endl;
	build(1, 1);
}

bool City::isAvailable(int x, int y) {
	int building = cityMap[x][y];
	int blocks = cityMap[x + 1][y] | cityMap[x - 1][y] | cityMap[x][y + 1] | cityMap[x][y - 1];
	return (blocks & (building - 1)) == building - 1;
}

bool City::isValid() {
	// check all blocks validity
	for (int i = 1; i <= height; i++) {
		if (!isAvailable(width, i))
			return false;
	}
	return true;
}

void City::build(int x, int y) {
	if (x > width) {
		if (isValid()) {
			if (score > maxScore) {
				maxScore = score;
				bestMap = cityMap;
			}
		}
		return;
	}
	if ((width * height - (x - 1) * height - y + 1) * 2.31 + score < maxScore) {
		return;
	}
	for (int i = 3; i >= 1; i--) {
		cityMap[x][y] = i == 3 ? 4 : i;
		score += i;
		if (x > 1 && !isAvailable(x - 1, y)){
			score -= i;
			continue;
		}
		if (x == width && y > 1)
			if (!isAvailable(x, y - 1)) {
				score -= i;
				continue;
			}
		if (y == height)
			build(x + 1, 1);
		else
			build(x, y + 1);
		score -= i;
		cityMap[x][y] = 0;
	}
}

void City::output(ofstream& outfile, bool flag) {
	outfile << maxScore << endl;
	if (!flag) {
		for (int i = 1; i <= height; i++) {
			for (int j = 1; j <= width; j++) { 
				// if (bestMap[j][i] == 4)
				outfile << (bestMap[j][i] == 4 ? 3 : bestMap[j][i]) << " ";
			}
			outfile << endl;
		}
	}
	else {
		for (int i = 1; i <= width; i++) {
			for (int j = 1; j <= height; j++) { 
				outfile << (bestMap[i][j] == 4 ? 3 : bestMap[i][j]) << " ";
			}
			outfile << endl;
		}
	}
}

int main(int argc, char** argv) {
	ifstream infile;
	infile.open(argv[1]);
	ofstream outfile;
	outfile.open(argv[2]);
	int times;
	infile >> times;
	bool flag = false;
	for (int i = 0; i < times; i++) {
		int m, n;
		infile >> m >> n;
		if (n < m) 
			flag = true;
		City city(min(m, n), max(m, n));
		city.output(outfile, flag);
		flag = false;
	}
	infile.close();
	outfile.close();
}
