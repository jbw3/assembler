#ifndef SYNTAX_TREE_H_
#define SYNTAX_TREE_H_

#include <vector>

#include "Token.h"

struct InstructionTokens
{
    Token label;
    TokenVector labelArguments;
    Token mnemonic;
    std::vector<TokenVector> arguments;
};

class SyntaxTree
{
public:
    SyntaxTree();

    std::vector<InstructionTokens> instructions;
};

#endif // SYNTAX_TREE_H_
