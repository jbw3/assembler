#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include <list>
#include <string>
#include <vector>

#include "InstructionSet.h"

class SyntaxTree;
struct InstructionTokens;

class CodeGenerator
{
public:
    typedef std::vector<std::uint64_t> InstructionCode;
    typedef std::list<InstructionCode> InstructionCodeList;

    CodeGenerator(const InstructionSet& instructionSet);

    void process(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList);

private:
    InstructionSet instSet;

    void encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode);

    void encodeArgs(const Instruction& inst, const std::vector<std::string>& argTokens, uint64_t& code);

    uint64_t encodeRegister(const std::string& token);

    uint64_t encodeImmediate(const std::string& token);
};

#endif // CODE_GENERATOR_H_
