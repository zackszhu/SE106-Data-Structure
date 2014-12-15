#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "huffman.h"
using namespace std;

int Huffman::noValue = 256;

void naiveCopy(string inputFilename, string outputFilename) {
  ofstream ofs(outputFilename.c_str(), ios::out | ios::binary);
  ifstream ifs(inputFilename.c_str(), ios::in | ios::binary);
  char c;
  while (ifs.get(c)) ofs.put(1);
  ofs.close();
  ifs.close();
}

void compress(string inputFilename, string outputFilename) {
  ofstream ofs(outputFilename.c_str(), ios::out | ios::binary);
  ifstream ifs(inputFilename.c_str(), ios::in | ios::binary);
  if (ifs.peek() == EOF)
    return;
  Huffman huffmanTree;
  char c;
  while (ifs.get(c)) {
    huffmanTree.add(c);
  }
  huffmanTree.buildTree();
  huffmanTree.output(ofs);
  ifstream ifs2(inputFilename.c_str(), ios::in | ios::binary);
  while (ifs2.get(c)) {
    huffmanTree.output(c, ofs);
  }
  huffmanTree.flush(ofs);
};

void decompress(string inputFilename, string outputFilename) {
  ofstream ofs(outputFilename.c_str(), ios::out | ios::binary);
  ifstream ifs(inputFilename.c_str(), ios::in | ios::binary);
  if (ifs.peek() == EOF)
    return;
  Huffman huffmanTree;
  vector<int> inOrder, preOrder;
  huffmanTree.input(preOrder, inOrder, ifs);
  huffmanTree.buildTree(preOrder, inOrder, ifs, ofs);
}

void useage(string prog) {
  cerr << "Useage: " << endl
    << "    " << prog << "[-d] input_file output_file" << endl;
  exit(2);
}

int main(int argc, char* argv[]) {
  int i;
  string inputFilename, outputFilename;
  bool isDecompress = false;
  for (i = 1; i < argc; i++) {
    if (argv[i] == string("-d")) isDecompress = true;
    else if (inputFilename == "") inputFilename = argv[i];
    else if (outputFilename == "") outputFilename = argv[i];
    else useage(argv[0]);
  }
  if (outputFilename == "") useage(argv[0]);
  if (isDecompress) decompress(inputFilename, outputFilename);
  else compress(inputFilename, outputFilename);
  return 0;
}