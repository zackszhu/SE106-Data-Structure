#include <iostream>
#include <string>
#include "../StanfordCPPLib/error.h"
#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

Expression *parseExp(TokenScanner & scanner) {
	Expression *exp = readE(scanner, 0);
	if (scanner.hasMoreTokens()) {
		error("parseExp: Found extra token: " + scanner.nextToken());
	}
	return exp;
}

Expression *readE(TokenScanner & scanner, int prec) {
	Expression *exp = readT(scanner);
	string token;
	while (true) {
		token = scanner.nextToken();
		int newPrec = precedence(token);
		if (newPrec <= prec) break;
		Expression *rhs = readE(scanner, newPrec);
		exp = new CompoundExp(token, exp, rhs);
	}
	scanner.saveToken(token);
	return exp;
}

Expression *readT(TokenScanner & scanner) {
	int sign = 1;
	string token = scanner.nextToken();
	TokenType type = scanner.getTokenType(token);
	if (type == WORD) return new IdentifierExp(token);
	if (type == NUMBER) return new ConstantExp(stringToInteger(token));
	if (token != "(" && token!= "-") error("Illegal term in expression");
	Expression *exp = readE(scanner, 0);
	if (token == "-")
		exp->setMinus();
	if (scanner.nextToken() != ")" && token!= "-") {
		error("Unbalanced parentheses in expression");
	}
	return exp;
}

int precedence(string token) {
	if (token == "=") return 1;
	if (token == "+" || token == "-") return 2;
	if (token == "*" || token == "/") return 3;
	return 0;
}