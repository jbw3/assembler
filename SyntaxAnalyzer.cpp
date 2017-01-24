#include <iostream>

#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{
}

void SyntaxAnalyzer::process(const std::vector<std::string>& tokens)
{
    for (auto x : instSet.getInstructions())
    {
        std::cout << x.first << '\n';
    }
}
