#ifndef _exp_h
#define _exp_h
#include "evalstate.h"

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression {
public:
	Expression();
	virtual ~Expression();
	virtual int eval(EvalState & state) = 0;
	virtual std::string toString() = 0;
	virtual ExpressionType getType() = 0;
	virtual void setMinus();
};

class ConstantExp: public Expression {
public:
	ConstantExp(int value);
	virtual int eval(EvalState & state);
	virtual std::string toString();
	virtual ExpressionType getType();
	virtual void setMinus();
	int getValue();
private:
	int value;
};

class IdentifierExp : public Expression {
public:
	IdentifierExp(std::string name);
	virtual int eval(EvalState & state);
	virtual std::string toString();
	virtual ExpressionType getType();
	std::string getName();
	virtual void setMinus();
private:
	std::string name;
	int sign;
};

class CompoundExp: public Expression {
public:

	CompoundExp(std::string op, Expression *lhs, Expression *rhs);

	virtual ~CompoundExp();
	virtual int eval(EvalState & state);
	virtual std::string toString();
	virtual ExpressionType getType();
	std::string getOp();
	Expression *getLHS();
	Expression *getRHS();
	
private:
	std::string op;
	Expression *lhs, *rhs;
};
#endif