#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <vector>

#include "InstructionSet.h"
#include "Token.h"

class SyntaxTree;

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(const InstructionSet& instructionSet);

    void process(const std::vector<Token>& tokens, SyntaxTree& syntaxTree);

private:
    InstructionSet instSet;

    void parseArgs(const std::vector<Token>& instTokens, std::vector<Token>& argTokens);
};

#endif // SYNTAX_ANALYZER_H_
