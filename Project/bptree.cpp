#include "bptree.h"
#include "string.h"
using namespace std;

// operator overload of Key
Key& Key::operator = (const Key& b){
	strcpy(name, b.name);
	return *this;
}
bool Key::operator==(const Key& b) const {
	return (strcmp(name, b.name) == 0);
}
bool Key::operator<(const Key& b) const {
	return (strcmp(name, b.name) < 0);
}
bool Key::operator>(const Key& b) const {
	return (strcmp(name, b.name) > 0);
}
bool Key::operator==(Key& b) const {
	return (strcmp(name, b.name) == 0);
}
bool Key::operator<(Key& b) const {
	return (strcmp(name, b.name) < 0);
}
bool Key::operator>(Key& b) const {
	return (strcmp(name, b.name) > 0);
}


void BPTree::readNodeFromFile(Node* node, unsigned int id) {
	unsigned int tmp = sizeof(Node) * (id - 1) + 2 * sizeof(int);
	file.seekg(sizeof(Node) * (id - 1) + 2 * sizeof(int));
	file.read((char*)node, sizeof(Node));
}

void BPTree::writeNodeToFile(Node* node) {
	unsigned int tmp = sizeof(Node) * (node->id - 1) + 2 * sizeof(int);
	file.seekp(sizeof(Node) * (node->id - 1) + 2 * sizeof(int), file.beg);
	file.write((char*)node, sizeof(Node));
}

void BPTree::readNodeFromCache(Node* node, unsigned int id) {
	if (id == indexCache[(id >> 2) % CACHESIZE].id)
		memcpy(node, &indexCache[(id >> 2)  % CACHESIZE], sizeof(Node));
	else {
		readNodeFromFile(node, id);
		writeNodeToCache(node);
	}
}

void BPTree::writeNodeToCache(Node* node) {
	if (indexCache[(node->id >> 2)  % CACHESIZE].id > 0 && node->id != indexCache[(node->id >> 2) % CACHESIZE].id)
		writeNodeToFile(&indexCache[(node->id >> 2) % CACHESIZE]);
	memcpy(&indexCache[(node->id >> 2) % CACHESIZE], node, sizeof(Node));
}

BPTree::Node::Node() : id(0), n(0), leaf(0) {
	memset(key, 0, sizeof(Key) * 2 * T);
	memset(child, 0, sizeof(int) * 2 * T);
}

BPTree::Node::Node(unsigned int _id, unsigned int _n, int _leaf) 
: id(_id), n(_n), leaf(_leaf) {
	memset(key, 0, sizeof(Key) * 2 * T);
	memset(child, 0, sizeof(int) * 2 * T);
}

BPTree::Node::Node(const Node& _that)
: id(_that.id), n(_that.n), leaf(_that.leaf) {
	memcpy(key, _that.key, sizeof(KeyType) * 2 * T);
	memcpy(child, _that.child, sizeof(int) * 2 * T);
}

BPTree::BPTree(const string& filename, char _nFile) 
: 	indexName(filename), smallest(1) {
	if (_nFile == 'r') {
		file.open(filename.c_str(), ios::out | ios::in | ios::binary);
		file.seekg(file.beg);
		file.read((char*)&num, sizeof(int));
		file.read((char*)&rootID, sizeof(int));
		root = new Node();
		readNodeFromCache(root, rootID);
	}
	else {
		file.open(filename.c_str(), ios::out | ios::in | ios::binary | ios::trunc);
		root = new Node(1, 0, 1);
		rootID = 1; num = 0; 
		file.write((char*)&num, sizeof(int));
		file.write((char*)&rootID, sizeof(int));
		writeNodeToCache(root);
	}
}

BPTree::~BPTree() {
	file.seekp(0, file.beg);
	file.write((char*)&num, sizeof(int));
	file.write((char*)&rootID, sizeof(int));
	for (int i = 0; i < CACHESIZE; i++) {
		if (indexCache[i].id != 0)
			writeNodeToFile(&indexCache[i]);
	}
	delete root;
	file.close();
}

