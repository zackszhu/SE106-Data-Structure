#include <string>
#include "evalstate.h"
#include "../StanfordCPPLib/map.h"

using namespace std;

EvalState::EvalState() {}
EvalState::~EvalState() {}

void EvalState::setValue(string var, int value) {
	symbolTable.put(var, value);
}

int EvalState::getValue(string var) {
	return symbolTable.get(var);
}

bool EvalState::isDefined(string var) {
	return symbolTable.containsKey(var);
}

void EvalState::clear() {
	symbolTable.clear();
}
