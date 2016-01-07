#ifndef TYPE_H
#define TYPE_H

#include "Object.h"
#include "TypeSymbol.h"

class TypeSymbol;

class DataType : public Object {
public:
	DataType(size_t const size);
	size_t GetSize();
	wchar_t* GetName();
	bool SetNameSymbol(TypeSymbol * pNameSymbol);
private:
	size_t mSize;
	TypeSymbol * mpNameSymbol;
};

#endif
