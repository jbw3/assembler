#include <cstring>

#include "Error.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxTree.h"
#include "tokens.h"

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{}

void SyntaxAnalyzer::process(const vector<string>& tokens, SyntaxTree& syntaxTree)
{
    vector<string> lineTokens;
    lineTokens.reserve(8);

    for (string token : tokens)
    {
        lineTokens.push_back(token);

        // if we've reached the end of an instruction, process it
        if (token == END_OF_LINE)
        {
            // if there are more tokens than just the END_OF_LINE
            // token, this must be an instruction
            if (lineTokens.size() > 1)
            {
                InstructionTokens instTokens;
                instTokens.mnemonic = lineTokens[0];

                parseArgs(lineTokens, instTokens.arguments);

                syntaxTree.instructions.push_back(instTokens);
            }

            lineTokens.clear();
        }
    }
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
