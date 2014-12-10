#include "compiler.h"
#include "parser.h"
#include "exp.h"
#include "../StanfordCPPLib/error.h"

Compiler::Compiler() {
	program[0] = "";
}

int Compiler::parseLine(istream& is) {
	try {
		TokenScanner scanner;
		scanner.ignoreWhitespace();
		scanner.scanNumbers();
		getline(is, line);
		scanner.setInput(line + " ");
		string firstToken = scanner.nextToken();
		if (scanner.getTokenType(firstToken) == NUMBER) {
			// save program
			if (scanner.hasMoreTokens()){
				int i = scanner.getPosition();
				// while (line[++i] == ' ') {}
				program[atoi(firstToken.c_str())] = line.substr(i);
			}
			else
				program[atoi(firstToken.c_str())] = "";
		}
		else if (scanner.hasMoreTokens()) {
			return execute(upperCase(line));
		}
		else {
			return executeBuiltIn(upperCase(firstToken));
		}
	}
	catch (ErrorException& e) {
		cout << e.getMessage() << endl;
	}
	return 1;
}

bool Compiler::execute(const string& line) {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firstToken = scanner.nextToken();
	if (firstToken == "REM") {
		return 1;
	}
	if (firstToken == "LET") {
		Expression *exp = parseExp(scanner);
		int value = exp->eval(variables);
		return 1;
	}
	if (firstToken == "PRINT") {
		Expression *exp = parseExp(scanner);
		int value = exp->eval(variables);
		cout << value << endl;
		return 1;
	}
	if (firstToken == "INPUT") {
		string variable = scanner.nextToken();
		int flag = 1;
		string tempLine;
		while (true) {
			try {
				cout << " ? ";
				getline(cin, tempLine);
				TokenScanner tempScanner(tempLine);
				tempLine = tempScanner.nextToken();
				if (tempLine == "-"){
					flag = -1;
					tempLine = tempScanner.nextToken();
				}
				if (tempScanner.hasMoreTokens() || tempScanner.getTokenType(tempLine) != NUMBER)
					error("INVALID NUMBER");
				for (auto it = tempLine.begin(); it != tempLine.end(); it++)
					if (*it < '0' || *it > '9')
						error("INVALID NUMBER");
				break;
			}
			catch (ErrorException& e) {
				cout << e.getMessage() << endl;
			}
		}
		variables.setValue(variable, flag * atoi(tempLine.c_str()));
		return 1;
	}
	if (firstToken == "IF") {
		string tempExpression;
		string temp;
		while ((temp = scanner.nextToken()) != "=" && temp != ">" && temp != "<" && scanner.hasMoreTokens()) {
			tempExpression += temp;
		}
		TokenScanner tempScanner(tempExpression);
		Expression *exp = parseExp(tempScanner);
		int leftValue = exp->eval(variables);
		char op = temp[0];
		tempExpression = "";
		while ((temp = scanner.nextToken()) != "THEN") {
			tempExpression += temp;
		}
		tempScanner.setInput(tempExpression);
		exp = parseExp(tempScanner);
		int rightValue = exp->eval(variables);
		// cout << "leftValue" << leftValue << "rightValue" << rightValue << endl;
		switch(op) {
			case '>': 	if (!(leftValue > rightValue)) return 1;
						break;
			case '<': 	if (!(leftValue < rightValue)) return 1;
						break;
			case '=': 	if (!(leftValue == rightValue)) return 1;
						break;
		}
		firstToken = "GOTO";
	}
	if (firstToken == "GOTO") {
		string tempLine = scanner.nextToken();
		if (scanner.hasMoreTokens() || scanner.getTokenType(tempLine) != NUMBER)
			error("INVALID NUMBER");
		for (auto it = tempLine.begin(); it != tempLine.end(); it++)
			if (*it < '0' || *it > '9')
				error("INVALID NUMBER");
		auto tempIterator = program.find(atoi(tempLine.c_str()));
		if (tempIterator == program.end() || tempIterator->second == "")
			error("LINE NUMBER ERROR");
		programPointer = --tempIterator;
		return 1;
	}
	error("SYNTAX ERROR");
	return 1;
}

bool Compiler::executeBuiltIn(const string& line) {
	if (line == "HELP") {
		cout << "http://tcloud.sjtu.edu.cn/course/se106/2014/assignments/lab3/index.php" << endl;
		return 1;
	}
	if (line == "QUIT") {
		return 0;
	}
	if (line == "LIST") {
		outputProgram();
		return 1;
	}
	if (line == "CLEAR") {
		program.clear();
		variables.clear();
		program[0] = "";
		return 1;
	}
	if (line == "RUN") {
		runProgram();
		return 1;
	}
	error("SYNTAX ERROR");
	return 1;
}

void Compiler::outputProgram() {
	for (auto it = ++program.begin(); it != program.end(); it++) {
		if (it->second != "")
			cout << it->first << " " << it->second << endl;
	}
}

string Compiler::upperCase(const string& str) {
	string ret;
	for (auto it = str.begin(); it != str.end(); it++) {
		if (*it >= 'a' && *it <= 'z')
			ret += 'A' + *it - 'a';
		else
			ret += *it;
	}
	return ret;
}

void Compiler::runProgram() {
	programPointer = ++program.begin();
	while (programPointer != program.end() && upperCase(programPointer->second) != "END") {
		execute(upperCase(programPointer->second));
		programPointer++;
	}
}