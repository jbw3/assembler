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
    std::map<std::string, std::uint64_t> symbols;

    void processLabels(const SyntaxTree& syntaxTree);

    void processInstructions(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList);

    void encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode);

    void encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code);

    uint64_t encodeRegister(const Token& token);

    uint64_t encodeImmediate(const Token& token, const Argument& arg);

    uint64_t encodeImmediateNum(const Token& token);

    uint64_t encodeImmediateLabel(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // CODE_GENERATOR_H_