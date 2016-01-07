#include "DACGenerator.h"

bool DACGenerator::SetParser(MIEC::Parser * pParser) {
	if (pParser == nullptr) {
		return false;
	}
	mpParser = pParser;
	return true;
}

DACEntry * DACGenerator::AddStatement(OpKind opKind, Symbol * sym) {
	return AddStatement(opKind, sym, nullptr);
}

DACEntry * DACGenerator::AddStatement(OpKind opKind, Symbol * sym1, Symbol * sym2) {
	if (mpParser == nullptr) {
		// error
		return nullptr;
	}
	
	size_t err = 0;				// local error counter
	DataType* pDataType = 0;	// unknown DataType;
	
	switch (opKind)
	{
	case eAssign:
		break;

	case eAdd: case eSubtract: case eMultiply: case eDivide:
		if (sym1 == nullptr) {
			//TODO: error handling
			break;
		}
		pDataType = sym1->GetType();

		break;

	case eIsEqual:
		break;
	case eIsLessEqual:
		break;
	case eIsGreaterEqual:
		break;
	case eIsNotEqual:
		break;
	case eIsLess:
		break;
	case eIsGreater:
		break;

	case eJump:
		break;
	case eIfJump:
		break;
	case eIfFalseJump:
		break;
	case ePrint:
		break;
	case eExit:
		break;
	default:

		break;
	}

	switch (opKind)
	{
	case eAssign:
		break;

	case eAdd: case eSubtract: case eMultiply: case eDivide:

		break;

	case eIsEqual:
		break;
	case eIsLessEqual:
		break;
	case eIsGreaterEqual:
		break;
	case eIsNotEqual:
		break;
	case eIsLess:
		break;
	case eIsGreater:
		break;

	case eJump:
		break;
	case eIfJump:
		break;
	case eIfFalseJump:
		break;
	case ePrint:
		break;
	case eExit:
		break;
	default:
		break;
	}


	DACEntry * dacEntry = new DACEntry(opKind, sym1, sym2);

	mDACEntries.push_back(dacEntry);
	return dacEntry;
}