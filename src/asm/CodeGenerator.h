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

    void printSymbols(std::ostream& os) const;

private:
    InstructionSet instSet;
    std::map<std::string, std::uint64_t> symbols;

    void processLabels(const SyntaxTree& syntaxTree);

    void addSymbol(const Token& token, std::uint64_t value);

    void processInstructions(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList);

    void encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode);

    void encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code);

    uint64_t encodeRegister(const Token& token);

    uint64_t encodeImmediate(const Token& token, const Argument& arg);

    uint64_t evalImmediateExpression(const Token& token);

    uint64_t evalImmediateNum(const Token& token);

    uint64_t evalImmediateLabel(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // CODE_GENERATOR_H_
