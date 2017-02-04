#include <cctype>
#include <iostream>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "Logger.h"

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

bool isNumber(const string& tokenStr)
{
    size_t idx = 0;

    // determine base
    int base = 10;
    if (tokenStr.size() >= 2 && tokenStr[0] == '0')
    {
        switch (tokenStr[1])
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
        for (; idx < tokenStr.size(); ++idx)
        {
            if (tokenStr[idx] != '0' && tokenStr[idx] != '1')
            {
                return false;
            }
        }
        return true;
    }
    else if (base == 8)
    {
        for (; idx < tokenStr.size(); ++idx)
        {
            if (tokenStr[idx] < '0' || tokenStr[idx] > '7')
            {
                return false;
            }
        }
        return true;
    }
    else if (base == 10)
    {
        for (; idx < tokenStr.size(); ++idx)
        {
            if (!isdigit(tokenStr[idx]))
            {
                return false;
            }
        }
        return true;
    }
    else if (base == 16)
    {
        for (; idx < tokenStr.size(); ++idx)
        {
            if (!isxdigit(tokenStr[idx]))
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
    tokenStr(""),
    isValid(false),
    line(0),
    column(0)
{
}

void LexicalAnalyzer::process(istream& is, vector<Token>& tokens)
{
    tokens.clear();
    tokens.reserve(128);

    line = 1;
    column = 1;

    char ch;
    is.read(&ch, 1);
    while (!is.eof())
    {
        parseChar(ch, tokens);

        if (ch == '\n')
        {
            ++line;
            column = 1;
        }
        else
        {
            ++column;
        }

        is.read(&ch, 1);
    }

    // check for leftover token
    if (!tokenStr.empty())
    {
        if (isValid)
        {
            addToken(tokens);
        }
        else
        {
            Logger::getInstance().logError("Invalid syntax: \"" + tokenStr + "\".", line, column - tokenStr.size());
            throw Error();
        }
    }

#if PRINT_TOKENS
    cout << "Tokens:\n----------\n|";
    for (const Token& t : tokens)
    {
        cout << t.getValue() << "|";
    }
    cout << "\n----------\n";
#endif
}

void LexicalAnalyzer::addToken(std::vector<Token>& tokens)
{
    tokens.push_back(Token(tokenStr, line, column - tokenStr.size()));
}

void LexicalAnalyzer::parseChar(char ch, vector<Token>& tokens)
{
    if (isblank(ch))
    {
        if (!tokenStr.empty())
        {
            if (isValid)
            {
                addToken(tokens);
                tokenStr = "";
                isValid = false;
            }
            else
            {
                Logger::getInstance().logError("Invalid syntax: \"" + tokenStr + "\".", line, column - tokenStr.size());
                throw Error();
            }
        }
    }
    else // ch is not blank
    {
        if (isValid)
        {
            if (isValidToken(tokenStr + ch))
            {
                tokenStr += ch;
            }
            else
            {
                addToken(tokens);
                tokenStr = ch;
                isValid = isValidToken(tokenStr);
            }
        }
        else
        {
            tokenStr += ch;
            isValid = isValidToken(tokenStr);
        }
    }
}

bool LexicalAnalyzer::isValidToken(const string& str)
{
    // argument separator
    if (str == ARGUMENT_SEPARATOR.getValue())
    {
        return true;
    }

    // end of line
    if (str == END_OF_LINE.getValue())
    {
        return true;
    }

    // identifier (instruction, register, etc.)
    if (isIdentifierChar(str[0], true))
    {
        size_t idx = 1;
        for (; idx < str.size(); ++idx)
        {
            if (!isIdentifierChar(str[idx], false))
            {
                break;
            }
        }

        // if we made it through all the chars, this is an identifier
        if (idx == str.size())
        {
            return true;
        }
    }

    // number
    if (isNumber(str))
    {
        return true;
    }

    return false;
}
