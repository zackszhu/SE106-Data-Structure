#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

class Huffman {
public:
	Huffman() : heapSize(256){}
	void add(char ch) {
		int index = int(ch);
		nodes[index].value = ch;
		nodes[index].time++;
	}

	void buildHuffman() {
		for (int i = 0; i < 255; i++)
			if (nodes[i].time == 0) {
				nodes[i].time = ~0;
				heapSize--;
			}
		heap();
		swap(nodes[0], nodes[--heapSize]);
		Node* left = nodes + heapSize;
		down(0);
		swap(nodes[0], nodes[--heapSize]);
		Node* right = nodes + heapSize;
		down(0);
		Node* parent = new Node(left->time + right->time, left, right);
	}
private:
	struct Node {
		bool leaf;
		char value;
		unsigned int time;
		Node* left;
		Node* right;
		Node* parent;
		Node(Node* _left, Node* _right)
		: leaf(false), value(''), parent(NULL) {
			left = _left;
			right = _right;
			time = _left->time + _right->time;
		}
		Node() 
		: leaf(true), value(''), time(0), left(NULL), right(NULL), parent(NULL) {}
	};
	Node nodes[256];
	int heapSize;

	void swap(Node& a, Node& b) {
		Node temp = a;
		a = b;
		b = temp;
	}

	void down(int index) {
		int smallIndex = nodes[index * 2].time < nodes[index * 2 + 1].time ? index * 2 : index * 2 + 1;
		while (a[index].time > a[smallIndex].time) {
			Node temp = a[smallIndex];
			a[smallIndex] = a[index];
			a[index] = temp;
			index = smallIndex;
			if (index <= heapSize / 2)
				smallIndex = nodes[index * 2].time < nodes[index * 2 + 1].time ? index * 2 : index * 2 + 1;
			else
				break;
		}
	}

	void heap() {
		for (int i = heapSize / 2; i >= 0; i--) {
			down(i);
		}
	}


};

#endif