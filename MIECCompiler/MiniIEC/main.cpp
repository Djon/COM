#ifdef _DEBUG
//#include <vld.h>
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

			cout << "Parser Errors: " << parser->errors->count << endl;

			myfile << "MiniIEC.exe" << endl << argv[i] << ": ";
			if (parser->errors->count == 0) {
				myfile << "OK" << endl;
			}
			else {
				myfile << "FAILED: " << parser->errors->count << " error(s) detected" << endl;
			}
			
#ifdef _DEBUG
			std::cout << std::endl << "SymbolTable:" << std::endl;
			parser->mDACGen.PrintSymTab();
			std::cout << std::endl << "DACEntries: " << std::endl;
			parser->mDACGen.PrintDACList();
#endif
			coco_string_delete(fileName);
			delete parser;
			delete scanner;
		}
		myfile.close();
	}
	else {
		cout << "-- No source file specified\n" << endl;
	}

	return 0;
}
