#include "DACGenerator.h"
#include <iostream>
#include <string>
#include "DACEntry.h"
#include <assert.h>

namespace MIEC {

DACGenerator::DACGenerator() : mLabelCount(0), mErrorCount(0), mLine(0), mTempVarCount(0), mCurrentLabel(0)
{
}


bool DACGenerator::SetParser(Parser * pParser) {
	if (pParser == nullptr) {
		return false;
	}
	mpParser = pParser;

	AddTypeSymbol(INT);
	AddTypeSymbol(BOOLEAN);

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

	DataType* pDataType = 0;	// unknown DataType;

	switch (opKind)
	{
	case OpKind::eAssign:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		if (sym1->GetSymbolType() != SymbolType::eVar) { Error("AddStatement: invalid assignment (Variable expected)"); mErrorCount++; break; }
		if (sym2 == 0) { Error("Invalid right parameter"); mErrorCount++; break; }
		pDataType = sym1->GetDataType();
		break;
	case OpKind::eAdd: case OpKind::eSubtract: case OpKind::eMultiply: case OpKind::eDivide:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		pDataType = sym1->GetDataType();
		if (sym2 == 0) { Error("Invalid right parameter"); mErrorCount++; break; }
		break;
	case OpKind::eIsEqual: case OpKind::eIsNotEqual: case OpKind::eIsLessEqual: case OpKind::eIsGreaterEqual: case OpKind::eIsLess: case OpKind::eIsGreater:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		pDataType = GetDataType(BOOLEAN_NAME);
		if (sym2 == 0) { Error("Invalid right parameter"); mErrorCount++; break; }
		break;
	case OpKind::eIfJump: case OpKind::eIfFalseJump:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		if (sym1->GetDataType() != GetDataType(BOOLEAN_NAME)) { Error("AddStatement: invalid condition (type Boolean expected)"); mErrorCount++; break; }
		break;
	case OpKind::eJump:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		if (sym1->GetSymbolType() != SymbolType::eLabel) { Error("AddStatement: invalid jump destination (Label expected)"); mErrorCount++; break; }
		break;
	case OpKind::ePrint:
		if (sym1 == 0) { Error("Invalid left parameter"); mErrorCount++; break; }
		break;
	case OpKind::eExit:
		if (sym1 != 0) { Error("Too many parameters"); mErrorCount++; break; }
		break;
	default:
		Error("AddStatement: unknown operation kind");
		break;
	}

	if (mErrorCount > 0)
	{
		return 0;
	}

	// store result in temp variable

	// get name next name
	size_t const strLength = swprintf(0, 0, L"%s%d", "t", mTempVarCount) + 1;
	wchar_t* pName = new wchar_t[strLength];
	swprintf(pName, strLength, L"%s%d", "t", mTempVarCount++);

	DACEntry * dacEntry = new DACEntry(opKind, sym1, sym2, pDataType, pName, mCurrentLabel);
	// unregister label
	mCurrentLabel = 0;
	// add to dac entry list
	mDACEntries.push_back(dacEntry);
	// add to symbol table
	AddSymbol(dacEntry);

	coco_string_delete(pName);

	return dacEntry;
}

void DACGenerator::SetPosition(size_t const line)
{
	mLine = line;
}

void DACGenerator::Error(std::string msg)
{
	std::cout << "-- " << "Line " << mLine << ": " << msg << std::endl;	
}

size_t DACGenerator::GetErrorCount() {
	return mErrorCount;
}

const std::list<DACEntry*>& DACGenerator::GetDACList() const
{
	return mDACEntries;
}

#ifdef _DEBUG
void DACGenerator::PrintDACList() const
{
	std::list<DACEntry*>::const_iterator itor = mDACEntries.begin();
	for (; itor != mDACEntries.end(); itor++) {
		(*itor)->Print();
	}
}

void DACGenerator::PrintSymTab() const
{
	mSymbolTable.Print();
}
#endif

Label* const DACGenerator::CreateLabel()
{
	size_t const strLength = swprintf(0, 0, L"%s%d", "L", mTempVarCount) + 1;
	wchar_t* pName = new wchar_t[strLength];
	swprintf(pName, strLength, L"%s%d", "L", mLabelCount++);

	// create label
	Label * pLabel = new Label(pName);

	// add label to symbols
	mSymbolTable.Add(pLabel);
	
	coco_string_delete(pName);

	return pLabel;
}

Label* const DACGenerator::RegisterLabel(Label * pLabel)
{
	if (pLabel->GetName() == 0)
	{
		Error("Invalid Label Name");
		delete pLabel; pLabel = 0;
	}

	// if we have not registered a label yet, do it now
	if (mCurrentLabel == 0)
	{
		mCurrentLabel = pLabel;
	}

	return mCurrentLabel;
}

Symbol * DACGenerator::AddSymbol(Symbol * pSymbol)
{
	if (pSymbol == 0) {
		Error("AddSymbol: invalid symbol");
		return 0;
	}

	// check if symbol already exists
	Symbol* result = mSymbolTable.Find(pSymbol->GetName());

	if (result != 0)
	{
		if (result->GetSymbolType() != eConst)
		{
			std::wstring wname = std::wstring(result->GetName());
			std::string name = std::string(wname.begin(), wname.end());
			Error("Symbol " + name + " already defined");
		}
		delete pSymbol; pSymbol = result;
	}
	else
	{
		result = mSymbolTable.Add(pSymbol);
	}

	return result;
}

Symbol * DACGenerator::FindSymbol(wchar_t* const name)
{
	if (name == 0) {
		Error("Invalid symbol name");
		return 0;
	}

	Symbol * result = mSymbolTable.Find(name);
	if (result == 0)
	{
		std::wstring wname = std::wstring(name);
		std::string name = std::string(wname.begin(), wname.end());
		Error("Unknown Symbol " + name);
	}
	return result;
}

Symbol * DACGenerator::AddTypeSymbol(BaseTypeName baseTypeName)
{
	Symbol * result = mSymbolTable.AddTypeSymbol(baseTypeName);
	if (result == 0)
	{
		Error("Type already added");
	}
	return result;
}

DataType* DACGenerator::GetDataType(wchar_t* const pName)
{
	DataType * result = mSymbolTable.GetDataType(pName);
	if (result == 0)
	{
		std::wstring wname = std::wstring(pName);
		std::string name = std::string(wname.begin(), wname.end());
		Error("Unknown Type " + name);
	}
	return result;
}

} // namespace MIEC