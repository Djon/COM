#ifdef _DEBUG
#include <vld.h>
#endif

#include <iostream>
#include <fstream>

#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>

#include "Type.h"
#include "BaseType.h"
#include "Symbol.h"
#include "VarSymbol.h"
#include "ConstIntSymbol.h"
#include "TypeSymbol.h"
#include "SymbolTable.h"
#include "SymbolFactory.h"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc >= 2) {
		MIEC::Scanner *scanner = 0;
		MIEC::Parser *parser = 0;
		ofstream myfile;
		myfile.open("MIECCompiler_result", ios::out | ios::app);

		for (int i = 1; i < argc; i++) {
			wchar_t *fileName = coco_string_create(argv[i]);
			scanner = new MIEC::Scanner(fileName);
			parser = new MIEC::Parser(scanner);
			
			//parser->gen = new MIEC::CodeGenerator();
			parser->Parse();
			/*if (parser->errors->count == 0) {
			parser->gen->Decode();
			parser->gen->Interpret("Taste.IN");
			}*/

			cout << parser->errors->count << endl;

			myfile << "MiniIEC.exe" << endl << argv[i] << ": ";
			if (parser->errors->count == 0) {
				myfile << "OK" << endl;
			}
			else {
				myfile << "FAILED: " << parser->errors->count << " error(s) detected" << endl;
			}
			
#ifdef _DEBUG
			SymbolTable::GetInstance().Print();
#endif
			coco_string_delete(fileName);
			delete parser;
			delete scanner;
			SymbolTable::GetInstance().Reset();
		}
		myfile.close();
	}
	else {
		cout << "-- No source file specified\n" << endl;
	}

	// tests without factory
	/*Type * typeInt = new BaseType(
	Type * typeSymbol = new TypeSymbol("Integer", typeInt);
	
	Type * typeChar = new BaseType(CHAR);
	cout << "Type: " << typeInt->GetName() << " Size: " << typeInt->GetSize() << endl;
	cout << "Type: " << typeChar->GetName() << " Size: " << typeChar->GetSize() << endl;

	Symbol * varSym = new VarSymbol("myInt", typeInt, 10);
	cout << "Symbol name: " << varSym->GetName()
		 << " Type: " << varSym->GetType()->GetName()
		 << " Size: " << varSym->GetType()->GetSize()
		 << " Offset: " << ((VarSymbol*)varSym)->GetOffset()
		 << endl;

	Symbol * constIntSym = new ConstIntSymbol("myConstInt", typeInt, 20);
	cout << "Symbol name: " << constIntSym->GetName()
		<< " Type: " << constIntSym->GetType()->GetName()
		<< " Size: " << constIntSym->GetType()->GetSize()
		<< " Value: " << ((ConstIntSymbol*)constIntSym)->GetValue()
		<< endl;

	SymbolTable& symTab = SymbolTable::GetInstance();
	cout << symTab.Add(varSym) << endl;
	cout << symTab.Add(constIntSym) << endl;
	cout << symTab.Add(constIntSym) << endl;

	auto findMyInt = symTab.Find("myInt");
	if (findMyInt != NULL) {
		cout << "Found: " << findMyInt->GetName() << endl;
	}
	else {
		cout << "Didnt find: " << "myInt" << endl;
	}

	auto findMyDouble = symTab.Find("myDouble");
	if (findMyDouble != NULL) {
		cout << "Found: " << findMyDouble->GetName() << endl;
	}
	else {
		cout << "Didnt find: " << "myDouble" << endl;
	}

	delete typeInt;
	delete typeChar;*/

	// tests with factory
	//SymbolFactory& symFac = SymbolFactory::GetInstance();
	/*symFac.SetIntType(symTab.GetIntType());
	symTab.Add(symFac.CreateVarIntSymbol("sum"));
	symTab.Add(symFac.CreateVarIntSymbol("factor"));
	symTab.Add(symFac.CreateConstIntSymbol("max", 100));
	symTab.Add(symFac.CreateConstIntSymbol("min", -100));*/

	//cout << symTab.Find("max")->GetName() << endl;


	//SymbolTable& symTab = SymbolTable::GetInstance();

	//Type * typeInt = new BaseType(INT);

	//Symbol * typeSymbol = new TypeSymbol("Integer", typeInt);
	//typeInt->SetNameSymbol((TypeSymbol*)typeSymbol);
	//symTab.Add(typeSymbol);

	//// without factory
	//Symbol * varIntSymbol = new VarSymbol("myInt", typeInt, 0);
	//Symbol * constIntSymbol = new ConstIntSymbol("myConstInt", typeInt, 10);
	//symTab.Add(varIntSymbol);
	//symTab.Add(constIntSymbol);

	//cout << varIntSymbol->GetName() << endl;
	//cout << varIntSymbol->GetType()->GetName() << endl;
	//cout << ((VarSymbol*)varIntSymbol)->GetOffset() << endl;

	//cout << constIntSymbol->GetName() << endl;
	//cout << constIntSymbol->GetType()->GetName() << endl;
	//cout << ((ConstIntSymbol*)constIntSymbol)->GetValue() << endl;

	//// with factory
	//SymbolFactory& symFac = SymbolFactory::GetInstance();
	//symFac.SetIntType((BaseType*)typeInt);
	//symTab.Add(symFac.CreateVarIntSymbol("myFacInt"));
	//symTab.Add(symFac.CreateConstIntSymbol("myFacConstInt", 10));

	//cout << symTab.Find("myFacInt")->GetName() << endl;
	//cout << symTab.Find("myFacInt")->GetType()->GetName() << endl;
	//cout << ((VarSymbol*)symTab.Find("myFacInt"))->GetOffset() << endl;

	//cout << symTab.Find("myFacConstInt")->GetName() << endl;
	//cout << symTab.Find("myFacConstInt")->GetType()->GetName() << endl;
	//cout << ((ConstIntSymbol*)symTab.Find("myFacConstInt"))->GetValue() << endl;

	//delete typeInt;

	return 0;
}
