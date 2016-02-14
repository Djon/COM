#include "Scanner.h"
#include "Symbol.h"

Symbol::Symbol(SymbolType symbolType, wchar_t* const name, DataType * pType) {
	mName = coco_string_create(name);
	mpDataType = pType;
	mSymbolType = symbolType;
}
Symbol::~Symbol() {
	coco_string_delete(mName);
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