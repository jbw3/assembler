#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <string>

#include "CodeGenerator.h"
#include "LexicalAnalyzer.h"
#include "Preprocessor.h"
#include "SyntaxAnalyzer.h"

class Assembler
{
public:
    Assembler(const std::string& filename);

    void assemble();

private:
    std::string inFilename;

    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer syntaxAnalyzer;
    CodeGenerator codeGenerator;

    void process();
};

#endif // ASSEMBLER_H_
