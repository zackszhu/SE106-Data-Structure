#ifndef _B_TREE_
#define _B_TREE_

#include <string>
#include <iostream>
#include <fstream>
#include <memory.h>

#define T 64
#define CACHESIZE 1000
#define KeyType Key

struct Key{
	char name[9];
	Key(const char b[9]) {
		strcpy(name, b);
	}
	Key() {
		memset(name, 0, sizeof(char) * 4);
	}
	Key& operator = (const Key& b);
	bool operator == (const Key& b) const;
	bool operator < (const Key& b) const;
	bool operator > (const Key& b) const;
	bool operator == (Key& b) const;
	bool operator < (Key& b) const;
	bool operator > (Key& b) const;
};

class DataSystem;

class BPTree {
	struct Node {
		unsigned int id;		// id
		unsigned int n;			// number of keys
		int leaf;				// is leaf?
		KeyType key[2*T];	// keys
		unsigned int child[2*T];		// id of children when not leaf

		Node();
		Node(unsigned int _id, unsigned int _n, int _leaf);
		Node(const Node& _that);
	};

	Node* root;
	unsigned int rootID;		// where the root is in index
	unsigned int num;			// total nodes
	std::string indexName;		// index file name
	std::fstream file;
	int smallest;				// smallest leaf id

	/* Read&Write
	 * 
	 * Read the node according to the id
	 * and return the struct value back to node
	 * 
	 * Write the node according to the id
	 * write the value into index file in binary
	 * 
	 */
	void readNodeFromFile(Node* node, unsigned int id);
	void writeNodeToFile(Node* node);

	void readNodeFromCache(Node* node, unsigned int id);
	void writeNodeToCache(Node* node);

	/* Split&Insert
	 * 
	 * Splite the node into two parts
	 * and insert the head of the second parts into parent node
	 * 
	 * Insert the node if this node is not full
	 * pre-garentee that it's not full
	 *
	 */

	void splitNode(Node* _p, Node* _c, int _i);	// splite node and insert into parent node
	void insertNotFull(Node* _n, const KeyType& _key, unsigned int _id, unsigned _index);

	// Delete functions
	void replaceKey(const KeyType& _old, const KeyType& _new);
	void adjustToDel(Node* _p, Node* _x, unsigned int _i);
	unsigned int delKey(Node* _p, const KeyType& _key);

	Node indexCache[CACHESIZE];

public:

	BPTree(const std::string& filename, char _nFile);				// read the root and num from index
	~BPTree();														// write the root info

	unsigned int search(const KeyType& _key);
	void insertKey(const KeyType& _key, unsigned int _index);
	unsigned int deleteKey(const KeyType& _key);


};	

#endif