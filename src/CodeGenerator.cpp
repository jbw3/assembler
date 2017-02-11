#include <iomanip>
#include <iostream>

#include "CodeGenerator.h"
#include "Error.h"
#include "Logger.h"
#include "SyntaxTree.h"
#include "utils.h"

#define PRINT_SYMBOLS 0

using namespace std;

CodeGenerator::CodeGenerator(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{}

void CodeGenerator::process(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList)
{
    processLabels(syntaxTree);
    processInstructions(syntaxTree, instCodeList);

#if PRINT_SYMBOLS
    cout << left;
    for (auto pair : symbols)
    {
        cout << setw(12) << pair.first << " = " << pair.second << "\n";
    }
    cout << right;
#endif
}

void CodeGenerator::processLabels(const SyntaxTree& syntaxTree)
{
    uint64_t byteWordSize = instSet.getWordSize() / 8;
    uint64_t address = 0;

    for (const InstructionTokens& tokens : syntaxTree.instructions)
    {
        // register labels (if any)
        string label = tokens.label.getValue();
        if (!label.empty())
        {
            auto pair = symbols.insert({label, address});
            if (!pair.second)
            {
                throwError("\"" + label + "\" has already been defined.", tokens.label);
            }
        }

        // increment address if there is an instruction
        if (!tokens.mnemonic.getValue().empty())
        {
            address += byteWordSize;
        }
    }
}

void CodeGenerator::processInstructions(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList)
{
    // make sure list is empty
    instCodeList.clear();

    for (const InstructionTokens& tokens : syntaxTree.instructions)
    {
        // encode instruction (if there is one)
        if (!tokens.mnemonic.getValue().empty())
        {
            InstructionCode instCode;
            encodeInstruction(tokens, instCode);
            instCodeList.push_back(instCode);
        }
    }
}

/// @todo support instructions longer than 64-bit
void CodeGenerator::encodeInstruction(const InstructionTokens& tokens, InstructionCode& instCode)
{
    map<string, Instruction> instructions = instSet.getInstructions();

    const string& mnemonicStr = tokens.mnemonic.getValue();
    string mnemonicUpper = toUpper(mnemonicStr);
    auto instIter = instructions.find(mnemonicUpper);
    if (instIter == instructions.end())
    {
        Logger::getInstance().logError("Unknown instruction \"" + mnemonicStr + "\"",
                                       tokens.mnemonic.getLine(),
                                       tokens.mnemonic.getColumn());
        throw Error();
    }

    const Instruction& inst = instIter->second;
    const InstructionType& instType = inst.getType();

    // op code
    uint64_t code = inst.getOpCode();
    code &= bitMask(instType.getOpCodeSize());
    code <<= instType.getOpCodeOffset();

    // arguments
    encodeArgs(inst, tokens, code);

    instCode.push_back(code);
}

void CodeGenerator::encodeArgs(const Instruction& inst, const InstructionTokens& tokens, uint64_t& code)
{
    const vector<Argument>& args = inst.getType().getArguments();
    const vector<Token>& argTokens = tokens.arguments;

    size_t expectedNumArgs = args.size();
    size_t numArgs = argTokens.size();
    if (numArgs != expectedNumArgs)
    {
        string errorMsg = "Expected " + to_string(expectedNumArgs);
        errorMsg += expectedNumArgs == 1 ? " argument. " : " arguments. ";
        errorMsg += "Got " + to_string(numArgs);
        errorMsg += numArgs == 1 ? " argument." : " arguments.";
        Logger::getInstance().logError(errorMsg,
                                       tokens.mnemonic.getLine(),
                                       tokens.mnemonic.getColumn());
        throw Error();
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
            argCode = encodeImmediate(argTokens[i], arg);
            break;

        default:
            Logger::getInstance().logError("Internal error! Unknown argument type: " + to_string(arg.getType()));
            throw Error();
            break;
        }

        // add arg code to instruction code
        argCode &= bitMask(arg.getSize());
        argCode <<= arg.getOffset();

        code |= argCode;
    }
}

uint64_t CodeGenerator::encodeRegister(const Token& token)
{
    map<string, Register> registers = instSet.getRegisters();

    // look up the register by name
    string tokenUpper = toUpper(token.getValue());
    auto regIter = registers.find(tokenUpper);
    if (regIter == registers.cend())
    {
        Logger::getInstance().logError(token.getValue() + " is not a valid register name.",
                                       token.getLine(),
                                       token.getColumn());
        throw Error();
    }

    uint64_t regCode = regIter->second.getCode();
    return regCode;
}

uint64_t CodeGenerator::encodeImmediate(const Token& token, const Argument& arg)
{
    uint64_t immCode = 0;

    const string& tokenStr = token.getValue();

    if (isdigit(tokenStr[0]))
    {
        immCode = encodeImmediateNum(token);
    }
    else
    {
        immCode = encodeImmediateLabel(token);
    }

    // Warn if number will be truncated in instruction.
    if ( immCode != (immCode & bitMask(arg.getSize())) )
    {
        Logger::getInstance().logWarning("Immediate value \"" + tokenStr + "\" was truncated.",
                                         token.getLine(),
                                         token.getColumn());
    }

    return immCode;
}

uint64_t CodeGenerator::encodeImmediateNum(const Token& token)
{
    bool error = false;
    uint64_t immCode = 0;
    size_t pos = 0;

    const string& tokenStr = token.getValue();

    // determine base
    int base = 10;
    if (tokenStr.size() >= 2 && tokenStr[0] == '0')
    {
        switch (tokenStr[1])
        {
        case 'b':
        case 'B':
            base = 2;
            break;

        case 'o':
        case 'O':
            base = 8;
            break;

        case 'x':
        case 'X':
            base = 16;
            break;

        default:
            base = 10;
            break;
        }
    }

    // strip prefix if not a decimal number
    string noPrefixToken = (base == 10) ? tokenStr : tokenStr.substr(2);

    // try to convert the string to an integer
    try
    {
        immCode = stoull(noPrefixToken, &pos, base);
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
    if (pos != noPrefixToken.size())
    {
        error = true;
    }

    if (error)
    {
        Logger::getInstance().logError(tokenStr + " is not a valid integer.",
                                       token.getLine(),
                                       token.getColumn());
        throw Error();
    }

    return immCode;
}

uint64_t CodeGenerator::encodeImmediateLabel(const Token& token)
{
    uint64_t immCode = 0;
    string label = token.getValue();

    auto iter = symbols.find(label);
    if (iter == symbols.end())
    {
        throwError("\"" + label + "\" is not a valid symbol.", token);
    }
    else
    {
        immCode = iter->second;
    }

    return immCode;
}

void CodeGenerator::throwError(const std::string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
