#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <fstream>
using namespace std;

class Huffman {
public:
	Huffman()
		: frequencyList(256, Data(0, 0)), header(NULL),
		outputBit(0), outputChar(0), inputChar(0), inputBit(0) {}

	~Huffman() {
		destroy(header);
	}
	
	void add(unsigned char ch) {
		int index = int(ch);
		frequencyList[index].frequency++;
		frequencyList[index].value = index;
	}

	void buildTree() {
		auto lessThan = [](const Node* x, const Node* y)->bool {
			return x->frequency > y->frequency;
		};
		priority_queue<Node*, vector<Node*>, decltype(lessThan)> heap(lessThan);
		for_each(frequencyList.begin(), frequencyList.end(), [&heap](const Data& _d) {
			if (_d.frequency != 0)
				heap.emplace(new Node(_d));
		});
		while (heap.size() > 1) {
			Node* tempLeft = heap.top();
			heap.pop();
			Node* tempRight = heap.top();
			heap.pop();
			heap.emplace(new Node(tempLeft, tempRight));
		}
		if (heap.size() == 0)
			header = NULL;
		else
			header = heap.top();
		Trace trace;
		trace.code.push_back(0);
		setTrace(header, trace, 1);
	}

	void buildTree(vector<int>& _p, vector<int>& _i, ifstream& ifs, ofstream& ofs) {
		header = build(_p.begin(), _i.begin(), _i.end());
		Node* tmp = new Node(header, new Node());
		header = tmp;
		char c;
		bool b;
		Node* index = header;
		while (ifs.get(c)) {
			for (auto i = 7; i >= 0; i--) {
				b = (c >> i) % 2;
				if (b == 0)
					index = index->left;
				if (b == 1)
					index = index->right;
				if (index == header->right)
					break;
				if (index->value < 256) {
					ofs.put((unsigned char)(index->value));
					index = header;
				}
			}
		}
	}

	void output(ofstream& ofs) {
		preOutput(header, ofs);
		for (int i = 0; i < 9; i++)
			writeBit(1, ofs);
		inOutput(header, ofs);
		for (int i = 0; i < 9; i++)
			writeBit(1, ofs);
		flush(ofs);
	}

	void output(unsigned char ch, ofstream& ofs) {
		for (auto it = traceTable[ch].code.begin(); it != traceTable[ch].code.end(); it++) {
			writeBit(*it, ofs);
		}
	}

	void flush(ofstream& ofs) {
		while (outputBit != 0)
			writeBit(1, ofs);
	}

	void input(vector<int>& _p, vector<int>& _i, ifstream& ifs) {
		char num;
		bool tmp = true;;
		while (tmp) {
			ifs.get(num);
			for (int i = 7; i >= 0; i--) {
				tmp = readBit((num >> i) % 2, _p) && tmp;
			}
		}
		tmp = true;;
		while (tmp) {
			ifs.get(num);
			for (int i = 7; i >= 0; i--) {
				tmp = readBit((num >> i) % 2, _i) && tmp;
			}
		}
	}

private:
	struct Data {
		int frequency;
		int value;
		Data(int _f, int _v) : frequency(_f), value(_v) {}
	};
	vector<Data> frequencyList;
	static int noValue;
	struct Node {
		unsigned int frequency;
		int value;
		Node* left;
		Node* right;
		Node() : left(NULL), right(NULL){}
		Node(const Data& _d)
			: frequency(_d.frequency), value(_d.value), left(NULL), right(NULL) {}
		Node(Node* _left, Node* _right)
			: frequency(_left->frequency + _right->frequency), value(noValue++), left(_left), right(_right) {}
		Node(int _value, Node* _left, Node* _right)
			: value(_value), left(_left), right(_right) {}
	};
	Node* header;
	void preOutput(Node* x, ofstream& ofs) {
		int temp = x->value;
		for (int i = 8; i >= 0; i--) {
			writeBit((temp >> i) % 2, ofs);
		}
		if (x->left != NULL)
			preOutput(x->left, ofs);
		if (x->right != NULL)
			preOutput(x->right, ofs);
	}
	void inOutput(Node* x, ofstream& ofs) {
		if (x->left != NULL)
			inOutput(x->left, ofs);
		int temp = x->value;
		for (int i = 8; i >= 0; i--) {
			writeBit((temp >> i) % 2, ofs);
		}
		if (x->right != NULL)
			inOutput(x->right, ofs);
	}
	int outputBit, outputChar;
	void writeBit(bool bit, ofstream& ofs) {
		// cout << bit;
		outputBit++;
		outputChar = (outputChar << 1) + bit;
		if (outputBit == 8) {
			ofs.put(outputChar);
			outputBit = 0;
			outputChar = 0;
		}
	}
	int inputBit, inputChar;
	bool readBit(bool bit, vector<int>& v) {
		bool ret = true;
		inputBit++;
		inputChar = (inputChar << 1) + bit;
		if (inputBit == 9) {
			if (inputChar != 511)
				v.push_back(inputChar);
			ret = (inputChar != 511);
			inputBit = 0;
			inputChar = 0;
		}
		return ret;
	}

	struct Trace {
		vector<bool> code;
	};

	map<int, Trace> traceTable;

	void setTrace(Node* x, Trace& t, int n) {
		t.code.push_back(0);
		t.code[n] = 0;
		if (x->left && x->left->value < 256)
			traceTable[x->left->value] = t;
		else
			setTrace(x->left, t, n + 1);
		t.code[n] = 1;
		if (x->right && x->right->value < 256)
			traceTable[x->right->value] = t;
		else
			setTrace(x->right, t, n + 1);
		t.code.pop_back();
	}

	Node* build(vector<int>::iterator _ph,
		vector<int>::iterator _ih,
		vector<int>::iterator _it) {
		if (_it <= _ih) return NULL;
		auto f = _ih;
		for (auto i = _ih; i != _it; i++)
		if (*i == *_ph) {
			f = i;
			break;
		}
		Node* father = new Node(*_ph, build(_ph + 1, _ih, f), build(_ph + (f - _ih) + 1, f + 1, _it));
		return father;
	}
	void destroy(Node* node) {
		if (node != NULL) {
			destroy(node->left);
			destroy(node->right);
			delete node;
		}
	}
};

#endif