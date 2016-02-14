#ifdef _DEBUG
#include <vld.h>
#endif

#include <iostream>
#include <fstream>
#include <string>

#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>

#include "CodeGenerator.h"

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
			
			
			std::wstring wname = std::wstring(fileName);
			std::string name = std::string(wname.begin(), wname.end());
			cout << "Parsing " << name << " now..." << endl;
			parser->Parse();

			// output error count
			size_t const parserErrors = parser->errors->count;
			size_t const semanticErrors = parser->mDACGen.GetErrorCount();
			cout << "Parser Errors: " << parserErrors << endl;
			cout << "Semantic Errors: " << semanticErrors << endl;

			// write to log file
			myfile << "MiniIEC.exe" << endl << argv[i] << ": ";
			if (parserErrors == 0) {
				myfile << "OK" << endl;
			}
			else {
				myfile << "FAILED: " << parserErrors << " error(s) detected" << endl;
			}
			
			
#ifdef _DEBUG
			// output symbol table and DAC entries
			std::cout << std::endl << "SymbolTable:" << std::endl;
			parser->mDACGen.PrintSymTab();
			std::cout << std::endl << "DACEntries: " << std::endl;
			parser->mDACGen.PrintDACList();
			cout << endl;
#endif

			// generate machine code
			if (parserErrors == 0 && semanticErrors == 0) {
				MIEC::CodeGenerator codeGenerator(parser->mDACGen.GetDACList());
				coco_string_merge(fileName, L".iex");
				codeGenerator.GenerateCode(fileName);
			}

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
