#include "compiler.h"
#include <iostream>
using namespace std;

int main() {
	Compiler basicCompiler;
	while (basicCompiler.parseLine(cin)) {}
}