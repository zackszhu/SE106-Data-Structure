/*
 * File: WordLadder.cpp
 * --------------------
 * [TODO: fill in your name and student ID]
 * Name: Zhu Chaojie
 * Student ID: 5130379013
 * This file is he starter project for the word ladder problem.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "lexicon.h"
#include "vector.h"
#include "set.h"
#include "foreach.h"
using namespace std;

void dictInit(Vector<string>& _dict, int length) {
	Lexicon dict("EnglishWords.dat");
	foreach (string word in dict) {
		if (word.length() == length)
			_dict.add(word);
	}
}

int diff(const string& str1, const string& str2) {
	if (str1.length() != str2.length())
		return -1;
	int diffNum(0);
	for (int i = 0; i < str1.length(); i++)
		if (str1[i] != str2[i])
			diffNum++;
	return diffNum;
}

void getLadder(int ptr, const Vector<string> &words, const Vector<int> &father) {
	Vector<string> ladder;
	while (ptr != -1) {
		ladder.add(words[ptr]);
		ptr = father[ptr];
	}
	for (int i = ladder.size() - 1; i >= 0; i--)
		cout << ladder[i] << " ";
}

int main() {
	string start, destination;
	cout << "Start?";
	cin >> start;
	cout << "End?";
	cin >> destination;
	Vector<string> wordsQueue, dict;
	Vector<int> father;
	Set<string> wordsAppear;

	wordsQueue.add(start);
	father.add(-1);
	wordsAppear.add(start);
	dictInit(dict, start.length());
	int head(0), dictSize(dict.size());

	while (head != wordsQueue.size()) {
		if (wordsQueue[head] == destination){
			getLadder(head, wordsQueue, father);
			break;
		}
		for (int i = 0; i < dictSize; i++) {
			if (!wordsAppear.contains(dict[i]))
				if (diff(wordsQueue[head], dict[i]) == 1) {
					wordsQueue.add(dict[i]);
					father.add(head);
					wordsAppear.add(dict[i]);
				}
		}
		head++;
	}

	cout << endl;

 	return 0;
}
