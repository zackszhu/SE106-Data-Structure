#include <iostream>
#include <string>
#include "exp.h"
#include "tokenscanner.h"
using namespace std;

Expression *parseExp(TokenScanner &scanner);
Expression *readT(TokenScanner& scanner);
Expression *readE(TokenScanner& scanner, int prec);
int precedence(string token);

Expression *parseExp(TokenScanner &scanner) {
	Expression *exp = readE(scanner, 0);
	if (scanner.hasMoreTokens()) {
		cout << "Error: Found extra token." << endl;
	}
	return exp;
}

Expression *readE(TokenScanner& scanner, int prec) {
	Expression *exp = readT(scanner);
	string token;
	while (true) {
		token = scanner.nextToken();
		int newPrec = precedence(token);
		if (newPrec <= prec)
			break;
		Expression *rhs = readE(scanner, newPrec);
		exp = new OpExp(token, exp, rhs);
	}
	scanner.saveToken(token);
	return exp;
}

Expression *readT(TokenScanner& scanner) {
	string token = scanner.nextToken();
	TokenType type = scanner.getTokenType(token);
	if (type == WORD || type == NUMBER)
		return new NonOpExp(token);
	if (token != "(")
		cout << "Error: Illegal term." << endl;
	Expression *exp = readE(scanner, 0);
	if (scanner.nextToken() != ")") 
		cout << "Error: Missing ')'." << endl;
	return exp;
}

int precedence(string token) {
	if (token == "+" || token == "-") return 1;
	if (token == "*" || token == "/") return 2;
	return 0;
}

int main() {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	while (true) {
		string line;
		getline(cin, line);
		if (line == "quit")
			break;
		scanner.setInput(line);
		Expression *exp = parseExp(scanner);
		cout << exp->toString() << endl;
		delete exp;
	}
}