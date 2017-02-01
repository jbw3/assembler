#include <cctype>
#include <iostream>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "Logger.h"
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
    if (token.size() >= 2 && token[0] == '0')
    {
        switch (token[1])
        {
        case 'b':
        case 'B':
            base = 2;
            idx += 2;
            break;

        case 'o':
        case 'O':
            base = 8;
            idx += 2;
            break;

        case 'x':
        case 'X':
            base = 16;
            idx += 2;
            break;

        default:
            base = 10;
            break;
        }
    }

    if (base == 2)
    {
        for (; idx < token.size(); ++idx)
        {
            if (token[idx] != '0' && token[idx] != '1')
            {
                return false;
            }
        }
        return true;
    }
    else if (base == 8)
    {
        for (; idx < token.size(); ++idx)
        {
            if (token[idx] < '0' || token[idx] > '7')
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
    else if (base == 16)
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
            Logger::getInstance().logError("Invalid token \"" + token + "\".");
            throw Error();
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
                Logger::getInstance().logError("Invalid token \"" + token + "\".");
                throw Error();
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
