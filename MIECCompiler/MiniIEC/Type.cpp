#include "Type.h"
#include <string>

DataType::DataType(size_t const size)
{
	mSize = size;
	mpNameSymbol = 0;
}

size_t DataType::GetSize()
{
	return mSize;
}

wchar_t* DataType::GetName()
{
	if (mpNameSymbol != NULL) {
		return mpNameSymbol->GetName();
	}
	else {
		throw new std::string("error in GetName: NameSymbol not defined!");
	}
}

bool DataType::SetNameSymbol(TypeSymbol * pNameSymbol) {
	if (pNameSymbol != NULL) {
		mpNameSymbol = pNameSymbol;
		return true;
	}
	return false;
}
