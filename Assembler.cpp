#include <fstream>
#include <sstream>

#include "Assembler.h"
#include "Error.h"

using namespace std;

Assembler::Assembler(const InstructionSet& instructionSet, ICodeGenerator* codeGenerator, std::istream& is) :
    syntaxAnalyzer(instructionSet),
    codeGenerator(codeGenerator),
    is(is)
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
    /////////////////////////////////
    // Preprocess
    /////////////////////////////////

    stringstream preProcStream;

    preprocessor.process(is, preProcStream);

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

    /////////////////////////////////
    // Code Generator
    /////////////////////////////////

    codeGenerator->generate(instCodeList);
}
