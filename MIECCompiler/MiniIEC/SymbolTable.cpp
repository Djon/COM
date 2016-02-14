#include <algorithm>
#include <iostream>
#include <string>
#include <assert.h>
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
		if (pair.second != nullptr) {
			// free the basetypes
			if (pair.second->GetSymbolType() == SymbolType::eType) {
				DataType * dataType = pair.second->GetDataType();
				if (dataType != nullptr) {
					delete dataType; dataType = nullptr;
				}
			}
			
			// free the symbol
			delete pair.second; pair.second = nullptr;
		}
	});
	mSymbols.clear();
}

Symbol * SymbolTable::AddTypeSymbol(BaseTypeName baseTypeName) {
	DataType * dataType = 0;
	TypeSymbol * symbol = 0;

	// add symbol to table, according to datatype
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

Symbol * SymbolTable::Add(Symbol * pSymbol) {
	if (pSymbol != NULL) {

		auto result = mSymbols.insert(tSymbolEntry(pSymbol->GetName(), pSymbol));
		
		if (!result.second)
		{
			// symbol already added
			if (result.first->second != pSymbol) {
				delete pSymbol;
				pSymbol = result.first->second;
			}

			return 0;
		}
	}
	return pSymbol;
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

	// return the datatype if there were no errors
	return pSymbol->GetDataType();
}

#ifdef _DEBUG
void SymbolTable::Print() const {
	std::for_each(mSymbols.begin(), mSymbols.end(), [](std::pair<wchar_t*, Symbol*> pair)
	{
		std::wstring wname = std::wstring(pair.first);
		std::string name = std::string(wname.begin(), wname.end());
		
		std::string type = "None";

		if (pair.second->GetDataType() != 0)
		{
			std::wstring wtype = std::wstring(pair.second->GetDataType()->GetName());
			type = std::string(wtype.begin(), wtype.end());
		}
		
		std::string symType = "Unknown";

		switch (pair.second->GetSymbolType())
		{
		case eType:
			symType = "Type";
			break;
		case eVar:
			symType = "Variable";
			break;
		case eTempVar:
			symType = "Temp Variable";
			break;
		case eLabel:
			symType = "Label";
			break;
		case eConst:
			symType = "Const";
			break;
		}

		std::cout << "Name: " << name << ", DataType: " << type << ", SymbolType: " << symType << std::endl;
	});
}
#endif