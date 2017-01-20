#include "ICodeGenerator.h"
#include "ILexicalAnalyzer.h"
#include "ISyntaxAnalyzer.h"

class Assembler
{
public:
    Assembler();

private:
    ILexicalAnalyzer* lexicalAnalyzer;
    ISyntaxAnalyzer* syntaxAnalyzer;
    ICodeGenerator* codeGenerator;
};
