#include <algorithm>
#include <iostream>
#include <string>
#include "SymbolTable.h"
#include "BaseType.h"
#include "Parser.h"

SymbolTable::SymbolTable() {}

SymbolTable::~SymbolTable()
{
	Reset();
}

void SymbolTable::Reset() {
	std::for_each(mSymbols.begin(), mSymbols.end(), [](std::pair<wchar_t*, Symbol*> pair)
	{
		if (pair.first == INT_NAME) {
			DataType * dataType = pair.second->GetDataType();
			if (dataType != nullptr) {
				delete dataType; dataType = nullptr;
			}
		}
		delete pair.second; pair.second = 0;
	});
	mSymbols.clear();
}

bool SymbolTable::AddTypeSymbol(BaseTypeName baseTypeName) {
	DataType * dataType = 0;
	TypeSymbol * symbol = 0;

	switch (baseTypeName)
	{
		case INT:
			dataType = new BaseType(baseTypeName);
			symbol = new TypeSymbol(INT_NAME, dataType);
			dataType->SetNameSymbol(symbol);
			return this->Add(symbol);
			break;
		case BOOLEAN:
			dataType = new BaseType(baseTypeName);
			symbol = new TypeSymbol(BOOLEAN_NAME, dataType);
			dataType->SetNameSymbol(symbol);
			return this->Add(symbol);
			break;
		case CHAR:
			return false;
			break;
		default:
			return false;
			break;
	}
}

bool SymbolTable::Add(Symbol * pSymbol) {
	if (pSymbol != NULL) {

		auto result = mSymbols.insert(tSymbolEntry(pSymbol->GetName(), pSymbol));
		
		if (!result.second)
		{
			// symbol already added
			if (result.first->second != pSymbol) {
				delete pSymbol;
				pSymbol = result.first->second;
			}

			std::wstring wname = std::wstring(pSymbol->GetName());
			std::string name = std::string(wname.begin(), wname.end());

			std::cout << "Symbol " << name << " already defined" << std::endl;

			return false;
		}
		return true;
	}
	return false;
}

Symbol * SymbolTable::Find(wchar_t* const name) {
	auto pos = mSymbols.find(name);
	if (pos != mSymbols.end()) {
		return pos->second;
	}
	else {
		return NULL;
	}
}

DataType* const SymbolTable::GetDataType(wchar_t* const pName)
{
	if (pName == 0 || coco_string_equal(pName, L"")) {
		std::cout << "GetDataType: invalid type name" << std::endl;
		return 0;
	}

	Symbol* const pSymbol = Find(pName);
	if (pSymbol == 0 || pSymbol->GetSymbolType() != SymbolType::eType) {
		std::cout << "GetDataType: undefined Type" << std::endl;
		return 0;
	}

	return pSymbol->GetDataType();
}

#ifdef _DEBUG
void SymbolTable::Print() {
	std::for_each(mSymbols.begin(), mSymbols.end(), [](std::pair<wchar_t*, Symbol*> pair)
	{
		std::wstring wname = std::wstring(pair.first);
		std::string name = std::string(wname.begin(), wname.end());
		
		std::wstring wtype = std::wstring(pair.second->GetDataType()->GetName());
		std::string type = std::string(wtype.begin(), wtype.end());

		std::cout << "Name: " << name << ", Type: " << type << std::endl;
	});
}
#endif