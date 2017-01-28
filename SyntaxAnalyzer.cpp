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
        // if we've reached the end of an instruction, process it
        if (token == END_OF_LINE)
        {
            if (!instTokens.empty())
            {
                InstructionCode instCode;
                encodeInstruction(instTokens, instCode);
                instCodeList.push_back(instCode);
                instTokens.clear();
            }
        }
        else
        {
            instTokens.push_back(token);
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

    instCode.push_back(code);
}
