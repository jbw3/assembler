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

void SyntaxAnalyzer::process(const TokenVec& tokens, SyntaxTree& syntaxTree)
{
    TokenVec lineTokens;
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
                InstructionTokens instTokens;

                if (numTokens > 2 && lineTokens[1] == ASSIGNMENT_OPERATOR)
                {
                    addLabel(instTokens, lineTokens[0]);

                    // add all tokens after the assignment operator and before
                    // the end of the line
                    for (size_t i = 2; i < lineTokens.size() - 1; ++i)
                    {
                        instTokens.labelArguments.push_back(lineTokens[i]);
                    }

                    // error if no arguments were added
                    if (instTokens.labelArguments.empty())
                    {
                        throwError("Expected an expression after \"" + lineTokens[1].getValue() + "\".", lineTokens[1]);
                    }
                }
                else
                {
                    size_t tokenIdx = 0;

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
                }

                syntaxTree.instructions.push_back(instTokens);
            }

            lineTokens.clear();
        }
    }
}

void SyntaxAnalyzer::addLabel(InstructionTokens& instTokens, const Token& token)
{
    // verify label is valid
    if (!isIdentifierString(token.getValue()))
    {
        throwError("\"" + token.getValue() + "\" is not a valid label name.", token);
    }

    instTokens.label = token;
}

void SyntaxAnalyzer::parseArgs(const TokenVec& instTokens, size_t tokenIdx, TokenVecVec& args)
{
    args.clear();

    // if there are no arguments, we can return
    if (instTokens[tokenIdx] == END_OF_LINE)
    {
        return;
    }

    TokenVec argTokens;
    for (; tokenIdx < instTokens.size(); ++tokenIdx)
    {
        const Token& token = instTokens[tokenIdx];

        if (token == ARGUMENT_SEPARATOR || token == END_OF_LINE)
        {
            if (argTokens.empty())
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
                args.push_back(argTokens);
                argTokens.clear();
            }
        }
        else
        {
            argTokens.push_back(token);
        }
    }
}

void SyntaxAnalyzer::throwError(const string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
