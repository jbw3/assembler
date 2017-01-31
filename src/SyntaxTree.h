#ifndef SYNTAX_TREE_H_
#define SYNTAX_TREE_H_

#include <string>
#include <vector>

struct InstructionTokens
{
    std::string mnemonic;
    std::vector<std::string> arguments;
};

class SyntaxTree
{
public:
    SyntaxTree();

    std::vector<InstructionTokens> instructions;
};

#endif // SYNTAX_TREE_H_
