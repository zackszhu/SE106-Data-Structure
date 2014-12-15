#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct test {
	int value;
	test(int a) : value(a) {}
	test() {}
};

int main() {
	auto less = [](const test* a, const test* b)->bool { return a->value < b->value; };
	test* t3;
	{	
		priority_queue<test*, vector<test*>, decltype(less)> pq(less);
		pq.emplace(new test(1));
		pq.emplace(new test(3));
		t3 = pq.top();
		pq.pop();
	}
	cout << t3->value << endl;
}