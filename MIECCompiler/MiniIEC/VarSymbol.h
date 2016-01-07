#ifndef VARSYMBOL_H
#define VARSYMBOL_H

#include "Symbol.h"

class VarSymbol : public Symbol {
public:
	VarSymbol(wchar_t* const name, DataType * pType, size_t const offset);
	size_t GetOffset();
private:
	size_t mOffset;
};

#endif
