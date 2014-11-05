#include <string>
#include "exp.h"
#include "strlib.h"
using namespace std;

Expression::Expression() {}

Expression::~Expression() {}

NonOpExp::NonOpExp(string v) : value(v) {}

string NonOpExp::toString() {
	return value;
}

OpExp::OpExp(string _op, Expression *l, Expression *r) 
: op(_op), left(l), right(r) {}

OpExp::~OpExp() {
	delete left;
	delete right;
}

string OpExp::toString() {
	return left->toString() + ' ' + right->toString() + ' ' + op;
}