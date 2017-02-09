#include <cstring>

#include "Error.h"
#include "Logger.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxTree.h"

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(const InstructionSet& instructionSet) :
    instSet(instructionSet)
{}

void SyntaxAnalyzer::process(const vector<Token>& tokens, SyntaxTree& syntaxTree)
{
    vector<Token> lineTokens;
    lineTokens.reserve(8);

    for (const Token& token : tokens)
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

void SyntaxAnalyzer::parseArgs(const vector<Token>& instTokens, vector<Token>& argTokens)
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
        const Token& token = instTokens[i];

        if (expectSep)
        {
            if (token != ARGUMENT_SEPARATOR && token != END_OF_LINE)
            {
                string message = "Expected \"" + ARGUMENT_SEPARATOR.getValue() + "\" before argument \"" + token.getValue() + "\".";
                throwError(message, token);
            }
        }
        else
        {
            if (token == ARGUMENT_SEPARATOR || token == END_OF_LINE)
            {
                throwError("Expected argument.", token);
            }
            else
            {
                argTokens.push_back(token);
            }
        }

        expectSep = !expectSep;
    }
}

void SyntaxAnalyzer::throwError(const string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
