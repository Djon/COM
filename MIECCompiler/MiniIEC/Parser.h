

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include "DACGenerator.h"
#include "SymbolFactory.h"
#include <wchar.h>


#include "Scanner.h"

namespace MIEC {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ident=1,
		_number=2
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

DACGenerator mDACGen;

	void Error(wchar_t* const msg) {
		errors->Error(la->line, la->col, msg);
	}



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void MIEC();
	void VarDeclList();
	void Statements();
	void VarDecl(size_t &addr);
	void Ident(wchar_t* &name);
	void Type(DataType* &pType);
	void Stat();
	void Assignment(Symbol* &pSym);
	void Branch(Symbol* &pSym);
	void Loop(Symbol* &pSym);
	void Print(Symbol* &pSym);
	void Expr(Symbol* &pSym);
	void Condition(Symbol* &pSym);
	void Relop(OpKind &op);
	void Term(Symbol* &pSym);
	void Fact(Symbol* &pSym);

	void Parse();

}; // end Parser

} // namespace


#endif // !defined(COCO_PARSER_H__)

