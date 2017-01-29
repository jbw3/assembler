#include "Error.h"
#include "SyntaxAnalyzer.h"
#include "tokens.h"

using namespace std;

/**
 * @brief Create a bitmask of the given size.
 * @details For example, a bitmask of size 3 is 0b111.
 *
 * @param size the size in bits
 * @return the bitmask
 */
constexpr uint64_t bitMask(unsigned int size)
{
    uint64_t mask = 1;
    mask <<= size;
    mask -= 1;
    return mask;
}

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{
}

void SyntaxAnalyzer::process(const vector<string>& tokens, InstructionCodeList& instCodeList)
{
    vector<string> instTokens;
    instTokens.reserve(8);

    // make sure list is empty
    instCodeList.clear();

    for (string token : tokens)
    {
        instTokens.push_back(token);

        // if we've reached the end of an instruction, process it
        if (token == END_OF_LINE)
        {
            if (instTokens.size() > 1)
            {
                InstructionCode instCode;
                encodeInstruction(instTokens, instCode);
                instCodeList.push_back(instCode);
            }

            instTokens.clear();
        }
    }
}

/// @todo support instructions longer than 64-bit
void SyntaxAnalyzer::encodeInstruction(const vector<string>& instTokens, InstructionCode& instCode)
{
    map<string, Instruction> instructions = instSet.getInstructions();

    // first token is instruction mnemonic
    string name = instTokens[0];
    auto instIter = instructions.find(name);
    if (instIter == instructions.end())
    {
        throw Error("Unknown instruction \"" + name + "\"");
    }

    const Instruction& inst = instIter->second;
    const InstructionType& instType = inst.getType();

    // op code
    uint64_t code = inst.getOpCode();
    code &= bitMask(instType.getOpCodeSize());
    code <<= instType.getOpCodeOffset();

    // arguments
    vector<string> argTokens;
    parseArgs(instTokens, argTokens);
    encodeArgs(inst, argTokens, code);

    instCode.push_back(code);
}

void SyntaxAnalyzer::parseArgs(const vector<string>& instTokens, vector<string>& argTokens)
{
    argTokens.clear();

    // if there are no arguments, we can return
    if (instTokens[1] == END_OF_LINE)
    {
        return;
    }

    bool expectSep = false;
    for (size_t i = 1; i < instTokens.size(); ++i)
    {
        const string& token = instTokens[i];

        if (expectSep)
        {
            if (token != ARGUMENT_SEPARATOR && token != END_OF_LINE)
            {
                throw Error("Did not expect argument.");
            }
        }
        else
        {
            if (token == ARGUMENT_SEPARATOR || token == END_OF_LINE)
            {
                throw Error("Expected argument.");
            }
            else
            {
                argTokens.push_back(token);
            }
        }

        expectSep = !expectSep;
    }
}

void SyntaxAnalyzer::encodeArgs(const Instruction& inst, const std::vector<std::string>& argTokens, uint64_t& code)
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
}
