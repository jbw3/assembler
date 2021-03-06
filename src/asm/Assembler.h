#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <istream>

#include "LexicalAnalyzer.h"
#include "Preprocessor.h"
#include "SyntaxAnalyzer.h"

class Arguments;
class CodeGenerator;
class InstructionSet;
class IOutputFormatter;
class SyntaxAnalyzer;

class Assembler
{
public:
    Assembler(const Arguments& arguments);

    ~Assembler();

    bool assemble();

private:
    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer syntaxAnalyzer;
    CodeGenerator* codeGenerator;
    IOutputFormatter* outputFormatter;

    const Arguments& args;
    const InstructionSet* iSet;
    std::istream* is;
    std::ostream* os;

    bool configIO();

    void process();
};

#endif // ASSEMBLER_H_
