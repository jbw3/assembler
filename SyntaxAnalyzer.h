#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <string>
#include <vector>

#include "InstructionSet.h"

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(const InstructionSet& instructionSet);

    void process(const std::vector<std::string>& tokens);

private:
    InstructionSet instSet;
};

#endif // SYNTAX_ANALYZER_H_
