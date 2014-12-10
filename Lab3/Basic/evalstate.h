#ifndef _evalstate_h
#define _evalstate_h
#include <string>
#include "../StanfordCPPLib/map.h"

class EvalState {
public:
	EvalState();

	~EvalState();

	void setValue(std::string var, int value);

	int getValue(std::string var);

	bool isDefined(std::string var);
	
	void clear();

private:
	Map<std::string,int> symbolTable;
};

#endif