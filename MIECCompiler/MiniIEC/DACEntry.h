#ifndef DACENTRY_H
#define DACENTRY_H

#include "Symbol.h"
#include "Label.h"

enum OpKind {
	eNone, eAdd, eSubtract, eMultiply, eDivide, eIsEqual, eIsLessEqual, eIsGreaterEqual,
	eIsNotEqual, eIsLess, eIsGreater, eAssign, eJump, eIfJump, eIfFalseJump, ePrint,
	eExit
};

class DACEntry : public Symbol {
public:
	DACEntry(OpKind opKind, Symbol * sym1, Symbol * sym2, DataType * dataType, wchar_t * const name, Label * label);

	Label* const GetLabel() const;
	OpKind const GetOpKind() const;
	Symbol* const GetArg1() const;
	Symbol* const GetArg2() const;

#ifdef _DEBUG
	void Print();
#endif

private:
	Label * mLabel;
	OpKind mOpKind;
	Symbol * mArg1;
	Symbol * mArg2;
};

#endif