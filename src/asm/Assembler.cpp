#include <fstream>
#include <sstream>

#include "Arguments.h"
#include "Assembler.h"
#include "BinaryOutputFormatter.h"
#include "CodeGenerator.h"
#include "Error.h"
#include "InstructionSetRegister.h"
#include "IOutputFormatter.h"
#include "Logger.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxTree.h"
#include "TextOutputFormatter.h"
#include "Token.h"

using namespace std;

Assembler::Assembler(const Arguments& arguments) :
    args(arguments)
{
    Logger::getInstance().setColorOutput(args.colorOutput);

    iSet = InstructionSetRegister::getInstance().getInstructionSet(args.instructionSetName);

    syntaxAnalyzer = new SyntaxAnalyzer(*iSet);

    codeGenerator = new CodeGenerator(*iSet);

    /// @todo Add an argument to let the user specify the output formatter.
    outputFormatter = new TextOutputFormatter;
    // outputFormatter = new BinaryOutputFormatter;
}

Assembler::~Assembler()
{
    delete syntaxAnalyzer;
    delete codeGenerator;
    delete outputFormatter;
}

bool Assembler::assemble()
{
    bool ok = true;

    try
    {
        process();
    }
    catch (const Error&)
    {
        ok = false;
    }

    return ok;
}

void Assembler::process()
{
    /////////////////////////////////
    // Preprocess
    /////////////////////////////////

    stringstream preProcStream;

    preprocessor.process(*args.is, preProcStream);

    /////////////////////////////////
    // Lexical Analyzer
    /////////////////////////////////

    vector<Token> tokens;

    lexicalAnalyzer.process(preProcStream, tokens);

    /////////////////////////////////
    // Syntax Analyzer
    /////////////////////////////////

    SyntaxTree syntaxTree;

    syntaxAnalyzer->process(tokens, syntaxTree);

    /////////////////////////////////
    // Code Generator
    /////////////////////////////////

    CodeGenerator::InstructionCodeList instCodeList;

    codeGenerator->process(syntaxTree, instCodeList);

    if (args.outputSymbols)
    {
        codeGenerator->printSymbols(*args.os);
        return;
    }

    /////////////////////////////////
    // Output Formatter
    /////////////////////////////////

    outputFormatter->generate(*args.os, iSet->getWordSize(), instCodeList);
}
