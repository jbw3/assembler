#include <iomanip>
#include <iostream>

#include "CodeGenerator.h"
#include "Error.h"
#include "Logger.h"
#include "SyntaxTree.h"
#include "utils.h"

using namespace std;

CodeGenerator::CodeGenerator(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{}

void CodeGenerator::process(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList)
{
    processLabels(syntaxTree);
    processInstructions(syntaxTree, instCodeList);
}

void CodeGenerator::printSymbols(ostream& os) const
{
    // save flags
    ios_base::fmtflags flags = os.flags();

    // get format widths
    int nameWidth = 0;
    int valueWidth = 0;
    for (auto pair : symbols)
    {
        int temp = pair.first.size();
        nameWidth = temp > nameWidth ? temp : nameWidth;

        temp = to_string(pair.second).size();
        valueWidth = temp > valueWidth ? temp : valueWidth;
    }

    for (auto pair : symbols)
    {
        os << left << setw(nameWidth) << pair.first << "  "
           << right << setw(valueWidth) << pair.second << "\n";
    }

    // restore flags
    os.flags(flags);
}

void CodeGenerator::processLabels(const SyntaxTree& syntaxTree)
{
    uint64_t byteWordSize = instSet.getWordSize() / 8;
    uint64_t address = 0;

    for (const InstructionTokens& tokens : syntaxTree.instructions)
    {
        // register labels (if any)
        vector<Token> labelArgs = tokens.labelArguments;
        if (!tokens.label.getValue().empty())
        {
            // if there are no arguments, assign label value to current address
            if (labelArgs.empty())
            {
                addSymbol(tokens.label, address);
            }
            else // label is being assigned a value
            {
                if (labelArgs.size() > 1)
                {
                    throwError("Invalid assignment syntax.", labelArgs[1]);
                }

                // translate value to number
                uint16_t value = evalImmediateExpression(labelArgs[0]);

                addSymbol(tokens.label, value);
            }
        }

        // increment address if there is an instruction
        if (!tokens.mnemonic.getValue().empty())
        {
            address += byteWordSize;
        }
    }
}

void CodeGenerator::addSymbol(const Token& token, std::uint64_t value)
{
    string symbolName = token.getValue();

    auto pair = symbols.insert({symbolName, value});
    if (!pair.second)
    {
        throwError("\"" + symbolName + "\" has already been defined.", token);
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
        throwError("\"" + mnemonicStr + "\" is not a valid instruction.", tokens.mnemonic);
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

        throwError(errorMsg, tokens.mnemonic);
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
        throwError(token.getValue() + " is not a valid register name.", token);
    }

    uint64_t regCode = regIter->second.getCode();
    return regCode;
}

uint64_t CodeGenerator::encodeImmediate(const Token& token, const Argument& arg)
{
    uint64_t immCode = evalImmediateExpression(token);

    // Warn if number will be truncated in instruction.
    if ( immCode != (immCode & bitMask(arg.getSize())) )
    {
        Logger::getInstance().logWarning("Immediate value \"" + token.getValue() + "\" was truncated.",
                                         token.getLine(),
                                         token.getColumn());
    }

    return immCode;
}

uint64_t CodeGenerator::evalImmediateExpression(const Token& token)
{
    uint64_t value = 0;

    const string& tokenStr = token.getValue();

    if (isdigit(tokenStr[0]))
    {
        value = evalImmediateNum(token);
    }
    else
    {
        value = evalImmediateLabel(token);
    }

    return value;
}

uint64_t CodeGenerator::evalImmediateNum(const Token& token)
{
    bool error = false;
    uint64_t value = 0;
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
        value = stoull(noPrefixToken, &pos, base);
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
        throwError(tokenStr + " is not a valid integer.", token);
    }

    return value;
}

uint64_t CodeGenerator::evalImmediateLabel(const Token& token)
{
    uint64_t value = 0;
    string label = token.getValue();

    auto iter = symbols.find(label);
    if (iter == symbols.end())
    {
        throwError("\"" + label + "\" is not a valid symbol.", token);
    }
    else
    {
        value = iter->second;
    }

    return value;
}

void CodeGenerator::throwError(const std::string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
