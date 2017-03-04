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

    if (is != &cin)
    {
        delete is;
    }

    if (os != &cout)
    {
        delete os;
    }
}

bool Assembler::assemble()
{
    bool ok = configIO();

    if (ok)
    {
        try
        {
            process();
        }
        catch (const Error&)
        {
            ok = false;
        }
    }

    return ok;
}

bool Assembler::configIO()
{
    bool ok = true;

    // --- Input ---

    if (args.inFilename.empty())
    {
        is = &cin;
    }
    else
    {
        fstream* inFile = new fstream;
        inFile->open(args.inFilename, ios_base::in);

        is = inFile;
        if (is->fail())
        {
            cerr << "Error: Could not open file \"" << args.inFilename << "\".\n";
            ok = false;
        }
    }

    // --- Output ---

    if (args.outFilename.empty())
    {
        os = &cout;
    }
    else
    {
        fstream* outFile = new fstream;
        outFile->open(args.outFilename, ios_base::out);

        os = outFile;
        if (os->fail())
        {
            cerr << "Error: Could not open file \"" << args.outFilename << "\".\n";
            ok = false;
        }
    }

    return ok;
}

void Assembler::process()
{
    /////////////////////////////////
    // Preprocess
    /////////////////////////////////

    stringstream preProcStream;

    preprocessor.process(*is, preProcStream);

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
        codeGenerator->printSymbols(*os);
        return;
    }

    /////////////////////////////////
    // Output Formatter
    /////////////////////////////////

    outputFormatter->generate(*os, iSet->getWordSize(), instCodeList);
}
