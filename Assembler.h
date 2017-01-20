#include "CodeGenerator.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

class Assembler
{
public:
    Assembler();

private:
    LexicalAnalyzer* lexicalAnalyzer;
    SyntaxAnalyzer* syntaxAnalyzer;
    CodeGenerator* codeGenerator;
};
