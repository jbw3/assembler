#include <fstream>
#include <sstream>

#include "Arguments.h"
#include "Assembler.h"
#include "Error.h"

using namespace std;

Assembler::Assembler(const Arguments& args) :
    syntaxAnalyzer(args.instructionSet),
    codeGenerator(args.codeGenerator),
    is(*args.is)
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

        cerr << "Error";
        if (line > 0)
        {
            cerr << ", line " << line;
        }
        cerr << ":\n";
        cerr << error.getMessage() << "\n";
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
