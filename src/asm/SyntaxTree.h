#ifndef SYNTAX_TREE_H_
#define SYNTAX_TREE_H_

#include <vector>

#include "Token.h"

struct InstructionTokens
{
    Token label;
    TokenVec labelArguments;
    Token mnemonic;
    TokenVecVec arguments;
};

class SyntaxTree
{
public:
    SyntaxTree();

    std::vector<InstructionTokens> instructions;
};

#endif // SYNTAX_TREE_H_
