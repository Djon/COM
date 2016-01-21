#ifndef BASETYPE_H
#define BASETYPE_H

#include "Type.h"

#define INT_SIZE 4
#define CHAR_SIZE 1
#define BOOLEAN_SIZE 1
#define INT_NAME L"Integer"
#define CHAR_NAME L"Char"
#define BOOLEAN_NAME L"Boolean"

enum BaseTypeName { INT, CHAR, BOOLEAN };

class BaseType : public DataType {
public:
	BaseType(BaseTypeName const baseTypeName);

private:
	BaseTypeName mBaseTypeName;
	size_t TypeToSize(BaseTypeName const baseTypeName);
};

#endif