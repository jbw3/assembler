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

    void process(const std::vector<std::string>& tokens, InstructionCodeList& instCodeList);

private:
    InstructionSet instSet;

    void encodeInstruction(const std::vector<std::string>& instTokens, InstructionCode& instCode);

    void parseArgs(const std::vector<std::string>& instTokens, std::vector<std::string>& argTokens);

    void encodeArgs(const Instruction& inst, const std::vector<std::string>& argTokens, uint64_t& code);

    uint64_t encodeRegister(const std::string& token);

    uint64_t encodeImmediate(const std::string& token);
};

#endif // SYNTAX_ANALYZER_H_
