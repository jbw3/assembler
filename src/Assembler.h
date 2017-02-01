#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <istream>

#include "LexicalAnalyzer.h"
#include "Preprocessor.h"

class Arguments;
class CodeGenerator;
class IOutputFormatter;
class SyntaxAnalyzer;

class Assembler
{
public:
    Assembler(const Arguments& args);

    ~Assembler();

    bool assemble();

private:
    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer* syntaxAnalyzer;
    CodeGenerator* codeGenerator;
    IOutputFormatter* outputFormatter;

    std::istream& is;

    void process();
};

#endif // ASSEMBLER_H_
