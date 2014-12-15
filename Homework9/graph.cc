#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <fstream>
using namespace std;

struct Node {
	int y;
	int value;
	Node(int _y, int _z) : y(_y), value(_z) {}
};

class Graph {
	vector< list<Node> > graph;
public:
	Graph(int n) : graph(n) {}
	void addArc(int _x, int _y, int _z) {
		graph[_x].push_back(Node(_y, _z));
	}
	inline list<Node>::const_iterator getBegin(int _x) const {
		return graph[_x].cbegin();
	}
	inline list<Node>::const_iterator getEnd(int _x) const {
		return graph[_x].cend();
	}
};

const int MAXINT = (unsigned int)~0 >> 1;

int main(int argc, char** argv) {
	ifstream infile;
	infile.open(argv[1]);
	int n, m;
	infile >> n;
	infile >> m;
	Graph map(n + 1);
	for (auto i = 0; i < m; i++) {
		int x, y, z;
		infile >> x >> y >> z;
		map.addArc(x, y, z);
	}
	vector<int> dist(n + 1, MAXINT);
	auto lessThan = [&](int x, int y) -> bool {
		return dist[x] > dist[y];
	};
	priority_queue<int, vector<int>, decltype(lessThan)> heap(lessThan);
	for (auto it = map.getBegin(0); it != map.getEnd(0); it++) {
		dist[it->y] = it->value;
		heap.push(it->y);
	}
	vector<bool> used(n + 1, false);
	used[0] = true;
	dist[0] = 0;
	for (auto i = 0; i <= n; i++) {
		int tmin = MAXINT;
		int k = 0;
		while (heap.size() > 0 && used[heap.top()]){
			heap.pop();
		}
		if (heap.size() <= 0) 
			break;
		k = heap.top();
		heap.pop();
		used[k] = true;
		for (auto it = map.getBegin(k); it != map.getEnd(k); it++) {
			if (dist[it->y] > (unsigned int)(dist[k] + it->value)) {
				dist[it->y] = dist[k] + it->value;
				heap.push(it->y);
			}
		}
	}
	for (auto i = 0; i <= n; i++) 
		cout << (dist[i] == MAXINT ? -1 : dist[i])  << " ";
	cout << endl;
}