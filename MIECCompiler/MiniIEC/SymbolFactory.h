#ifndef SYMBOLFACTORY_H
#define SYMBOLFACTORY_H

#include "SingletonBase.h"
#include "VarSymbol.h"
#include "ConstIntSymbol.h"
#include "BaseType.h"

class SymbolFactory : public SingletonBase < SymbolFactory > {
	friend class SingletonBase < SymbolFactory > ;
public:
	bool SetIntType(BaseType * pIntType);
	Symbol * CreateVarIntSymbol(wchar_t* name);
	Symbol * CreateConstIntSymbol(wchar_t* name, int const value);

private:
	SymbolFactory();
	BaseType * mpIntType;
	size_t mOffset;
};

#endif