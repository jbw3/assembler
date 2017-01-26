#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <list>
#include <string>
#include <vector>

#include "InstructionSet.h"

class SyntaxAnalyzer
{
public:
    typedef std::vector<std::uint64_t> InstructionCode;
    typedef std::list<InstructionCode> InstructionCodeList;

    SyntaxAnalyzer(const InstructionSet& instructionSet);

    bool process(const std::vector<std::string>& tokens, InstructionCodeList& instCodeList);

private:
    InstructionSet instSet;

    bool encodeInstruction(const std::vector<std::string>& instTokens, InstructionCode& instCode);
};

#endif // SYNTAX_ANALYZER_H_
