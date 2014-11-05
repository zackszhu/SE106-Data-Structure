#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int hashKey(char ch) {
	return ch;
}

int main() {
	int n;
	ifstream fin("hash.in");
	ofstream fout("hash.out");
	fin >> n;
	fin.ignore(1024, '\n');
	for (int times = 0; times < n; times++) {
		int hashTable[256] = {0};
		string source;
		getline(fin, source);
		for (string::iterator i = source.begin(); i != source.end(); i++)
			hashTable[hashKey(*i) - 1]++;
		for (string::iterator i = source.begin(); i != source.end(); i++) {
			if (hashTable[hashKey(*i) - 1] == 1) {
				fout << *i << endl;
				break;
			}
			if ((i + 1) == source.end())
				fout << "None" << endl;
		}

	}
}