#include "VarSymbol.h"

VarSymbol::VarSymbol(wchar_t* const name, DataType * pType, size_t const offset) : Symbol(SymbolType::eVar, name, pType) {
	mOffset = offset;
}

size_t VarSymbol::GetOffset() {
	return mOffset;
}