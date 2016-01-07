#ifndef SYMBOL_H
#define SYMBOL_H

#include "Object.h"
//#include "Type.h"

class DataType;

class Symbol : public Object {
public:
	Symbol(wchar_t* const name, DataType * pType);
	wchar_t* GetName();
	DataType * GetType();

private:
	wchar_t* mName;
	DataType * mpType;
};

#endif
