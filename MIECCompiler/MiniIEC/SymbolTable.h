#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include "Symbol.h"
#include "SingletonBase.h"
#include "BaseType.h"

class SymbolTable : public SingletonBase<SymbolTable> {
	friend class SingletonBase<SymbolTable>;
public:
	virtual ~SymbolTable();
	void Reset();
	bool Add(Symbol * pSymbol);
	Symbol * Find(wchar_t* const name);
	bool AddTypeSymbol(BaseTypeName baseTypeName);
#ifdef _DEBUG
	void Print();
#endif

private:
	SymbolTable();
	std::unordered_map<wchar_t*, Symbol*> mSymbols;
};

#endif