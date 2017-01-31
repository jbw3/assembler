#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <istream>

#include "LexicalAnalyzer.h"
#include "Preprocessor.h"
#include "SyntaxAnalyzer.h"

class IOutputFormatter;

class Arguments;

class Assembler
{
public:
    Assembler(const Arguments& args);

    ~Assembler();

    void assemble();

private:
    Preprocessor preprocessor;
    LexicalAnalyzer lexicalAnalyzer;
    SyntaxAnalyzer* syntaxAnalyzer;
    IOutputFormatter* outputFormatter;

    std::istream& is;

    void process();
};

#endif // ASSEMBLER_H_
