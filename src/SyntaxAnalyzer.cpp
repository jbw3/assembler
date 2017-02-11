#include <cstring>

#include "Error.h"
#include "Logger.h"
#include "SyntaxAnalyzer.h"
#include "SyntaxTree.h"
#include "utils.h"

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
            size_t numTokens = lineTokens.size();

            // if there are more tokens than just the END_OF_LINE
            // token, this must be an instruction
            if (numTokens > 1)
            {
                size_t tokenIdx = 0;
                InstructionTokens instTokens;

                if (numTokens > 2 && lineTokens[1] == LABEL_SEPARATOR)
                {
                    addLabel(instTokens, lineTokens[0]);
                    tokenIdx = 2;
                }

                if (tokenIdx < numTokens - 1)
                {
                    instTokens.mnemonic = lineTokens[tokenIdx];
                    ++tokenIdx;

                    parseArgs(lineTokens, tokenIdx, instTokens.arguments);
                }

                syntaxTree.instructions.push_back(instTokens);
            }

            lineTokens.clear();
        }
    }
}

void SyntaxAnalyzer::addLabel(InstructionTokens& instTokens, const Token& token)
{
    if (!isIdentifierString(token.getValue()))
    {
        throwError("\"" + token.getValue() + "\" is not a valid label name.", token);
    }

    // verify label is valid
    instTokens.label = token;
}

void SyntaxAnalyzer::parseArgs(const vector<Token>& instTokens, size_t tokenIdx, vector<Token>& argTokens)
{
    argTokens.clear();

    // if there are no arguments, we can return
    if (instTokens[tokenIdx] == END_OF_LINE)
    {
        return;
    }

    bool expectSep = false;
    for (; tokenIdx < instTokens.size(); ++tokenIdx)
    {
        const Token& token = instTokens[tokenIdx];

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
                string message = "Expected an argument before ";
                if (token == ARGUMENT_SEPARATOR)
                {
                    message += "\"" + ARGUMENT_SEPARATOR.getValue() + "\".";
                }
                else // token == END_OF_LINE
                {
                    message += "the end of the line.";
                }
                throwError(message, token);
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
