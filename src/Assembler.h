#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <istream>

#include "ICodeGenerator.h"
#include "LexicalAnalyzer.h"
#include "Preprocessor.h"
#include "SyntaxAnalyzer.h"

class Arguments;

class Assembler
{
public:
    Assembler(const Arguments& args);

    void assemble();

private:
    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer syntaxAnalyzer;
    ICodeGenerator* codeGenerator;

    std::istream& is;

    void process();
};

#endif // ASSEMBLER_H_