#include <iostream>

#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{
}

void SyntaxAnalyzer::process(const std::vector<std::string>& tokens)
{
    std::cout << "Registers:\n";
    for (auto x : instSet.getRegisters())
    {
        std::cout << x.first << '\n';
    }

    std::cout << "Instructions:\n";
    for (auto x : instSet.getInstructions())
    {
        std::cout << x.first << '\n';
    }
}
