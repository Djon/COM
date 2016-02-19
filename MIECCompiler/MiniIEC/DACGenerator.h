#ifndef DACGENERATOR_H
#define DACGENERATOR_H

#include <list>
#include "DACEntry.h"
#include "Symbol.h"
#include "SymbolTable.h"

namespace MIEC {

class Parser;

class DACGenerator {
public:
	DACGenerator();

	bool SetParser(Parser * pParser);
	
	DACEntry * AddStatement(OpKind opKind, Symbol * sym);
	DACEntry * AddStatement(OpKind opKind, Symbol * sym1, Symbol * sym2);
	
	Label* const CreateLabel();
	Label* const RegisterLabel(Label * pLabel);

	const std::list<DACEntry*>& GetDACList() const;

#ifdef _DEBUG
	void PrintDACList() const;
	void PrintSymTab() const;
#endif

	void DACGenerator::Error(std::string msg);
	size_t GetErrorCount();
	void SetPosition(size_t const line);

	Symbol * AddSymbol(Symbol * pSymbol);
	Symbol * FindSymbol(wchar_t* const name);
	Symbol * AddTypeSymbol(BaseTypeName baseTypeName);
	DataType* GetDataType(wchar_t* const pName);

private:
	std::list<DACEntry*> mDACEntries;

	Parser * mpParser;
	SymbolTable mSymbolTable;

	Label * mCurrentLabel;
	size_t mTempVarCount;		// auto incrementing value for temporary variable name creation
	size_t mLabelCount;			// auto incrementing value for label generation
	
	size_t mLine;				// the current line we are at
	size_t mErrorCount;			// number of found errors
};

} // namespace MIEC

#endif