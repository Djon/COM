#include "Symbol.h"

Symbol::Symbol(SymbolType symbolType, wchar_t* const name, DataType * pType) {
	mName = name;
	mpDataType = pType;
	mSymbolType = symbolType;
}

wchar_t* Symbol::GetName() {
	return mName;
}

DataType * Symbol::GetDataType() {
	return mpDataType;
}

SymbolType Symbol::GetSymbolType() const {
	return mSymbolType;
}