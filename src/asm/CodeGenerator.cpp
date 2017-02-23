#include <iomanip>
#include <iostream>
#include <sstream>

#include "CodeGenerator.h"
#include "Error.h"
#include "Logger.h"
#include "SyntaxTree.h"
#include "utils.h"

using namespace std;

CodeGenerator::CodeGenerator(const InstructionSet& instructionSet) :
    instSet(instructionSet),
    exprEval(symbols)
{}

void CodeGenerator::process(const SyntaxTree& syntaxTree, InstructionCodeList& instCodeList)
{
    processLabels(syntaxTree);
    processInstructions(syntaxTree, instCodeList);
}

void CodeGenerator::printSymbols(ostream& os) const
{
    const string SYMBOL_HEADER = "symbol";
    const string DEC_HEADER = "dec";
    const string HEX_HEADER = "hex";

    // save state
    ios_base::fmtflags flags = os.flags();
    char fill = os.fill();

    // get format widths
    int nameWidth = SYMBOL_HEADER.size();
    int decWidth = DEC_HEADER.size();
    int hexWidth = HEX_HEADER.size();
    stringstream ss;
    for (auto pair : symbols)
    {
        int temp = pair.first.size();
        nameWidth = temp > nameWidth ? temp : nameWidth;

        // get decimal width
        ss.str(""); // clear contents
        ss << dec;
        ss << pair.second;
        temp = ss.str().size();
        decWidth = temp > decWidth ? temp : decWidth;

        // get hexadecimal width
        ss.str(""); // clear contents
        ss << hex;
        ss << pair.second;
        temp = ss.str().size();
        hexWidth = temp > hexWidth ? temp : hexWidth;
    }

    // print header
    os << setw(nameWidth) << SYMBOL_HEADER << "  "
       << setw(decWidth) << DEC_HEADER << "  "
       << setw(hexWidth) << HEX_HEADER << "\n"
       << string(nameWidth, '-') << "  "
       << string(decWidth, '-') << "  "
       << string(hexWidth, '-') << "\n";

    // print values
    for (auto pair : symbols)
    {
        // name
        os << setw(nameWidth) << setfill(' ') << pair.first << "  ";

        // dec value
        os << dec << setw(decWidth) << pair.second << "  ";

        // hex value
        os << hex << setw(hexWidth) << setfill('0') << pair.second << "\n";
    }

    // restore state
    os.flags(flags);
    os.fill(fill);
}

void CodeGenerator::processLabels(const SyntaxTree& syntaxTree)
{
    uint64_t byteWordSize = instSet.getWordSize() / 8;
    int64_t address = 0;

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
                // translate value to number
                int64_t value = exprEval.eval(labelArgs);

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

void CodeGenerator::addSymbol(const Token& token, std::int64_t value)
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
    const vector<TokenVector>& argTokens = tokens.arguments;

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

uint64_t CodeGenerator::encodeRegister(const TokenVector& tokens)
{
    if (tokens.size() > 1)
    {
        throwError("Expected only one register name.", tokens[1]);
    }

    const Token& token = tokens[0];

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

uint64_t CodeGenerator::encodeImmediate(const TokenVector& tokens, const Argument& arg)
{
    uint64_t immCode = exprEval.eval(tokens);

    // Warn if number will be truncated in instruction.
    if ( immCode != (immCode & bitMask(arg.getSize())) )
    {
        Logger::getInstance().logWarning("Immediate value was truncated.",
                                         tokens[0].getLine(),
                                         tokens[0].getColumn());
    }

    return immCode;
}

void CodeGenerator::throwError(const std::string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
