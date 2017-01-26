#include "Error.h"
#include "SyntaxAnalyzer.h"
#include "tokens.h"

using namespace std;

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

    /// @todo support instructions longer than 64-bit
    uint64_t code = instIter->second.getOpCode();

    instCode.push_back(code);
}
