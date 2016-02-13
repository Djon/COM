#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <fstream>
#include <list>
#include "DACGenerator.h"
#include "CodeGenProl16.h"
#include "RegisterAdmin.h"

namespace MIEC {

	class CodeGenerator {
	public:

		CodeGenerator(std::list<DACEntry*> const& DACEntries);
		~CodeGenerator();

		void GenerateCode(std::wstring const& fileName);

	private:
		CodeGenerator();
		CodeGenerator(const CodeGenerator&);
		CodeGenerator& operator= (const CodeGenerator&);

		typedef std::map<Label*, WORD> TUnresolvedJumps;
		typedef BYTE tRegNr;

		void OperationAdd(DACEntry * pDACEntry);
		void OperationSubtract(DACEntry * pDACEntry);
		void OperationMultiply(DACEntry * pDACEntry);
		void OperationDivide(DACEntry * pDACEntry);
		void OperationAssign(DACEntry * pDACEntry);
		void OperationJump(DACEntry * pDACEntry, TUnresolvedJumps& arUnresolvedJumps);
		void OperationConditionalJump(DACEntry * pDACEntry, TUnresolvedJumps& arUnresolvedJumps);
		void OperationPrint(DACEntry * pDACEntry);

		//private members
		std::list<DACEntry*> const& mDACEntries;
		CodeGenProl16 * mpGenProl16;
		RegisterAdmin * mpRegAdmin;
	};
} // namespace MIEC

#endif