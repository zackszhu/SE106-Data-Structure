#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

void swap(int& a, int& b) {
	int temp(a);
	a = b;
	b = temp;
}

void reverse(int _left, int _right, vector<int>& _data) {
	while (_left < _right) {
		swap(_data[_left++], _data[_right--]);
	}
}

void rotate(int _left, int _mid, int _right, vector<int>& _data) {
	reverse(_left, _mid - 1, _data);
	reverse(_mid, _right, _data);
	reverse(_left, _right, _data);
}

void merge(int _left, int _mid, int _right, vector<int>& _data) {
	int i(_left), j(_mid + 1);
	while (i < j && j <= _right) {
		while (i < j && _data[i] <= _data[j]) 
			i++;
		int mid(j);
		while (j <= _right && _data[j] < _data[i])
			j++;
		rotate(i, mid, j - 1, _data);
		i += j - mid;
	}
}

void mergeSort(int _left, int _right, vector<int>& _data) {
	if (_left == _right) {
		return;
	}
	int mid((_left + _right) / 2);
	mergeSort(_left, mid, _data);
	mergeSort(mid + 1, _right, _data);
	merge(_left, mid, _right, _data);
}

int main() {
	int n(100000);
	vector<int> data(n);
	srand(time(NULL));
	for (auto it = data.begin(); it != data.end(); it++) {
		*it = (rand() % 1000000);
	}
	mergeSort(0, n - 1, data);
	for (auto it = data.begin(); it != data.end(); it++) {
		cout << *it << endl;
	}
}