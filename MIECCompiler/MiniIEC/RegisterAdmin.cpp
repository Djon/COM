#include <iostream>
#include <assert.h>
#include "RegisterAdmin.h"
#include "ConstIntSymbol.h"
#include "VarSymbol.h"

namespace MIEC {

	RegisterAdmin::RegisterAdmin(CodeGenProl16* const pProl16Gen)
		: mpProl16Gen(pProl16Gen)
	{
	}

	RegisterAdmin::~RegisterAdmin() {
	}

	size_t const RegisterAdmin::GetRegister() {
		size_t reg = 0;
		
		// find empty register
		while (mRegisters.find(reg) != mRegisters.end()) {
			reg++;
		}

		Symbol * const pSymbol = 0;
		mRegisters.insert(std::pair<size_t const, Symbol * const>(reg, pSymbol));
		return reg;
	}

	size_t const RegisterAdmin::GetRegister(Symbol * const pSymbol) {
		if (pSymbol == nullptr) {
			return GetRegister();
		}

		// check if symbol is already in a register
		for (auto itor : mRegisters) {
			if (itor.second == pSymbol) {
				// found register
				return itor.first;
			}
		}

		// symbol is in no register -> make new register
		assert(mpProl16Gen != 0);
		size_t const reg = GetRegister();
		size_t addrReg = 0;	// register for address for const symbol
		switch (pSymbol->GetSymbolType())
		{
		case eConst:
			// for const symbols just load value in register
			mpProl16Gen->LoadI(reg, ((ConstIntSymbol*)pSymbol)->GetValue());
			break;
		case eVar:
			// variable registers
			addrReg = GetRegister();	// get register for address
			mpProl16Gen->LoadI(addrReg, ((VarSymbol*)pSymbol)->GetOffset()); // load the address
			mpProl16Gen->Load(reg, addrReg); // load the variable
			FreeRegister(addrReg); // free the temp address reg
			break;
		default:
			assert(false);
			break;
		}

		// assign symbol to register
		AssignRegister(reg, pSymbol);
		return reg;
	}

	void RegisterAdmin::AssignRegister(size_t const reg, Symbol * const pSymbol) {
		if (pSymbol == nullptr) {
			std::cout << "RegisterAdmin::AssignRegister: pSymbol is nullptr" << std::endl;
			return;
		}
		mRegisters.find(reg)->second = pSymbol;
	}

	void RegisterAdmin::FreeRegister(size_t const reg) {
		mRegisters.erase(reg);
	}

} // namespace MIEC