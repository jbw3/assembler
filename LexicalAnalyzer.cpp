#include <cctype>
#include <iostream>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "tokens.h"

#define PRINT_TOKENS 1

using namespace std;

constexpr bool isIdentifierChar(char ch, bool first)
{
    if (first)
    {
        return isalpha(ch) || ch == '_';
    }
    else
    {
        return isalnum(ch) || ch == '_';
    }
}

LexicalAnalyzer::LexicalAnalyzer() :
    token(""),
    isValid(false)
{
}

void LexicalAnalyzer::process(istream& is, vector<string>& tokens)
{
    tokens.clear();
    tokens.reserve(128);

    char ch;
    is.read(&ch, 1);
    while (!is.eof())
    {
        parseChar(ch, tokens);

        is.read(&ch, 1);
    }

    // check for leftover token
    if (!token.empty())
    {
        if (isValid)
        {
            tokens.push_back(token);
        }
        else
        {
            throw Error("Invalid token \"" + token + "\".");
        }
    }

#if PRINT_TOKENS
    cout << "Tokens:\n----------\n|";
    for (string t : tokens)
    {
        cout << t << "|";
    }
    cout << "\n----------\n";
#endif
}

void LexicalAnalyzer::parseChar(char ch, vector<string>& tokens)
{
    if (isblank(ch))
    {
        if (!token.empty())
        {
            if (isValid)
            {
                tokens.push_back(token);
                token = "";
                isValid = false;
            }
            else
            {
                throw Error("Invalid token \"" + token + "\".");
            }
        }
    }
    else // ch is not blank
    {
        if (isValid)
        {
            if (isValidToken(token + ch))
            {
                token += ch;
            }
            else
            {
                tokens.push_back(token);
                token = ch;
                isValid = isValidToken(token);
            }
        }
        else
        {
            token += ch;
            isValid = isValidToken(token);
        }
    }
}

bool LexicalAnalyzer::isValidToken(const string& token)
{
    // argument separator
    if (token == ARGUMENT_SEPARATOR)
    {
        return true;
    }

    // end of line
    if (token == END_OF_LINE)
    {
        return true;
    }

    // identifier (instruction, register, etc.)
    if (isIdentifierChar(token[0], true))
    {
        size_t idx = 1;
        for (; idx < token.size(); ++idx)
        {
            if (!isIdentifierChar(token[idx], false))
            {
                break;
            }
        }

        // if we made it through all the chars, this is an identifier
        if (idx == token.size())
        {
            return true;
        }
    }

    // decimal integer
    size_t idx = 0;
    for (; idx < token.size(); ++idx)
    {
        if (!isdigit(token[idx]))
        {
            break;
        }
    }

    // if we made it through all the chars, this is an integer
    if (idx == token.size())
    {
        return true;
    }

    return false;
}
