#include "dataSystem.h"
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
	DataSystem data;
	data.insertItem("a", "hello");
	data.insertItem("b", "world");
	data.insertItem("x", "merrychristmashahaha");
	data.insertItem("d", "zacks");
	if (!data.deleteItem("l"))
		std::cout << "No this item" << std::endl;
	if (!data.deleteItem("x"))
		std::cout << "No this item" << std::endl;
	std::cout << data.getItem("x") << std::endl;
	data.insertItem("c", "ch");
	std::cout << data.getItem("c") << std::endl;
	data.insertItem("f", "chrismas");
	data.insertItem("g", "xmas");
	// std::cout << data.getItem("c") << std::endl;
	// data.modifyItem("c", "lala");
	// std::cout << data.getItem("c") << std::endl;
	// data.insertItem("h", "xmas");
}