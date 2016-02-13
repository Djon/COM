#ifndef SYMBOL_H
#define SYMBOL_H

#include "Object.h"

class DataType;

typedef enum { eType, eVar, eTempVar, eLabel, eConst } SymbolType;

class Symbol : public Object {

public:
	Symbol(SymbolType symbolType, wchar_t* const name, DataType * pType);

	wchar_t* GetName();
	DataType * GetDataType();
	SymbolType Symbol::GetSymbolType() const;

private:
	wchar_t* mName;
	DataType * mpDataType;
	SymbolType mSymbolType;
};

#endif
