#ifndef REGISTERADMIN_H
#define REGISTERADMIN_H

#include <map>
#include "CodeGenProl16.h"
#include "Symbol.h"

namespace MIEC {

	class RegisterAdmin {
	public:
		RegisterAdmin(CodeGenProl16* const pProl16Gen);
		~RegisterAdmin();

		size_t const GetRegister();
		size_t const GetRegister(Symbol * const pSymbol);
		void AssignRegister(size_t const reg, Symbol * const pSymbol);
		void FreeRegister(size_t const reg);

	private:
		CodeGenProl16 * const mpProl16Gen;
		std::map<size_t const, Symbol *> mRegisters;
	};

} // namespace MIEC

#endif