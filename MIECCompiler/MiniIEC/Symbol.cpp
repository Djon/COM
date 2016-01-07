#include "Symbol.h"

Symbol::Symbol(wchar_t* const name, DataType * pType) {
	mName = name;
	mpType = pType;
}

wchar_t* Symbol::GetName() {
	return mName;
}

DataType * Symbol::GetType() {
	return mpType;
}