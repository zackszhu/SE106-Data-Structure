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
#include "foreach.h"
using namespace std;

void dictInit(string _dict[100002], int &_dictLength, int length) {
	Lexicon dict("EnglishWords.dat");
	foreach (string word in dict) {
		if (word.length() == length){
			_dictLength++;
			_dict[_dictLength] = word;
		}
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

void getLadder(int ptr, string words[100002], int father[100002]) {
	string ladder[100002];
	int l;
	while (ptr != -1) {
		l++;
		ladder[l] = words[ptr];
		ptr = father[ptr];
	}
	for (int i = l; i > 0; i--)
		cout << ladder[i] << " ";
}

int main() {
	while (1){
		string start, destination;
		cout << "Enter start word (RETURN to quit): ";
		char ch;
		cin.get(ch);
		if (ch == '\n'){
			cout << "Goodbye!" << endl;
			return 0;
		}
		cin >> start;

		start = ch + start;

		cout << "Enter destination word: ";
		cin >> destination;
		string wordsQueue[100002], dict[100002];
		int father[100002];
		int qLength(0), dLength(0), fLength(0);
		bool wordsAppear[100002];
		memset(wordsAppear, 0, sizeof(wordsAppear));

		qLength++;
		wordsQueue[1] = start;
		fLength++;
		father[1] = -1;
		dictInit(dict, dLength, start.length());
		int head(1);

		while (head <= qLength) {
			bool found(false);
			for (int i = 1; i <= dLength; i++) {
				if (!wordsAppear[i])
					if (diff(wordsQueue[head], dict[i]) == 1) {
						qLength++;
						wordsQueue[qLength] = dict[i];
						fLength++;
						father[fLength] = head;
						wordsAppear[i] = true;
						if (dict[i] == destination){
							getLadder(qLength, wordsQueue, father);
							found = true;
							break;
						}
					}

			}
			if (found)
				break;
			head++;
		}

		cout << endl;
		cin.ignore(1024, '\n');
	}
 	return 0;
}
