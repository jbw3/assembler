#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <string>
#include <vector>

#include "InstructionSet.h"

class SyntaxTree;

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(const InstructionSet& instructionSet);

    void process(const std::vector<std::string>& tokens, SyntaxTree& syntaxTree);

private:
    InstructionSet instSet;

    void parseArgs(const std::vector<std::string>& instTokens, std::vector<std::string>& argTokens);
};

#endif // SYNTAX_ANALYZER_H_
