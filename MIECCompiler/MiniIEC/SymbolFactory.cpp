#include "SymbolFactory.h"
#include "Type.h"
#include "BaseType.h"

SymbolFactory::SymbolFactory() {
	mOffset = 0;
}

bool SymbolFactory::SetIntType(BaseType * pIntType) {
	if (pIntType != nullptr) {
		mpIntType = pIntType;
		return true;
	}
	return false;	
}

Symbol * SymbolFactory::CreateVarIntSymbol(wchar_t* name) {
	if (mpIntType != nullptr) {
		mOffset += INT_SIZE;
		return new VarSymbol(name, mpIntType, mOffset);
	}
	return nullptr;
}

Symbol * SymbolFactory::CreateConstIntSymbol(wchar_t* name, int const value) {
	if (mpIntType != nullptr) {
		return new ConstIntSymbol(name, mpIntType, value);
	}
	return nullptr;
}