#ifndef DACGENERATOR_H
#define DACGENERATOR_H

#include <list>
#include "Parser.h"
#include "DACEntry.h"
#include "Symbol.h"
#include "SingletonBase.h"

class DACGenerator : public SingletonBase < DACGenerator > {
	friend class SingletonBase < DACGenerator > ;
public:
	bool SetParser(MIEC::Parser * pParser);
	
	DACEntry * AddStatement(OpKind opKind, Symbol * sym);
	DACEntry * AddStatement(OpKind opKind, Symbol * sym1, Symbol * sym2);

private:
	DACGenerator();

	std::list<DACEntry*> mDACEntries;
	
	MIEC::Parser * mpParser;
};

#endif