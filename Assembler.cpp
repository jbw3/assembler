#include <fstream>
#include <sstream>

#include "Assembler.h"
#include "Error.h"
#include "InstructionSet8Bit.h"

using namespace std;

Assembler::Assembler(const std::string& filename) :
    inFilename(filename),
    syntaxAnalyzer(ISET_8_BIT)
{
}

void Assembler::assemble()
{
    try
    {
        process();
    }
    catch (const Error& error)
    {
        long long line = error.getLine();

        cout << "Error";
        if (line > 0)
        {
            cout << ", line " << line;
        }
        cout << ":\n";
        cout << error.getMessage() << "\n";
    }
}

void Assembler::process()
{
    fstream inFile;

    /////////////////////////////////
    // Preprocess
    /////////////////////////////////

    inFile.open(inFilename, ios_base::in);
    stringstream preProcStream;

    preprocessor.process(inFile, preProcStream);

    inFile.close();

    /////////////////////////////////
    // Lexical Analyzer
    /////////////////////////////////

    vector<string> tokens;

    lexicalAnalyzer.process(preProcStream, tokens);

    /////////////////////////////////
    // Syntax Analyzer
    /////////////////////////////////

    SyntaxAnalyzer::InstructionCodeList instCodeList;

    syntaxAnalyzer.process(tokens, instCodeList);

    for (auto code : instCodeList)
    {
        cout << hex << code[0] << dec << "\n";
    }
}
