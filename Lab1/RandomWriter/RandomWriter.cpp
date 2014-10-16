/*
 * File: RandomWriter.cpp
 * ----------------------
 * [TODO: fill in your name and student ID]
 * Name: Zhu Chaojie
 * Student ID: 5130379013 
 * This file is the starter project for the random writer problem.
 * [TODO: extend the documentation]
 */

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "console.h"
#include "random.h"
#include "vector.h"
#include "filelib.h"
using namespace std;

class Info {
public:
	int total;
	map<char, int> times;
	Info(){}
	Info(char ch) {
		times[ch] = 1;
		total = 1;
	}
	void add(char ch) {
		total++;
		if (times.find(ch) == times.end()) {
			times[ch] = 1;
		}
		else
			times[ch]++;
	}
	char getNext() {
		int random = randomInteger(1, total);
		int i = 0;
		for (map<char, int>::iterator iter = times.begin(); iter != times.end(); iter++) {
			random -= iter->second;
			if (random <= 0) {
				return iter->first;
			}
		}
	}
};

int stringIntoInteger(string &str) {
	istringstream stream(str);
   	int value;
   	stream >> value >> ws;
   	if (stream.fail() || !stream.eof()) 
      	return -1;
   return value;
}

int loadNum() {
	int num = -1;
	while (num < 1 || num > 10) {
		cout << "Enter the Markov order [0-10]: ";
		string numStr;
		cin >> numStr;
		num = stringIntoInteger(numStr);
	}
	return num;
}

string initModel(map<string, Info> &model, int &length) {
	int max(0);
	ifstream input;
	string filename, maxStr;
	do {
		cout << "Enter the source text: ";
		cin >> filename;
		input.open(filename.c_str());
	}
	while (!input.is_open());
	length = loadNum();
	char ch;
	string current("");
	while (current.length() != length){
		input.get(ch);
		current += ch;
	}
	while (input.get(ch)) {
		if (model.find(current) == model.end())
			model[current] = Info(ch);
		else
			model[current].add(ch);
		if (model[current].total > max) {
			max = model[current].total;
			maxStr = current;
		}
		current = current.substr(1) + ch;
	}
	return maxStr;
}

int main() {
	map<string, Info> model;
	int length;
	string current(initModel(model, length));
	cout << current;
	do {
		char newChar(model[current].getNext());
		cout << newChar;
		current = current.substr(1) + newChar;
		length++;
	}
	while (length < 2000);
	// for (map<string, Info>::iterator it = model.begin(); it != model.end(); it++) {
	// 	cout << it->first << endl;
	// 	for (map<char, int>::iterator iter = it->second.times.begin(); iter != it->second.times.end(); iter++) {
	// 		cout << iter->first << " " << iter->second << endl;
	// 	}
	// }
	cout << endl;
	return 0;
}
