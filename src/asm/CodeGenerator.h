#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include <list>
#include <vector>

#include "ImmediateExpressionEvaluator.h"
#include "InstructionSet.h"
#include "SymbolMap.h"
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
    SymbolMap symbols;
    ImmediateExpressionEvaluator exprEval;

    void processLabels(const SyntaxTree& syntaxTree);

    void addSymbol(const Token& token, std::int64_t value);

    void processInstructions(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList);

    void encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode);

    void encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code);

    uint64_t encodeRegister(const TokenVector& tokens);

    uint64_t encodeImmediate(const TokenVector& tokens, const Argument& arg);

    /**
     * @brief Check if immediate value will be truncated
     */
    bool checkTrunc(uint64_t immCode, const Argument& arg);

    void throwError(const std::string& message, const Token& token);
};

#endif // CODE_GENERATOR_H_