void BPTree::splitNode(Node* _p, Node* _c, int _i) {
	Node* b = new Node(++num, T, _c->leaf);

	// copy the back part of c to b
	for (int j = 0; j < T; j++) {
		b->key[j] = _c->key[j + T];
		b->child[j] = _c->child[j + T];
	}

	// modify c info
	_c->n = T;

	// move the nodes in p backward
	for (int j = _p->n - 1; j > _i; j--) {
		_p->key[j + 1] = _p->key[j];
		_p->child[j + 1] = _p->child[j];
	}

	// insert b into p
	_p->key[_i + 1] = b->key[0];
	_p->child[_i + 1] = b->id;
	++_p->n;

	//write nodes
	writeNodeToCache(_p);
	writeNodeToCache(_c);
	writeNodeToCache(b);

	delete b;
}

void BPTree::insertNotFull(Node* _n, const KeyType& _key, unsigned int _id, unsigned int _index) {
	if (_n->leaf) {
		// if _n is leaf, find the position to insert
		int i = _n->n - 1;
		for (; i >= 0 && _n->key[i] > _key; i--) {
			_n->key[i + 1] = _n->key[i];
			_n->child[i + 1] = _n->child[i];
		}
		_n->key[i + 1] = _key;
		_n->child[i + 1] = _index;
		_n->n++;
		writeNodeToCache(_n);
	}
	else {
		// if _n is not a leaf, find the place to recursion
		int i = _n->n - 1;
		for (; i >= 0 && _n->key[i] > _key; i--) {}
		if (i < 0) {
			// smallest
			i++;
			_n->key[i] = _key;
			writeNodeToCache(_n);
		}

		Node* tmp = new Node();
		readNodeFromCache(tmp, _n->child[i]);
		if (tmp->n == 2 * T) {
			// full
			splitNode(_n, tmp, i);
			if (_key > _n->key[i + 1]) 
				i++;
			readNodeFromCache(tmp, _n->child[i]);
		}

		insertNotFull(tmp, _key, _id, _index);

		delete tmp;
	}
}

unsigned int BPTree::search(const KeyType& _key) {
	Node* r = root;
	if (_key < r->key[0]) {
		// smallest
		return 1;
	}
	Node* tmp = new Node();
	int i;
	while (1) {
		i = r->n - 1;
		for (; i >= 0 && r->key[i] > _key; i--) {}
		if (r->leaf)
			break;
		readNodeFromCache(tmp, r->child[i]);
		r = tmp;
	}

	if (r->key[i] < _key) {
		// not find
		delete tmp;
		return 1;
	}

	int ret;
	ret = r->child[i];
	delete tmp;
	return ret;
}

void BPTree::insertKey(const KeyType& _key, unsigned int _index) {
	Node* r = root;

	if (search(_key) != 1)
		// already exist
		return;

	if (root->n == 2 * T) {
		// root is full, re-allocate
		Node* s = new Node(++num, 1, 0);
		s->key[0] = root->key[0];
		s->child[0] = root->id;

		writeNodeToCache(s);
		splitNode(s, r, 0);

		// point root to new root s
		memcpy(root, s, sizeof(Node));
		rootID = s->id;

		insertNotFull(s, _key, ++num, _index);
		delete s;
	}
	else 
		insertNotFull(r, _key, ++num, _index);
}

void BPTree::replaceKey(const KeyType& _old, const KeyType& _new) {
	Node* r = root;
	Node* tmp = new Node();

	while (1) {
		int i = r->n - 1;
		// find position
		while (i >= 0 && r->key[i] > _old) 
			i--;
		if (r->key[i] == _old) {
			r->key[i] = _new;
			writeNodeToCache(r);
		}

		if (r->leaf)
			break;

		readNodeFromCache(tmp, r->child[i]);
		r = tmp;
	}
	delete tmp;
}

