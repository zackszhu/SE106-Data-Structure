#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "../StanfordCPPLib/tokenscanner.h"
#include <map>
#include <string>
#include <cstdlib>
#include "evalstate.h"
using namespace std;

class Compiler {
public:
	Compiler();

	int parseLine(istream& is);

	bool execute(const string& line);
	bool executeBuiltIn(const string& line);
	
private:
	string line;
	map<int, string> program;
	EvalState variables;
	map<int, string>::iterator programPointer;

	void outputProgram();
	string upperCase(const string& str);
	void runProgram();
};

#endif