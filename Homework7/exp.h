#ifndef _EXP_H_
#define _EXP_H_

#include <string>

using namespace std;

class Expression {
public:
	Expression();

	virtual ~Expression();

	virtual string toString() = 0;

};

class NonOpExp: public Expression {
public:
	NonOpExp(string v);
	virtual string toString();

private:

	string value;
};

class OpExp: public Expression {
public:
	OpExp(string _op, Expression *l, Expression *r);
	virtual string  toString();
	virtual ~OpExp();

private:

	string op;
	Expression *left, *right;

};

#endif