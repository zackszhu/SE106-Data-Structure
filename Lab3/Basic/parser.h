#include <iostream>
#include <string>
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

Expression *parseExp(TokenScanner & scanner);

Expression *readE(TokenScanner & scanner, int prec);

Expression *readT(TokenScanner & scanner);

int precedence(string token);