void BPTree::adjustToDel(Node* _p, Node* _x, unsigned int _i) {
	Node *left, *right;
	Node* tmp = new Node();

	if (_i > 0) {
		// have left brother, borrow one child
		readNodeFromCache(tmp, _p->child[_i - 1]);
		left = tmp;
		if (left->n > T) {
			for (int j = _x->n; j > 0; j--) {
				_x->key[j] = _x->key[j - 1];
				_x->child[j] = _x->child[j - 1];
			}
			_x->n++;
			_x->key[0] = left->key[left->n - 1];
			_x->child[0] = left->child[left->n - 1];
			writeNodeToCache(_x);

			left->n--;
			writeNodeToCache(left);

			_p->key[_i] = _x->key[0];
			writeNodeToCache(_p);

			delete tmp;
			return;
		}
	}

	if (_i < _p->n - 1) {
		// have right brother, borrow one
		readNodeFromCache(tmp, _p->child[_i + 1]);
		right = tmp;
		left = NULL;
		if (right->n > T) {
			_x->key[_x->n] = right->key[0];
			_x->child[_x->n] = right->child[0];
			_x->n++;
			writeNodeToCache(_x);

			for (int j = 0; j < right->n - 1; j++) {
				right->key[j] = right->key[j + 1];
				right->child[j] = right->child[j + 1];
			}
			right->n--;
			writeNodeToCache(right);

			_p->key[_i + 1] = right->key[0];
			writeNodeToCache(_p);

			delete tmp;
			return;
		}
	}

	// merge with left or right
	if (left == tmp) {
		for (int j = 0; j < _x->n; j++) {
			left->key[left->n + j] = _x->key[j];
			left->child[left->n + j] = _x->child[j];
		}
		left->n += _x->n;
		writeNodeToCache(_x);
		// remove from parent
		for (int j = _i; j < _p->n - 1; j++) {
			_p->key[j] = _p->key[j + 1];
			_p->child[j] = _p->child[j + 1];
		}
		_p->n--;
		writeNodeToCache(_p);
		memcpy(_x, left, sizeof(Node));
	}
	else {
		for (int j = 0; j < right->n; j++) {
			_x->key[_x->n + j] = right->key[j];
			_x->child[_x->n + j] = right->child[j];
		}
		_x->n += right->n;
		writeNodeToCache(_x);
		// remove from parent
		for (int j = _i + 1; j < _p->n; j++) {
			_p->key[j] = _p->key[j + 1];
			_p->child[j] = _p->child[j + 1];
		}
		_p->n--;
		writeNodeToCache(_p);
	}

	delete tmp;
}

unsigned int BPTree::delKey(Node* _p, const KeyType& _key) {
	// start from _p, delete every k
	// promise: can be directly delete validly
	int i = 0;
	for (; _p->key[i] < _key && i < _p->n - 1; i++) {}
	unsigned int pos = _p->child[i];

	if (_p->leaf) {
		// leaf
		for (int j = i; j< _p->n - 1; j++) {
			_p->key[j] = _p->key[j + 1];
			_p->child[j] = _p->child[j + 1];
		}
		_p->n--;
		writeNodeToCache(_p);

		if (i == 0) {
			// min number
			replaceKey(_key, _p->key[i]);
		}

		return pos;
	}

	Node* x = new Node();
	readNodeFromCache(x, _p->child[i]);
	unsigned int ret;
	if (x->n > T)
		ret = delKey(x, _key);
	else {
		adjustToDel(_p, x, i);
		ret = delKey(x, _key);
	}
	delete x;
	return ret;
}

unsigned int BPTree::deleteKey(const KeyType& _key) {
	if (search(_key) == 1)
		return 1;

	Node* r = root;
	unsigned int pos = delKey(r, _key);

	if (r->n == 1) {
		rootID = r->child[0];
		readNodeFromCache(root, r->child[0]);
		num--;
	}

	return pos;
}
