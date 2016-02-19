#include "DACEntry.h"
#include <algorithm>
#include <iostream>
#include <string>

DACEntry::DACEntry(OpKind opKind, Symbol * sym1, Symbol * sym2, DataType * dataType, wchar_t * const name, Label * label)
	: Symbol(SymbolType::eTempVar, name, dataType) {
	mLabel = label;
	mOpKind = opKind;
	mArg1 = sym1;
	mArg2 = sym2;
}

#ifdef _DEBUG
void DACEntry::Print()
{
	std::string label = "    ";
	std::string opKindName = "";
	std::string name = "";
	std::string arg1 = "";
	std::string arg2 = "";

	if (mLabel != 0)
	{
		std::wstring wlabel = std::wstring(mLabel->GetName());
		label = std::string(wlabel.begin(), wlabel.end()) + ": ";
	}

	std::wstring wname = std::wstring(GetName());
	name = std::string(wname.begin(), wname.end());

	if (mArg1 != 0)
	{
		std::wstring warg1 = std::wstring(mArg1->GetName());
		arg1 = std::string(warg1.begin(), warg1.end());
	}

	if (mArg2 != 0)
	{
		std::wstring warg2 = std::wstring(mArg2->GetName());
		arg2 = std::string(warg2.begin(), warg2.end());
	}

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
	std::cout << label << name << " = " << opKindName;
		
	if (mArg1 != 0)
	{
		std::cout << ", " << arg1;
	}

	if (mArg2 != 0)
	{
		std::cout << ", " << arg2;
	}

	std::cout << std::endl;
}
#endif

Label* const DACEntry::GetLabel() const
{
	return mLabel;
}

OpKind const DACEntry::GetOpKind() const
{
	return mOpKind;
}

Symbol* const DACEntry::GetArg1() const
{
	return mArg1;
}

Symbol* const DACEntry::GetArg2() const
{
	return mArg2;
}