#include "Vector.h"

int main() {
	Vector<int> test(4);
	test.output();
	test[2] = 3;
	test.output();
	test.resize(5, 9);
	test.output();
}