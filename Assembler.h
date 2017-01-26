#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <string>

#include "ICodeGenerator.h"
#include "LexicalAnalyzer.h"
#include "Preprocessor.h"
#include "SyntaxAnalyzer.h"

class Assembler
{
public:
    Assembler(const InstructionSet& instructionSet, ICodeGenerator* codeGenerator, const std::string& filename);

    void assemble();

private:
    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer syntaxAnalyzer;
    ICodeGenerator* codeGenerator;

    std::string inFilename;

    void process();
};

#endif // ASSEMBLER_H_
