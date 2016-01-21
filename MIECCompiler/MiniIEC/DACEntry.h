#ifndef DACENTRY_H
#define DACENTRY_H

#include "Symbol.h"

enum OpKind {
	eAdd, eSubtract, eMultiply, eDivide, eIsEqual, eIsLessEqual, eIsGreaterEqual,
	eIsNotEqual, eIsLess, eIsGreater, eAssign, eJump, eIfJump, eIfFalseJump, ePrint,
	eExit
};

class DACEntry : public Object {
public:
	DACEntry(OpKind opKind, Symbol * sym1, Symbol * sym2);

#ifdef _DEBUG
	void Print();
#endif

private:
	OpKind mOpKind;
	Symbol * mArg1;
	Symbol * mArg2;
};

#endif