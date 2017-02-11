#include <cctype>
#include <iomanip>
#include <iostream>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "Logger.h"
#include "utils.h"

#define PRINT_CHARS  0
#define PRINT_TOKENS 0

using namespace std;

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

#if PRINT_CHARS
    cout << "line: col:ch:token\n";
#endif

    char ch;
    is.read(&ch, 1);
    while (!is.eof())
    {
        parseChar(ch, tokens);

#if PRINT_CHARS
        {
            string tempToken = tokenStr;
            for (auto iter = tempToken.find('\n'); iter != string::npos; iter = tempToken.find('\n'))
            {
                tempToken.replace(iter, iter + 1, "\\n");
            }

            cout << setw(4) << line << "|"
                 << setw(4) << column << "|"
                 << setw(2) << (ch == '\n' ? "\\n" : string(1, ch)) << "|"
                 << tempToken << "|"
                 << '\n';
        }
#endif

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
            throwError();
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
                throwError();
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
            // if we reached the end of a line and the last token is
            // not valid, report an error
            if (!tokenStr.empty() && string(1, ch) == END_OF_LINE.getValue())
            {
                throwError();
            }
            else
            {
                tokenStr += ch;
                isValid = isValidToken(tokenStr);
            }
        }

        // if the token string is the end of the line, go ahead
        // and add it before line is incremented
        if (tokenStr == END_OF_LINE.getValue())
        {
            ++column;
            addToken(tokens);
            tokenStr = "";
            isValid = false;
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

    // label separator
    if (str == LABEL_SEPARATOR.getValue())
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

void LexicalAnalyzer::throwError()
{
    Logger::getInstance().logError("Invalid syntax: \"" + tokenStr + "\".", line, column - tokenStr.size());
    throw Error();
}
