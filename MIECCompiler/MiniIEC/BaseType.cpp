#include "BaseType.h"
#include <string>

BaseType::BaseType(BaseTypeName const baseTypeName) : DataType(TypeToSize(baseTypeName))
{
	mBaseTypeName = baseTypeName;
}

size_t BaseType::TypeToSize(BaseTypeName const baseTypeName) {
	switch (baseTypeName)
	{
	case INT:
		return INT_SIZE;
		break;
	case CHAR:
		return CHAR_SIZE;
		break;
	case BOOLEAN:
		return BOOLEAN_SIZE;
		break;
	default:
		throw new std::string("Type not allowed.");
		break;
	}
}
