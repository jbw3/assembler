#include "CodeGenerator.h"
#include "Error.h"
#include "SyntaxTree.h"
#include "utils.h"

using namespace std;

CodeGenerator::CodeGenerator(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{}

void CodeGenerator::process(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList)
{
    // make sure list is empty
    instCodeList.clear();

    // encode each instruction
    for (const InstructionTokens& tokens : syntaxTree.instructions)
    {
        InstructionCode instCode;
        encodeInstruction(tokens, instCode);
        instCodeList.push_back(instCode);
    }
}

/// @todo support instructions longer than 64-bit
void CodeGenerator::encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode)
{
    map<string, Instruction> instructions = instSet.getInstructions();

    string mnemonicUpper = toUpper(tokens.mnemonic);
    auto instIter = instructions.find(mnemonicUpper);
    if (instIter == instructions.end())
    {
        throw Error("Unknown instruction \"" + tokens.mnemonic + "\"");
    }

    const Instruction& inst = instIter->second;
    const InstructionType& instType = inst.getType();

    // op code
    uint64_t code = inst.getOpCode();
    code &= bitMask(instType.getOpCodeSize());
    code <<= instType.getOpCodeOffset();

    // arguments
    vector<string> argTokens;
    encodeArgs(inst, tokens.arguments, code);

    instCode.push_back(code);
}

void CodeGenerator::encodeArgs(const Instruction& inst, const std::vector<std::string>& argTokens, uint64_t& code)
{
    const vector<Argument>& args = inst.getType().getArguments();

    size_t expectedNumArgs = args.size();
    size_t numArgs = argTokens.size();
    if (numArgs != expectedNumArgs)
    {
        string errorMsg = "Expected " + to_string(expectedNumArgs);
        errorMsg += expectedNumArgs == 1 ? " argument. " : " arguments. ";
        errorMsg += "Got " + to_string(numArgs);
        errorMsg += numArgs == 1 ? " argument." : " arguments.";
        throw Error(errorMsg);
    }

    // encode arguments
    for (size_t i = 0; i < numArgs; ++i)
    {
        const Argument& arg = args[i];
        uint64_t argCode = 0;
        switch (arg.getType())
        {
        case Argument::eRegister:
            argCode = encodeRegister(argTokens[i]);
            break;

        case Argument::eImmediate:
            argCode = encodeImmediate(argTokens[i]);
            break;

        default:
            throw Error("Internal error! Unknown argument type: " + to_string(arg.getType()));
            break;
        }

        // add arg code to instruction code
        argCode &= bitMask(arg.getSize());
        argCode <<= arg.getOffset();

        code |= argCode;
    }
}

uint64_t CodeGenerator::encodeRegister(const string& token)
{
    map<string, Register> registers = instSet.getRegisters();

    // look up the register by name
    string tokenUpper = toUpper(token);
    auto regIter = registers.find(tokenUpper);
    if (regIter == registers.cend())
    {
        throw Error(token + " is not a valid register name.");
    }

    uint64_t regCode = regIter->second.getCode();
    return regCode;
}

uint64_t CodeGenerator::encodeImmediate(const string& token)
{
    bool error = false;
    uint64_t immCode = 0;
    size_t pos = 0;

    // determine base
    int base = 10;
    if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
    {
        base = 16;
    }

    // try to convert the string to an integer
    try
    {
        immCode = stoull(token, &pos, base);
    }
    catch (invalid_argument)
    {
        error = true;
    }
    catch (out_of_range)
    {
        error = true;
    }

    // make sure the entire string was used
    if (pos != token.size())
    {
        error = true;
    }

    if (error)
    {
        throw Error(token + " is not a valid integer.");
    }

    /// @todo warn if number will be truncated in instruction

    return immCode;
}
