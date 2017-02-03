#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include <list>
#include <vector>

#include "InstructionSet.h"
#include "Token.h"

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

    void encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code);

    uint64_t encodeRegister(const Token& token);

    uint64_t encodeImmediate(const Token& token, const Argument& arg);
};

#endif // CODE_GENERATOR_H_
