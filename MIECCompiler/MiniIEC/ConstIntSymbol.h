#ifndef CONSTINTSYMBOL_H
#define CONSTINTSYMBOL_H

#include "Symbol.h"

class ConstIntSymbol : public Symbol {
public:
	ConstIntSymbol(wchar_t* const name, DataType * pType, int const value);
	int GetValue();
private:
	int mValue;
};

#endif
