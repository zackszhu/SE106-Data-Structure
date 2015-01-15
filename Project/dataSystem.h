#ifndef _DATA_SYSTEM_H_
#define _DATA_SYSTEM_H_

#include "bptree.h"
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fstream>
#include <string>

class DataSystem {
	BPTree indexTree;
	char* dataFile;
	std::fstream idleFile;

	struct IdleItem {
		unsigned int pos;
		int length;
		IdleItem* next;
		IdleItem();
		IdleItem(unsigned int _pos, int _length);
	};

	IdleItem* idleHead;

	void deleteList(IdleItem* item);

	unsigned int dataEnd;
	unsigned int pageNum;
	int fd;

	void readIdle();

public:
	DataSystem(char _nFile = 'n');
	~DataSystem();
	void insertItem(const KeyType& _key, const std::string& _value);
	std::string getItem(const KeyType& _key);
	bool deleteItem(const KeyType& _key);
	void modifyItem(const KeyType& _key, const std::string& _new);
};

#endif