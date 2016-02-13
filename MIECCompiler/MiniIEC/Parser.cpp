

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace MIEC {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::MIEC() {
		Expect(3);
		Expect(1);
		mDACGen.SetParser(this); 
		if (la->kind == 6) {
			VarDeclList();
		}
		Expect(4);
		Statements();
		while (!(la->kind == 0 || la->kind == 5)) {SynErr(32); Get();}
		Expect(5);
		mDACGen.AddStatement(OpKind::eExit, 0, 0); 
}

void Parser::VarDeclList() {
		Expect(6);
		size_t addr = 0; 
		VarDecl(addr);
		while (la->kind == 1 || la->kind == 2) {
			VarDecl(addr);
		}
		Expect(7);
}

void Parser::Statements() {
		Stat();
		while (StartOf(1)) {
			Stat();
		}
}

void Parser::VarDecl(size_t &addr) {
		wchar_t* name = 0; DataType * pType = 0; 
		Ident(name);
		Expect(8);
		Type(pType);
		Expect(9);
		if (mDACGen.AddSymbol(new VarSymbol(name, pType, addr))) { addr += pType->GetSize(); } 
}

void Parser::Ident(wchar_t* &name) {
		if (la->kind == 1) {
			Get();
			name =  t->val; 
		} else if (la->kind == 2) {
			Get();
		} else SynErr(33);
		mDACGen.SetPosition(t->line); 
}

void Parser::Type(DataType* &pType) {
		if (la->kind == 29) {
			Get();
			pType = mDACGen.FindSymbol(INT_NAME)->GetDataType(); 
		} else if (la->kind == 30) {
			Get();
			pType = mDACGen.FindSymbol(BOOLEAN_NAME)->GetDataType(); 
		} else if (StartOf(2)) {
			Get();
			Error(L"Unknown Type Symbol"); 
		} else SynErr(34);
		mDACGen.SetPosition(t->line); 
}

void Parser::Stat() {
		Symbol* pSym = 0; 
		if (la->kind == 1 || la->kind == 2) {
			Assignment(pSym);
		} else if (la->kind == 11) {
			Branch(pSym);
		} else if (la->kind == 14) {
			Loop(pSym);
		} else if (la->kind == 16) {
			Print(pSym);
		} else SynErr(35);
}

void Parser::Assignment(Symbol* &pSym) {
		wchar_t* pName = 0; Symbol* pSym1 = 0; 
		Ident(pName);
		pSym = mDACGen.FindSymbol(pName); 
		Expect(10);
		Expr(pSym1);
		while (!(la->kind == 0 || la->kind == 9)) {SynErr(36); Get();}
		Expect(9);
		pSym = mDACGen.AddStatement(OpKind::eAssign, pSym, pSym1); 
}

void Parser::Branch(Symbol* &pSym) {
		Label* lbElse = mDACGen.CreateLabel(); Label* lbNext = mDACGen.CreateLabel(); 
		Expect(11);
		Condition(pSym);
		Expect(12);
		mDACGen.AddStatement(OpKind::eIfFalseJump, pSym, lbElse); 
		Statements();
		if (la->kind == 13) {
			Get();
			mDACGen.AddStatement(OpKind::eJump, lbNext, 0); mDACGen.RegisterLabel(lbElse); 
			Statements();
			mDACGen.RegisterLabel(lbNext); 
		}
		while (!(la->kind == 0 || la->kind == 5)) {SynErr(37); Get();}
		Expect(5);
		mDACGen.RegisterLabel(lbElse); 
}

void Parser::Loop(Symbol* &pSym) {
		Label* lbCondition = mDACGen.CreateLabel(); Label* lbNext = mDACGen.CreateLabel(); 
		Expect(14);
		mDACGen.RegisterLabel(lbCondition); 
		Condition(pSym);
		Expect(15);
		mDACGen.AddStatement(OpKind::eIfFalseJump, pSym, lbNext); 
		Statements();
		mDACGen.AddStatement(OpKind::eJump, lbCondition, 0); 
		while (!(la->kind == 0 || la->kind == 5)) {SynErr(38); Get();}
		Expect(5);
		mDACGen.RegisterLabel(lbNext); 
}

void Parser::Print(Symbol* &pSym) {
		Expect(16);
		Expect(17);
		Expr(pSym);
		while (!(la->kind == 0 || la->kind == 18)) {SynErr(39); Get();}
		Expect(18);
		Expect(9);
		pSym = mDACGen.AddStatement(OpKind::ePrint, pSym, 0); 
}

void Parser::Expr(Symbol* &pSym) {
		OpKind op = eNone; Symbol* pSym1 = 0; 
		Term(pSym);
		while (la->kind == 25 || la->kind == 26) {
			if (la->kind == 25) {
				Get();
				op = OpKind::eAdd; 
			} else {
				Get();
				op = OpKind::eSubtract; 
			}
			Term(pSym1);
			pSym = mDACGen.AddStatement(op, pSym, pSym1); 
		}
}

void Parser::Condition(Symbol* &pSym) {
		OpKind op = OpKind::eNone; Symbol* pSym1 = 0; 
		Expr(pSym);
		Relop(op);
		Expr(pSym1);
		pSym = mDACGen.AddStatement(op, pSym, pSym1); 
}

void Parser::Relop(OpKind &op) {
		switch (la->kind) {
		case 19: {
			Get();
			op = OpKind::eIsEqual; 
			break;
		}
		case 20: {
			Get();
			op = OpKind::eIsLessEqual; 
			break;
		}
		case 21: {
			Get();
			op = OpKind::eIsGreaterEqual; 
			break;
		}
		case 22: {
			Get();
			op = OpKind::eIsNotEqual; 
			break;
		}
		case 23: {
			Get();
			op = OpKind::eIsLess; 
			break;
		}
		case 24: {
			Get();
			op = OpKind::eIsGreater; 
			break;
		}
		default: SynErr(40); break;
		}
}

void Parser::Term(Symbol* &pSym) {
		OpKind op = OpKind::eNone; Symbol* pSym1 = 0; 
		Fact(pSym);
		while (la->kind == 27 || la->kind == 28) {
			if (la->kind == 27) {
				Get();
				op = OpKind::eMultiply; 
			} else {
				Get();
				op = OpKind::eDivide; 
			}
			Fact(pSym1);
			pSym = mDACGen.AddStatement(op, pSym, pSym1); 
		}
}

void Parser::Fact(Symbol* &pSym) {
		if (la->kind == 1) {
			Get();
			pSym = mDACGen.FindSymbol(t->val); 
		} else if (la->kind == 2) {
			Get();
			int val; swscanf(t->val, L"%i", &val); pSym = mDACGen.AddSymbol(new ConstIntSymbol(t->val, mDACGen.GetDataType(L"Integer"), val)); 
		} else if (la->kind == 17) {
			Get();
			Expr(pSym);
			while (!(la->kind == 0 || la->kind == 18)) {SynErr(41); Get();}
			Expect(18);
		} else SynErr(42);
		mDACGen.SetPosition(t->line); 
}



void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	MIEC();

	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 31;

	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[3][33] = {
		{T,x,x,x, x,T,x,x, x,T,x,x, x,x,x,x, x,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,x, x,x,x,x, x,x,x,T, x,x,T,x, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T, T,x,x,T, x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"number expected"); break;
			case 3: s = coco_string_create(L"\"PROGRAM\" expected"); break;
			case 4: s = coco_string_create(L"\"BEGIN\" expected"); break;
			case 5: s = coco_string_create(L"\"END\" expected"); break;
			case 6: s = coco_string_create(L"\"BEGIN_VAR\" expected"); break;
			case 7: s = coco_string_create(L"\"END_VAR\" expected"); break;
			case 8: s = coco_string_create(L"\":\" expected"); break;
			case 9: s = coco_string_create(L"\";\" expected"); break;
			case 10: s = coco_string_create(L"\":=\" expected"); break;
			case 11: s = coco_string_create(L"\"IF\" expected"); break;
			case 12: s = coco_string_create(L"\"THEN\" expected"); break;
			case 13: s = coco_string_create(L"\"ELSE\" expected"); break;
			case 14: s = coco_string_create(L"\"WHILE\" expected"); break;
			case 15: s = coco_string_create(L"\"DO\" expected"); break;
			case 16: s = coco_string_create(L"\"print\" expected"); break;
			case 17: s = coco_string_create(L"\"(\" expected"); break;
			case 18: s = coco_string_create(L"\")\" expected"); break;
			case 19: s = coco_string_create(L"\"=\" expected"); break;
			case 20: s = coco_string_create(L"\"<=\" expected"); break;
			case 21: s = coco_string_create(L"\">=\" expected"); break;
			case 22: s = coco_string_create(L"\"!=\" expected"); break;
			case 23: s = coco_string_create(L"\"<\" expected"); break;
			case 24: s = coco_string_create(L"\">\" expected"); break;
			case 25: s = coco_string_create(L"\"+\" expected"); break;
			case 26: s = coco_string_create(L"\"-\" expected"); break;
			case 27: s = coco_string_create(L"\"*\" expected"); break;
			case 28: s = coco_string_create(L"\"/\" expected"); break;
			case 29: s = coco_string_create(L"\"Integer\" expected"); break;
			case 30: s = coco_string_create(L"\"Boolean\" expected"); break;
			case 31: s = coco_string_create(L"??? expected"); break;
			case 32: s = coco_string_create(L"this symbol not expected in MIEC"); break;
			case 33: s = coco_string_create(L"invalid Ident"); break;
			case 34: s = coco_string_create(L"invalid Type"); break;
			case 35: s = coco_string_create(L"invalid Stat"); break;
			case 36: s = coco_string_create(L"this symbol not expected in Assignment"); break;
			case 37: s = coco_string_create(L"this symbol not expected in Branch"); break;
			case 38: s = coco_string_create(L"this symbol not expected in Loop"); break;
			case 39: s = coco_string_create(L"this symbol not expected in Print"); break;
			case 40: s = coco_string_create(L"invalid Relop"); break;
			case 41: s = coco_string_create(L"this symbol not expected in Fact"); break;
			case 42: s = coco_string_create(L"invalid Fact"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace


