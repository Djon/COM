#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include "Symbol.h"
#include "SingletonBase.h"
#include "BaseType.h"
#include "Scanner.h"


class SymbolTable {

public:

	class NameCompare : public std::binary_function<wchar_t*, wchar_t*, bool> {
	public:
		bool operator()(wchar_t* pName1, wchar_t* pName2) const {
			return (coco_string_compareto(pName1, pName2) < 0);
		}
	};

	SymbolTable();
	virtual ~SymbolTable();
	void Reset();
	Symbol * Add(Symbol * pSymbol);
	Symbol * Find(wchar_t* const name);
	Symbol * AddTypeSymbol(BaseTypeName baseTypeName);

	DataType* const SymbolTable::GetDataType(wchar_t* const pName);

#ifdef _DEBUG
	void Print() const;
#endif

private:
	typedef std::pair<wchar_t*, Symbol*> tSymbolEntry;
	std::map<wchar_t*, Symbol*, NameCompare> mSymbols;
};

#endif