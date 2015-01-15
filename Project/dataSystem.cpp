#include "dataSystem.h"
using namespace std;

#define MAXINT (unsigned int)~0 >> 1

DataSystem::IdleItem::IdleItem() : pos(0), next(NULL), length(MAXINT) {}

DataSystem::IdleItem::IdleItem(unsigned int _pos, int _length) : pos(_pos), next(NULL), length(_length) {}

DataSystem::DataSystem(char _nFile)
: indexTree("index.txt", _nFile), idleHead(new IdleItem), dataEnd(0), pageNum(0) {
	if (_nFile == 'r') {
		fd = open("data.txt", O_RDWR);
		dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		idleFile.open("idle.txt", ios::binary | ios::in);
		readIdle();
	}
	else {
		fd = open("data.txt", O_RDWR | O_TRUNC | O_CREAT);
		ftruncate(fd, 4096);
		dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		idleFile.open("idle.txt", ios::binary | ios::out | ios::trunc);
	}
}

void DataSystem::insertItem(const KeyType& _key, const std::string& _value) {
	// first find if it has been in this tree
	if (indexTree.search(_key) != 1)
		return;
	// find where to insert
	int length = _value.length();  
	IdleItem* index = idleHead;
	IdleItem* indexPrev = index;
	while (index->length - (int)sizeof(int) < length) {
		indexPrev = index;
		index = index->next;
	}
	if (index->pos + sizeof(length) + sizeof(char) * length >= 4096 + pageNum * 4096) {
		// add new page
		ftruncate(fd, 4096 + ++pageNum * 4096);
		index->pos = pageNum * 4096;
		munmap(dataFile, 4096);
		dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, pageNum * 4096);
	}
	else if (index->pos / 4096 != pageNum) {
		// change to next page
		munmap(dataFile, 4096);
		dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, ((unsigned int)index->pos >> 12) * 4096);
	}

	memcpy(&dataFile[index->pos % 4096], &length, sizeof(length));
	memcpy((dataFile + index->pos % 4096 + sizeof(length)), _value.c_str(), sizeof(char) * length);
	
	indexTree.insertKey(_key, index->pos);

	// record the tail of data
	if (index->pos + sizeof(length) + sizeof(char) * length > dataEnd)
		dataEnd = index->pos + sizeof(length) + sizeof(char) * length;

	// change idle list
	if (index->length == MAXINT)
		index->pos = dataEnd;
	else {
		int tmp = index->length - length - (int)sizeof(int);
		// remove index from list
		if (index == idleHead)
			idleHead = idleHead->next;
		else
			indexPrev->next = index->next;
		
		if (tmp > 0) {
			// modify index
			index->pos += sizeof(int) + length;
			index->length = tmp;
			memcpy(&dataFile[index->pos % 4096], &tmp, sizeof(length));
			// dataFile.write((char*) &tmp, sizeof(int));
			// find one just smaller than index
			IdleItem* tmpIdle = idleHead;
			while (tmpIdle->length < tmp && tmpIdle->next->length < tmp)
				tmpIdle = tmpIdle->next;
			// insert
			if (tmpIdle->length > tmp) {
				index->next = idleHead;
				idleHead = index;
			}
			else {
				index->next = tmpIdle->next;
				tmpIdle->next = index;
			}
		}
	}
	
}

void DataSystem::deleteList(IdleItem* item) {
	// write idlelist and delete it recursively
	idleFile.write((char*)&(item->pos), sizeof(int));
	if (item->next != NULL)
		deleteList(item->next);
	delete item;
}

DataSystem::~DataSystem() {
	idleFile.seekp(0);
	deleteList(idleHead);
	close(fd);
	munmap((void*)dataFile, 4096);
	idleFile.close();
}

string DataSystem::getItem(const KeyType& _key) {
	unsigned int pos = indexTree.search(_key);
	if (pos == 1)
		return "Not Found.";

	// get dataFile page
	munmap(dataFile, 4096);
	dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, ((unsigned int)pos >> 12) * 4096);

	unsigned int length = *((int*)(dataFile + pos % 4096));
	char* value = new char[length + 1];

	memcpy(value, dataFile + pos % 4096 + sizeof(length), length);
	value[length] = '\0';

	string ret(value);
	delete value;
	return ret;
}

bool DataSystem::deleteItem(const KeyType& _key) {
	unsigned int pos = indexTree.deleteKey(_key);
	if (pos == 1) {
		return false;
	}

	// get dataFile page
	munmap(dataFile, 4096);
	dataFile = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, ((unsigned int)pos >> 12) * 4096);

	unsigned int length = *((int*)(dataFile + pos % 4096));

	// add new idle item
	IdleItem* item = new IdleItem(pos, length);

	// find just small
	IdleItem* tmpIdle = idleHead;
	while (tmpIdle->length < length && tmpIdle->next->length < length)
		tmpIdle = tmpIdle->next;
	// insert
	if (tmpIdle->length > length) {
		item->next = idleHead;
		idleHead = item;
	}
	else {
		item->next = tmpIdle->next;
		tmpIdle->next = item;
	}
	return true;
}

void DataSystem::readIdle() {
	IdleItem* indexPrev;
	IdleItem* item = new IdleItem();
	while (idleFile.read((char*) &(item->pos), sizeof(int))) {
		memcpy(&(item->length), dataFile + item->pos, sizeof(int));
		if (item->pos > dataEnd)
			dataEnd = item->pos;
		if (item->length == 0)
			item->length = MAXINT;
		if (idleHead->pos == 0)
			idleHead = item;
		else 
			indexPrev->next = item;
		indexPrev = item;
		item = new IdleItem();
	}
	idleFile.close();
	idleFile.open("idle.txt", ios::binary | ios::out | ios::trunc);
	delete item;
}

void DataSystem::modifyItem(const KeyType& _key, const std::string& _new) {
	deleteItem(_key);
	insertItem(_key, _new);
}