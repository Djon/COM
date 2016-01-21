

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
		if (la->kind == 6) {
			VarDeclList();
		}
		Expect(4);
		Statements();
		Expect(5);
}

void Parser::VarDeclList() {
		Expect(6);
		size_t addr = 0; SymbolTable::GetInstance().AddTypeSymbol(INT); SymbolTable::GetInstance().AddTypeSymbol(BOOLEAN); 
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
		if (SymbolTable::GetInstance().Add(new VarSymbol(name, pType, addr))) { addr += pType->GetSize(); } 
}

void Parser::Ident(wchar_t* &name) {
		if (la->kind == 1) {
			Get();
			name =  t->val; 
		} else if (la->kind == 2) {
			Get();
		} else SynErr(32);
}

void Parser::Type(DataType* &pType) {
		if (la->kind == 29) {
			Get();
			pType = SymbolTable::GetInstance().Find(INT_NAME)->GetDataType(); 
		} else if (la->kind == 30) {
			Get();
			pType = SymbolTable::GetInstance().Find(BOOLEAN_NAME)->GetDataType(); 
		} else SynErr(33);
}

void Parser::Stat() {
		if (la->kind == 1) {
			Get();
			Expect(10);
			Expr();
			Expect(9);
		} else if (la->kind == 11) {
			Get();
			Expect(12);
			Expr();
			Expect(13);
			Expect(9);
		} else if (la->kind == 14) {
			Get();
			Condition();
			Expect(15);
			Statements();
			Expect(5);
		} else if (la->kind == 16) {
			Get();
			Condition();
			Expect(17);
			Statements();
			if (la->kind == 5) {
				Get();
			} else if (la->kind == 18) {
				Get();
				Statements();
				Expect(5);
			} else SynErr(34);
		} else SynErr(35);
}

void Parser::Expr() {
		Term();
		while (la->kind == 19 || la->kind == 20) {
			if (la->kind == 19) {
				Get();
			} else {
				Get();
			}
			Term();
		}
}

void Parser::Condition() {
		Expr();
		Relop();
		Expr();
}

void Parser::Term() {
		Fact();
		while (la->kind == 21 || la->kind == 22) {
			if (la->kind == 21) {
				Get();
			} else {
				Get();
			}
			Fact();
		}
}

void Parser::Fact() {
		if (la->kind == 1) {
			Get();
		} else if (la->kind == 2) {
			Get();
		} else if (la->kind == 12) {
			Get();
			Expr();
			Expect(13);
		} else SynErr(36);
}

void Parser::Relop() {
		switch (la->kind) {
		case 23: {
			Get();
			break;
		}
		case 24: {
			Get();
			break;
		}
		case 25: {
			Get();
			break;
		}
		case 26: {
			Get();
			break;
		}
		case 27: {
			Get();
			break;
		}
		case 28: {
			Get();
			break;
		}
		default: SynErr(37); break;
		}
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

	static bool set[2][33] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,x,x, x,x,x,x, x,x,x,T, x,x,T,x, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x}
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
			case 11: s = coco_string_create(L"\"print\" expected"); break;
			case 12: s = coco_string_create(L"\"(\" expected"); break;
			case 13: s = coco_string_create(L"\")\" expected"); break;
			case 14: s = coco_string_create(L"\"WHILE\" expected"); break;
			case 15: s = coco_string_create(L"\"DO\" expected"); break;
			case 16: s = coco_string_create(L"\"IF\" expected"); break;
			case 17: s = coco_string_create(L"\"THEN\" expected"); break;
			case 18: s = coco_string_create(L"\"ELSE\" expected"); break;
			case 19: s = coco_string_create(L"\"+\" expected"); break;
			case 20: s = coco_string_create(L"\"-\" expected"); break;
			case 21: s = coco_string_create(L"\"*\" expected"); break;
			case 22: s = coco_string_create(L"\"/\" expected"); break;
			case 23: s = coco_string_create(L"\"=\" expected"); break;
			case 24: s = coco_string_create(L"\"<=\" expected"); break;
			case 25: s = coco_string_create(L"\">=\" expected"); break;
			case 26: s = coco_string_create(L"\"!=\" expected"); break;
			case 27: s = coco_string_create(L"\"<\" expected"); break;
			case 28: s = coco_string_create(L"\">\" expected"); break;
			case 29: s = coco_string_create(L"\"Integer\" expected"); break;
			case 30: s = coco_string_create(L"\"Boolean\" expected"); break;
			case 31: s = coco_string_create(L"??? expected"); break;
			case 32: s = coco_string_create(L"invalid Ident"); break;
			case 33: s = coco_string_create(L"invalid Type"); break;
			case 34: s = coco_string_create(L"invalid Stat"); break;
			case 35: s = coco_string_create(L"invalid Stat"); break;
			case 36: s = coco_string_create(L"invalid Fact"); break;
			case 37: s = coco_string_create(L"invalid Relop"); break;

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


