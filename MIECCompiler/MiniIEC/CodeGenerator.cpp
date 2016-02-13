#include <iostream>
#include <map>
#include <assert.h>
#include "CodeGenerator.h"
#include "VarSymbol.h"

namespace MIEC {

	CodeGenerator::CodeGenerator(std::list<DACEntry*> const& DACEntries)
		: mDACEntries(DACEntries), mpGenProl16(new CodeGenProl16()), mpRegAdmin(new RegisterAdmin(mpGenProl16))
	{

	}

	CodeGenerator::~CodeGenerator() {
		delete mpGenProl16;
		delete mpRegAdmin;
	}

	void CodeGenerator::GenerateCode(std::wstring const& fileName) {
		if (mDACEntries.empty() || mpGenProl16 == nullptr) {
			std::cout << "Something is wrong, no Code generated!" << std::endl;
			return;
		}

		TUnresolvedJumps unresolvedJumps;

		// iterate through the DAC entries
		auto dacEntryItor = mDACEntries.begin();
		for (; dacEntryItor != mDACEntries.end(); ++dacEntryItor) {
			WORD const codePos = mpGenProl16->GetCodePosition();
			DACEntry * pDACEntry = *dacEntryItor;

			// check if DAC entry has a label and get it
			Label * pLabel = pDACEntry->GetLabel();
			if (pLabel != nullptr) {
				// set the adress of the label
				pLabel->SetAddr(codePos);
				// check if label is bookmarked
				auto labelItor = unresolvedJumps.find(pLabel);
				if (labelItor != unresolvedJumps.end()) {
					// write to program code
					mpGenProl16->SetAddress(labelItor->second, codePos);
					unresolvedJumps.erase(labelItor);
				}
				else {
					// bookmark label address
					unresolvedJumps.insert(std::pair<Label*, WORD>(pLabel, codePos));
				}
			}

			// DAC operation
			auto opKind = pDACEntry->GetOpKind();
			switch (opKind) {
				//case eNone:
				//	break;
			case eAdd:
				OperationAdd(pDACEntry);
				break;
			case eSubtract:
				OperationSubtract(pDACEntry);
				break;
			case eMultiply:
				OperationMultiply(pDACEntry);
				break;
			case eDivide:
				OperationDivide(pDACEntry);
				break;
			case eAssign:
				OperationAssign(pDACEntry);
				break;
			case eJump:
				OperationJump(pDACEntry, unresolvedJumps);
				break;
			case eIfJump:
			case eIfFalseJump:
				OperationConditionalJump(pDACEntry, unresolvedJumps);
				break;
			case ePrint:
				OperationPrint(pDACEntry);
				break;
			case eExit:
				mpGenProl16->Sleep();
				break;
			default:
				//assert(false);
				break;
			}
		}
		mpGenProl16->WriteIex(fileName);
	}

	// Addition
	void CodeGenerator::OperationAdd(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg1()); // summand 1
		tRegNr regB = mpRegAdmin->GetRegister(pDACEntry->GetArg2()); // summand 2
		tRegNr regResult = mpRegAdmin->GetRegister();

		mpGenProl16->Move(regResult, regA);
		mpGenProl16->Add(regResult, regB); // result in regA

		// regA contains result of addition -> assign to DAC-Entry
		mpRegAdmin->AssignRegister(regResult, pDACEntry);

		// free all other registers
		mpRegAdmin->FreeRegister(regA);
		mpRegAdmin->FreeRegister(regB);
	}

	// Subtraction
	void CodeGenerator::OperationSubtract(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg1()); // minuend
		tRegNr regB = mpRegAdmin->GetRegister(pDACEntry->GetArg2()); // subtrahend
		tRegNr regResult = mpRegAdmin->GetRegister();

		mpGenProl16->Move(regResult, regA);
		mpGenProl16->Sub(regResult, regB); // result in regA

		// regA contains result of subtraction -> assign to DAC-Entry
		mpRegAdmin->AssignRegister(regResult, pDACEntry);

		// free all other registers
		mpRegAdmin->FreeRegister(regA);
		mpRegAdmin->FreeRegister(regB);
	}

	////////////////////////////////////////////////////////////////////////////
	// Multiplication by shift
	//
	// result = 0
	// while (multiplier != 0)
	// {
	//   multiplier = multiplier >> 1
	//	 if (carry != 0)
	//	 {
	//	   result += multiplikand
	//	 }
	//   multiplikand = multiplikand << 1
	// }
	////////////////////////////////////////////////////////////////////////////
	void CodeGenerator::OperationMultiply(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg1()); // multiplikand
		tRegNr regB = mpRegAdmin->GetRegister(pDACEntry->GetArg2()); // multiplier
		tRegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps
		tRegNr regResult = mpRegAdmin->GetRegister(); // will contain result

		mpGenProl16->LoadI(regResult, 0); //init
		WORD codePosStart = mpGenProl16->GetCodePosition(); //start of loop begin
		tRegNr regHelp = mpRegAdmin->GetRegister();
		mpGenProl16->LoadI(regHelp, 0);
		mpGenProl16->Comp(regB, regHelp); //compare: multiplier != 0
		WORD jumpData1 = mpGenProl16->LoadI(regJmp, 0); //stores jump address of while-statement
		mpGenProl16->JumpZ(regJmp); //jump if zero bit is set (multiplier = 0)
		mpGenProl16->ShR(regB); //multiplier = multiplier >> 1

		WORD jumpData2 = mpGenProl16->LoadI(regJmp, 0); //stores jump address of if-statement

		//jump if carry bit is set ( multiplier = 3 -> 011 >> 1 = 001 (carry bit = 1) )
		mpGenProl16->JumpC(regJmp);
		mpGenProl16->ShL(regA); //multiplicand = multiplicand << 1
		mpGenProl16->LoadI(regJmp, codePosStart);  //prepeare jump address
		mpGenProl16->Jump(regJmp);  //jump to begin of while-statement
		//sets jump address for if-statement
		mpGenProl16->SetAddress(jumpData2, mpGenProl16->GetCodePosition());
		mpGenProl16->Add(regResult, regA);  //adds multiplicand to result register

		mpGenProl16->ShL(regA); //multiplicand = multiplicand << 1
		mpGenProl16->LoadI(regJmp, codePosStart); //prepeare jump address
		mpGenProl16->Jump(regJmp); //jump to begin of while-statement

		//sets jump address for end of while-statement
		mpGenProl16->SetAddress(jumpData1, mpGenProl16->GetCodePosition());

		// regResult contains result of multiplication -> assign to DAC-symbol
		mpRegAdmin->AssignRegister(regResult, pDACEntry);

		// free all other registers
		mpRegAdmin->FreeRegister(regA);
		mpRegAdmin->FreeRegister(regB);
		mpRegAdmin->FreeRegister(regJmp);
		mpRegAdmin->FreeRegister(regHelp);
	}

	////////////////////////////////////////////////////////////////////////////
	// Division by shift
	//
	// remainder = 0
	// bits = 16
	// do
	// {
	//   [Logic shift left] dividend
	//   [Shift left] remainder
	//   if ((carry != 0) || (remainder >= divisor))
	//   {
	//     remainder -= divisor
	//     dividend |= 0x01
	//   }
	//   bits--
	// } while (bits > 0)
	//
	////////////////////////////////////////////////////////////////////////////
	void CodeGenerator::OperationDivide(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg1()); // dividend
		tRegNr regB = mpRegAdmin->GetRegister(pDACEntry->GetArg2()); // divisor
		tRegNr regJmp = mpRegAdmin->GetRegister(); // used for jumps

		tRegNr regRemainder = mpRegAdmin->GetRegister(); // remainder
		mpGenProl16->LoadI(regRemainder, 0);
		tRegNr regBits = mpRegAdmin->GetRegister(); // bit counter
		mpGenProl16->LoadI(regBits, 16);
		WORD codePosStart = mpGenProl16->GetCodePosition();
		mpGenProl16->ShL(regA);
		mpGenProl16->ShLC(regRemainder);
		WORD jumpData1 = mpGenProl16->LoadI(regJmp, 0);
		mpGenProl16->JumpC(regJmp);
		mpGenProl16->Comp(regB, regRemainder);
		mpGenProl16->JumpC(regJmp);
		mpGenProl16->JumpZ(regJmp);
		WORD jumpData2 = mpGenProl16->LoadI(regJmp, 0);
		mpGenProl16->Jump(regJmp);
		mpGenProl16->SetAddress(jumpData1, mpGenProl16->GetCodePosition());
		mpGenProl16->Sub(regRemainder, regB);
		tRegNr regHelp = mpRegAdmin->GetRegister();
		mpGenProl16->LoadI(regHelp, 1);
		mpGenProl16->Or(regA, regHelp);
		mpGenProl16->SetAddress(jumpData2, mpGenProl16->GetCodePosition());
		mpGenProl16->Dec(regBits);
		WORD jumpData3 = mpGenProl16->LoadI(regJmp, 0);
		mpGenProl16->JumpZ(regJmp);
		mpGenProl16->LoadI(regJmp, codePosStart);
		mpGenProl16->Jump(regJmp);
		mpGenProl16->SetAddress(jumpData3, mpGenProl16->GetCodePosition());

		// regA contains result of division -> assign to DAC-symbol
		mpRegAdmin->AssignRegister(regA, pDACEntry);

		// free all other registers
		mpRegAdmin->FreeRegister(regB);
		mpRegAdmin->FreeRegister(regJmp);
		mpRegAdmin->FreeRegister(regRemainder);
		mpRegAdmin->FreeRegister(regBits);
		mpRegAdmin->FreeRegister(regHelp);
	}

	// Assignment
	void CodeGenerator::OperationAssign(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg2()); // source
		tRegNr regB = mpRegAdmin->GetRegister(); // destination address

		// load the address of the variable
		mpGenProl16->LoadI(regB, ((VarSymbol*)(pDACEntry->GetArg1()))->GetOffset());
		// store the source to the destination address
		mpGenProl16->Store(regA, regB);

		// regA contains result of assignment -> assign to DAC-symbol
		//mpRegAdmin->AssignRegister(regA, pDACEntry);

		// free all other registers
		mpRegAdmin->FreeRegister(regA);
		mpRegAdmin->FreeRegister(regB);
	}

	// Unconditional Jump
	void CodeGenerator::OperationJump(DACEntry * pDACEntry, TUnresolvedJumps& arUnresolvedJumps) {
		tRegNr regJump = mpRegAdmin->GetRegister();

		Label * const pLabel = (Label*)(pDACEntry->GetArg1());
		auto itorUJump = arUnresolvedJumps.find(pLabel);
		if (itorUJump == arUnresolvedJumps.end()) {
			WORD jumpNext = mpGenProl16->LoadI(regJump, 0);
			// could not find address of label, has to be replaced later
			arUnresolvedJumps.insert(std::pair<Label*, WORD>(pLabel, jumpNext));
		}
		else {
			mpGenProl16->LoadI(regJump, itorUJump->second);
			arUnresolvedJumps.erase(itorUJump);
		}
		mpGenProl16->Jump(regJump);

		// free all registers
		mpRegAdmin->FreeRegister(regJump);
	}

	// Conditional Jump
	void CodeGenerator::OperationConditionalJump(DACEntry * pDACEntry, TUnresolvedJumps& arUnresolvedJumps) {
		DACEntry * const pBranchCond = (DACEntry*)(pDACEntry->GetArg1());
		tRegNr regA = mpRegAdmin->GetRegister(pBranchCond->GetArg1());
		tRegNr regB = mpRegAdmin->GetRegister(pBranchCond->GetArg2());

		tRegNr regJump = mpRegAdmin->GetRegister();
		WORD jumpElse = mpGenProl16->LoadI(regJump, 0);

		// get the operation kind of the condition (relative operation)
		auto op = pBranchCond->GetOpKind();
		// if the conditional jump is IfFalseJump, turn the relative operations around
		if (pDACEntry->GetOpKind() == OpKind::eIfFalseJump) {
			switch (op) {
			case OpKind::eIsEqual:
				op = OpKind::eIsNotEqual; break;
			case OpKind::eIsNotEqual:
				op = OpKind::eIsEqual; break;
			case OpKind::eIsLessEqual:
				op = OpKind::eIsGreater; break;
			case OpKind::eIsGreaterEqual:
				op = OpKind::eIsLess; break;
			case OpKind::eIsLess:
				op = OpKind::eIsGreaterEqual; break;
			case OpKind::eIsGreater:
				op = OpKind::eIsLessEqual; break;
			default:
				assert(false);
				break;
			}
		}

		// generate commands depending on the relative operation
		switch (op) {
		case OpKind::eIsEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpZ(regJump);
			break;
		case OpKind::eIsNotEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpZ(regJump);
			mpGenProl16->SetAddress(jumpElse, mpGenProl16->GetCodePosition() + 3 * sizeof(WORD));
			jumpElse = mpGenProl16->LoadI(regJump, 0);
			mpGenProl16->Jump(regJump);
			mpRegAdmin->FreeRegister(regJump);
			break;
		case OpKind::eIsLessEqual:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpC(regJump);
			mpGenProl16->JumpZ(regJump);
			break;
		case OpKind::eIsGreaterEqual:
			mpGenProl16->Comp(regB, regA);
			mpGenProl16->JumpC(regJump);
			mpGenProl16->JumpZ(regJump);
			break;
		case OpKind::eIsLess:
			mpGenProl16->Comp(regA, regB);
			mpGenProl16->JumpC(regJump);
			break;
		case OpKind::eIsGreater:
			mpGenProl16->Comp(regB, regA);
			mpGenProl16->JumpC(regJump);
			break;
		default:
			assert(false);
			break;
		}

		// address of label has to be replaced later
		arUnresolvedJumps.insert(std::pair<Label*, WORD>((Label*)(pDACEntry->GetArg2()), jumpElse));

		// free all registers
		mpRegAdmin->FreeRegister(regA);
		mpRegAdmin->FreeRegister(regB);
		mpRegAdmin->FreeRegister(regJump);
	}

	void CodeGenerator::OperationPrint(DACEntry * pDACEntry) {
		tRegNr regA = mpRegAdmin->GetRegister(pDACEntry->GetArg1()); // get register
		mpGenProl16->PrintInt(regA); // print register
		mpRegAdmin->FreeRegister(regA); // free register
	}

} // namespace MIEC