#include <cctype>
#include <iostream>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "tokens.h"

#define PRINT_TOKENS 0

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

bool isNumber(const string& token)
{
    size_t idx = 0;

    // determine base
    int base = 10;
    if (token.size() >= 2)
    {
        if ( token[0] == '0' && (token[1] == 'x' || token[1] == 'X') )
        {
            base = 16;
            idx += 2;
        }
    }

    if (base == 16)
    {
        for (; idx < token.size(); ++idx)
        {
            if (!isxdigit(token[idx]))
            {
                return false;
            }
        }
        return true;
    }
    else if (base == 10)
    {
        for (; idx < token.size(); ++idx)
        {
            if (!isdigit(token[idx]))
            {
                return false;
            }
        }
        return true;
    }

    // should not get here
    return false;
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

    // number
    if (isNumber(token))
    {
        return true;
    }

    return false;
}
