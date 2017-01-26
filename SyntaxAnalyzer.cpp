#include <iostream>

#include "SyntaxAnalyzer.h"

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{
}

bool SyntaxAnalyzer::process(const vector<string>& tokens, InstructionCodeList& instCodeList)
{
    bool ok = true;
    vector<string> instTokens;
    instTokens.reserve(8);

    // make sure list is empty
    instCodeList.clear();

    for (string token : tokens)
    {
        // if we've reached the end of an instruction, process it
        if (token == "\n")
        {
            if (!instTokens.empty())
            {
                InstructionCode instCode;
                ok = encodeInstruction(instTokens, instCode);
                if (!ok)
                {
                    break;
                }

                instCodeList.push_back(instCode);
                instTokens.clear();
            }
        }
        else
        {
            instTokens.push_back(token);
        }
    }

    return ok;
}

bool SyntaxAnalyzer::encodeInstruction(const vector<string>& instTokens, InstructionCode& instCode)
{
    map<string, Instruction> instructions = instSet.getInstructions();

    // first token is instruction mnemonic
    string name = instTokens[0];
    auto instIter = instructions.find(name);
    if (instIter == instructions.end())
    {
        cout << "Error: Unknown instruction \"" << name << "\"\n";
        return false;
    }

    /// @todo support instructions longer than 64-bit
    uint64_t code = instIter->second.getOpCode();
    cout << "code: " << code << "\n";

    instCode.push_back(code);

    return true;
}
