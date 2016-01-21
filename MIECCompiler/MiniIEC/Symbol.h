#ifndef SYMBOL_H
#define SYMBOL_H

#include "Object.h"
//#include "Type.h"

class DataType;

typedef enum { eType, eVar, eTempVar, eLabel } SymbolType;

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
