#include "ConstIntSymbol.h"

ConstIntSymbol::ConstIntSymbol(wchar_t* const name, DataType * pType, int const value) : Symbol(SymbolType::eVar, name, pType) {
	mValue = value;
}

int ConstIntSymbol::GetValue() {
	return mValue;
}