#include "TypeSymbol.h"

TypeSymbol::TypeSymbol(wchar_t* const name, DataType * pType) : Symbol(SymbolType::eType, name, pType) {};