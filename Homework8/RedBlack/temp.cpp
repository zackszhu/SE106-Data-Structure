#include <iostream>
using namespace std;

struct node {
	node* next;
	int data;
};

node*& next(node* a) {
	return a->next;
}

int main() {
	node *a, *b, *c;
	a = new node;
	b = new node;
	c = new node;
	c->data = 1;
	a->next = c;
	a->data = 0;
	b->data = 2;
	cout << a->next->data << endl;
	node*& d = next(a);
	d = b;
	cout << a->next->data << endl;
}