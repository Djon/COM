#include "DACGenerator.h"
#include <iostream>
#include <string>
#include "DACEntry.h"



bool DACGenerator::SetParser(MIEC::Parser * pParser) {
	if (pParser == nullptr) {
		return false;
	}
	mpParser = pParser;
	return true;
}

DACEntry * DACGenerator::AddStatement(OpKind opKind, Symbol * sym) {
	return AddStatement(opKind, sym, nullptr);
}

DACEntry * DACGenerator::AddStatement(OpKind opKind, Symbol * sym1, Symbol * sym2) {
	if (mpParser == nullptr) {
		// error
		return nullptr;
	}
	
	size_t errorCount = 0;				// local error counter
	DataType* pDataType = 0;	// unknown DataType;

	if (sym1 == 0) { Error(L"Invalid left parameter"); errorCount++; }

	switch (opKind) 
	{
		case OpKind::eAssign:
			if (sym1->GetSymbolType() != SymbolType::eVar) { Error(L"AddStat: invalid assignment (Variable expected)"); errorCount++; break; }
			if (sym2 == 0) { Error(L"Invalid right parameter"); errorCount++; }
			pDataType = sym1->GetDataType();
			break;
		case OpKind::eAdd: case OpKind::eSubtract: case OpKind::eMultiply: case OpKind::eDivide:
			pDataType = sym1->GetDataType();
			if (sym2 == 0) { Error(L"Invalid right parameter"); errorCount++; }
			break;
		case OpKind::eIsEqual: case OpKind::eIsNotEqual: case OpKind::eIsLessEqual: case OpKind::eIsGreaterEqual: case OpKind::eIsLess: case OpKind::eIsGreater:
			pDataType = SymbolTable::GetInstance().GetDataType(BOOLEAN_NAME);
			if (sym2 == 0) { Error(L"Invalid right parameter"); errorCount++; }
			break;
		case OpKind::eIfJump: case OpKind::eIfFalseJump:
			if (sym1->GetDataType() != SymbolTable::GetInstance().GetDataType(BOOLEAN_NAME)) { Error(L"AddStat: invalid condition (type Boolean expected)"); errorCount++; break; }
			break;
		case OpKind::eJump:
			if (sym1->GetSymbolType() != SymbolType::eLabel) { Error(L"AddStat: invalid jump destination (Label expected)"); errorCount++; break; }
			break;
		default:
			Error(L"AddStatement: unknown operation kind");
			break;
	}

	DACEntry * dacEntry = new DACEntry(opKind, sym1, sym2);

	mDACEntries.push_back(dacEntry);
	return dacEntry;
}

void DACGenerator::Error(wchar_t * msg)
{
	std::wstring wname = std::wstring(msg);
	std::string name = std::string(wname.begin(), wname.end());
	std::cout << msg << std::endl;
}

const std::list<DACEntry*>& DACGenerator::GetDACList() const
{
	return mDACEntries;
}

void DACGenerator::PrintDACList(std::wostream& out) const
{
	std::list<DACEntry*>::const_iterator itor = mDACEntries.begin();
	for (; itor != mDACEntries.end(); itor++) {
		(*itor)->Print();
	}
}