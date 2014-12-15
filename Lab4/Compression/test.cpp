#include "huffman-2.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

int Huffman::noValue = 256;

int main() {
	string str = "aaaaaabbccc";
	Huffman huffmanTree;
	for_each(str.begin(), str.end(), [&huffmanTree](char ch) {
		huffmanTree.add(ch);
	});
	huffmanTree.buildTree();
	huffmanTree.output();
		for_each(str.begin(), str.end(), [&huffmanTree](char ch) {
		huffmanTree.output(ch);
	});
	// vector<int> inOrder, preOrder;
	// huffmanTree.input(preOrder, inOrder);
	// huffmanTree.buildTree(preOrder, inOrder);
}