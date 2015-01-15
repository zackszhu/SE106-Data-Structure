#include "dataSystem.h"
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	char charaters[62];
	for (int i = 0; i < 26; i++) {
		charaters[i] = 'a' + i;
		charaters[26 + i] = 'A' + i;
	}
	for (int i = 52; i < 62; i++)
		charaters[i] = '0' + i - 52;
	int num = atoi(argv[1]);
	clock_t t1, t2;
	double total = 0;
	for (int k = 0; k < 100; k++) {
		DataSystem data;
		for (int i = 0; i < num; i++) {
			char name[9] = { charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4],
				charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], 0 };
			char value[10] = {charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4],
							charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], charaters[rand() % 4], 0};
			t1 = clock();
			data.insertItem(name, value);
			t2 = clock();
			total += (t2 - t1);
		}	
	}
	std::cout << total * 10 / CLOCKS_PER_SEC << std::endl;
}