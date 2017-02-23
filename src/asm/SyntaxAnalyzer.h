#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <vector>

#include "InstructionSet.h"
#include "Token.h"

struct InstructionTokens;
class SyntaxTree;

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(const InstructionSet& instructionSet);

    void process(const TokenVector& tokens, SyntaxTree& syntaxTree);

private:
    InstructionSet instSet;

    void addLabel(InstructionTokens& instTokens, const Token& token);

    void parseArgs(const TokenVector& instTokens, std::size_t tokenIdx, std::vector<TokenVector>& args);

    void throwError(const std::string& message, const Token& token);
};

#endif // SYNTAX_ANALYZER_H_
