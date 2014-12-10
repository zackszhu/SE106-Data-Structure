#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct treeNode {
	int value;
	treeNode* left;
	treeNode* right;
	treeNode() {}
	treeNode(int x, treeNode* _left, treeNode* _right) 
	: value(x), left(_left), right(_right) {}
};

treeNode* build(vector<int>::iterator _ph,
				vector<int>::iterator _ih,
				vector<int>::iterator _it) {
	if (_it <= _ih) return NULL;
	auto f = _ih;
	for (auto i = _ih; i != _it; i++)
		if (*i == *_ph) {
			f = i;
			break;
		}
	treeNode* father = new treeNode(*_ph, build(_ph + 1, _ih, f), build(_ph + (f - _ih) + 1, f + 1, _it));
	// // treeNode* leftSon = build(_ph + 1, _ih, f);
	// // treeNode* rightSon = build(_ph + (f - _ih) + 1, f + 1, _it);
	// father->left = leftSon;
	// father->right = rightSon;
	return father;
}

void output(ofstream& outfile, treeNode* node) {
	if (node->left) 
		output(outfile, node->left);
	if (node->right)
		output(outfile, node->right);
	outfile << node->value << " ";
}

int main(int argc, char** argv) {
	ifstream infile;
	infile.open(argv[1]);
	ofstream outfile;
	outfile.open(argv[2]);
	int m;
	infile >> m;
	for (int j = 0; j < m; j++) {
		int n;
		infile >> n;
		vector<int> preOrder(n), inOrder(n);
		for (auto i = preOrder.begin(); i !=preOrder.end(); i++)
			infile >> *i;
		for (auto i = inOrder.begin(); i !=inOrder.end(); i++)
			infile >> *i;
		treeNode *head = build(preOrder.begin(), inOrder.begin(), inOrder.end());
		output(outfile, head);
		outfile << endl;
	}
}