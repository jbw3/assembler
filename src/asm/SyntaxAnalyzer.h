#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <vector>

#include "Token.h"

struct InstructionTokens;
class SyntaxTree;

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer() = default;

    void process(const TokenVec& tokens, SyntaxTree& syntaxTree);

private:
    void addLabel(InstructionTokens& instTokens, const Token& token);

    void parseArgs(const TokenVec& instTokens, std::size_t tokenIdx, TokenVecVec& args);

    void throwError(const std::string& message, const Token& token);
};

#endif // SYNTAX_ANALYZER_H_
