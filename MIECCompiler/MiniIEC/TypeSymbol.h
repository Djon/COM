#ifndef TYPESYMBOL_H
#define TYPESYMBOL_H

#include "Symbol.h"

class Symbol;

class TypeSymbol : public Symbol {
public:
	TypeSymbol(wchar_t* const name, DataType * pType);
};

#endif