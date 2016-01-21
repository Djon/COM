#include "DACEntry.h"
#include <algorithm>
#include <iostream>
#include <string>

DACEntry::DACEntry(OpKind opKind, Symbol * sym1, Symbol * sym2) {
	mOpKind = opKind;
	mArg1 = sym1;
	mArg2 = sym2;
}

void DACEntry::Print()
{
	std::string opKindName = "";

	std::wstring warg1 = std::wstring(mArg1->GetName());
	std::string arg1 = std::string(warg1.begin(), warg1.end());

	std::wstring warg2 = std::wstring(mArg2->GetName());
	std::string arg2 = std::string(warg2.begin(), warg2.end());

	switch (this->mOpKind)
	{
		case eAdd:
			opKindName = "add";
			break;
		case eSubtract:
			opKindName = "subtract";
			break;
		case eMultiply:
			opKindName = "multiply";
			break;
		case eDivide:
			opKindName = "divide";
			break;
		case eIsEqual:
			opKindName = "isequal";
			break;
		case eIsLessEqual:
			opKindName = "islessequal";
			break;
		case eIsGreaterEqual:
			opKindName = "isgreaterequal";
			break;
		case eIsNotEqual:
			opKindName = "isnotequal";
			break;
		case eIsLess:
			opKindName = "isless";
			break;
		case eIsGreater:
			opKindName = "isgreater";
			break;
		case eAssign:
			opKindName = "assign";
			break;
		case eJump:
			opKindName = "jump";
			break;
		case eIfJump:
			opKindName = "ifjump";
			break;
		case eIfFalseJump:
			opKindName = "iffalsejump";
			break;
		case ePrint:
			opKindName = "print";
			break;
		case eExit:
			opKindName = "exit";
			break;
	}

	std::cout << opKindName << " " << arg1 << " " << arg2 << std::endl;
}