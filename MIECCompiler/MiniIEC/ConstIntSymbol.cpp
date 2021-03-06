#include "ConstIntSymbol.h"

ConstIntSymbol::ConstIntSymbol(wchar_t* const name, DataType * pType, int const value) : Symbol(SymbolType::eConst, name, pType) {
	mValue = value;
}

int ConstIntSymbol::GetValue() {
	return mValue;
}