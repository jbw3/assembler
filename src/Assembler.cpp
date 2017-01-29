#include <fstream>
#include <sstream>

#include "Arguments.h"
#include "Assembler.h"
#include "Error.h"
#include "TextCodeGenerator.h"

using namespace std;

Assembler::Assembler(const Arguments& args) :
    is(*args.is)
{
    const InstructionSet* iSet = InstructionSet::getInstructionSet(args.instructionSetName);

    syntaxAnalyzer = new SyntaxAnalyzer(*iSet);

    /// @todo Add an argument to let the user specify the code generator.
    codeGenerator = new TextCodeGenerator(*args.os, iSet->getWordSize());
}

Assembler::~Assembler()
{
    delete syntaxAnalyzer;
    delete codeGenerator;
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

    syntaxAnalyzer->process(tokens, instCodeList);

    /////////////////////////////////
    // Code Generator
    /////////////////////////////////

    codeGenerator->generate(instCodeList);
}
