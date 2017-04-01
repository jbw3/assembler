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
    int64_t startAddress;
    int64_t address;
    bool isStartAddressDefined;
    bool isConstantDefined;

    void processConstants(const SyntaxTree& syntaxTree);

    void addSymbol(const Token& token, std::int64_t value);

    void processInstructions(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList);

    void encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode);

    void addCodes(const Instruction& inst, uint64_t& instCode);

    void encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code);

    uint64_t encodeRegister(const TokenVec& tokens);

    uint64_t encodeImmediate(const TokenVec& tokens, const Argument& arg);

    /**
     * @brief Check if immediate value will be truncated
     */
    bool checkTrunc(int64_t immCode, int64_t exprValue, const Argument& arg);

    void addField(const FieldType& field, std::uint64_t value, uint64_t& code);

    void throwError(const std::string& message, const Token& token);
};

#endif // CODE_GENERATOR_H_
