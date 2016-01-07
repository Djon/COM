#include "DACEntry.h"

DACEntry::DACEntry(OpKind opKind, Symbol * sym1, Symbol * sym2) {
	mOpKind = opKind;
	mArg1 = sym1;
	mArg2 = sym2;
}