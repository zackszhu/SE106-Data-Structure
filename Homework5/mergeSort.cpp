#include <iostream>
#include <vector>
using namespace std;

void merge(int _left, int _mid, int _right, vector<int>& _data) {
	int head(_left), tail(_mid + 1), now(_left - 1);
	vector<int> temp(_data);
	while (head <= _mid &&  tail <= _right) {
		now++;
		if (temp[head] < temp[tail]) {
			_data[now] = temp[head];
			head++;
		}
		else {
			_data[now] = temp[tail];
			tail++;
		}
	}
		if (head <= _mid) {
			for (int i = head; i <= _mid; i++) {
				++now;
				_data[now] = temp[i];
			}
		}
		if (tail <= _right) {
			for (int i = tail; i <= _right; i++) {
				++now;
				_data[now] = temp[i];
			}	
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